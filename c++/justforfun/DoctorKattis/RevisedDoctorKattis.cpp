/// \file DoctorKattis.cpp
/// \brief An attempt at solving https://open.kattis.com/problems/doctorkattis.
/// \author Chad Hogg
/// \version 2021-11-13
///
/// One naive approach to the problem is to store information about each cat in
///   a priority queue.  This makes queries constant time and arrivals log time
///   but both updates and removals and linear in the number of cats.  My
///   experiments in Java demonstrated that this is way too slow.
///
/// An alternative naive approach is to store information about each cat in a
///   hash table indexed by cat name.  This makes arrives, updates, and
///   treatments all constant time, but queries are linear.  Also too slow.
///
/// I tried taking advantage of the fact that there are only 70 distinct
///   infection values to try a variant of the second naive approach with 70
///   different hash tables.  This improves queries by a factor of 70 (on
///   average), but is not enough to make it feasible on large problems.
///
/// I had a thought of using a priority queue but caching updates and
///   treatments outside of it, applying them only to cats that found their
///   way to the top of the priority queue and then re-inserting them.  This
///   was fast, but it doesn't actually work.  Since each update increases a
///   cat's priority, there's no way to know for sure which is the highest
///   priority without actually applying every update.  (Or at least, I
///   couldn't find a way.)
///
/// This led to my latest idea, which is weird:
///   Cat information is stored in both a priority queue *and* a hash table.
///   The hash table stores only the most *recent* version of each cat, but the
///     priority queue stores potentially many versions of it.
///   For arrivals, we insert into both data structures in logarithmic time.
///   For updates, we mark the old priority queue entry as invalid (doesn't
///     require searching for it, because the hash table stores a pointer to
///     it, and doesn't require re-heapifying because validity doesn't affect
///     ordering) and insert a new, updated version into the priority queue
///     while replacing the hash table entry.  So this is constant time.
///   For treatments we mark the old priority queue entry as invalid and remove
///     from the hash table, in constant time.
///   For queries, we remove from the priority queue until its top is valid,
///     then return it.  Each removal is logarithmic in the number of entries
///     (both valid and invalid).  The number of removals we will need to do is
///     linear in the number of updates and treatments that have occurred in
///     past, but it should be noted that each invalid entry only needs to be
///     removed during processing of a single query.  And since invalid entries
///     tend to have low priorities, most of them won't ever make it to the top
///     anyway.  Operations on the priority queue take time logarithmic in the
///     total number of arrivals + updates rather than just the number of
///     arrivals, but this is still bounded by the total number of commands.
///   This approach took ~1.25 seconds to solve hard problems when I coded it
///     in Java, which is what led me to try translating the code into C++.
///   It takes 0.73 seconds to solve the problem with this program.  But I
///     think the 1 second Kattis limit is cumulative across several problems.
///     I guess we'll try it ...
///   It was accepted!
///
/// Profiling results:
///   54% of my time is in std::unordered_map::operator[] and descendants
///   18% of my time is in std::pop_heap() and descendants
///   13% of my time is in std::push_heap() and descendants
///    6% of my time is in std::unorderd_map::erase() and descendants
///   Nothing else uses more than 5% of CPU time.
///
/// \note I am using uint8_t to store infection levels, since they must be in
///   the range [30, 100].  I discovered the hard way that trying to extract a
///   uint8_t from an input stream does not work -- instead of getting the next
///   numeric token, you get the ASCII value of the next character.
///
/// \note I'd originally hoped to have the priority queue store cat objects
///   directly, but I need indirection because I need to be able to modify the
///   cat objects through pointers, and if the queue stored the objects
///   directly their locations would be changing whenever I reheapify.


#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>


/// The maximum number of commands the input should ever contain.
static const unsigned int MAX_COMMANDS = 1000000U;
/// The maximum number of cats that should ever arrive at the clinic.
static const unsigned int MAX_CATS = 200000U;
/// The lowest possible infection level for a cat.
static const unsigned int MIN_INFECTION = 30U;
/// The highest possible infection level for a cat.
static const unsigned int MAX_INFECTION = 100U;


/// \brief The types of commands that can exist in the input.  Their values
///   match the actual symbols in the input that will represent them.

// WK: i made this a "strong" enum to prevent implicit conversion
enum class CommandType : unsigned int {
  ArriveAtClinic = 0U,
  UpdateInfectionLevel = 1U,
  Treated = 2U,
  Query = 3U,
  Count = 4U
};

// Will exist in C++23
template <typename EnumClass>
auto
to_underlying(EnumClass const& e) {
  return static_cast<typename std::underlying_type<EnumClass>::type>(e);
}

/// \brief A command that will need to be processed.
class Command {
public:
  /// Either ArriveAtClinic, UpdateInfectionLevel, Treated, or Query.
  const CommandType type;
  /// The name of the cat to be adjusted.  (Irrelevant for Queries.)
  const std::string name;
  /// The original / change in infection level.  (Irrelevant for Treateds and
  ///   Queries.)
  const unsigned int level;

  /// \brief Constructs a new command.
  /// \param[in] t The type of the new command.
  /// \param[in] n The name of the cat involved in the command, or "".
  /// \param[in] l THe infection level involved in the command, or 0.
  Command (const CommandType t, const std::string& n, unsigned int l)
    : type (t), name (n), level (l) {
  }
};


/// \brief Extracts a new command from an input stream.
/// \param[inout] in The input stream.
/// \return A pointer to the command that was extracted.  The caller will be
///   responsible for deleting it.
static Command readCommand (std::istream& in) {
  unsigned int t;
  // WK: default initialized to empty string
  std::string name; 
  // WK: I prefer brace-initialization to assignment since it's stronger (will not perform narrowing conversions)
  unsigned int level {0};
  in >> t;
  // WK: I believe that exceptions are okay to use in C++ when exceptional behavior occurs
  if (t >= to_underlying(CommandType::Count)) {
    throw std::invalid_argument("Unknown command type specified");
  }
  const CommandType type {t};
  
  // else cases removed due to intial values being specified / implied
  if (type == CommandType::ArriveAtClinic || type == CommandType::UpdateInfectionLevel || type == CommandType::Treated) {
    in >> name;
  }
  if (type == CommandType::ArriveAtClinic || type == CommandType::UpdateInfectionLevel) {
    in >> level;
  }
  return Command {type, name, level};
};


/// \brief Information about a cat that is (or has been) waiting at the clinic.
/// Note that this may be valid (all values accurately describe the cat as it
///   currently exists) or invalid (another object of this type has superceded
///   it).
class Cat {
public:
  /// \brief Constructs a new cat.
  /// \param[in] n The name of the cat.
  /// \param[in] i The infection level of the cat.
  /// \param[in] t The time-step at which the cat arrived.
  /// \post All parameters have been copied, and the cat is valid.
  Cat (const std::string &n, unsigned int i, uint32_t t)
    : name {n}, infection {i}, time {t}, valid {true} {
  }

  /// \brief Gets the name of this cat.
  /// \return The name.
  inline const std::string& getName () const { return name; }

  /// \brief Gets the infection level of this cat.
  /// \return The infection level.
  inline unsigned int getInfection () const { return infection; }

  /// \brief Gets the time at which this cat arrived.
  /// \return The arrival time.
  inline uint32_t getTime () const { return time; }

  /// \brief Gets whether or not this object has the most up-to-date info about
  ///   the cat.
  /// \return Whether or not this object is valid.
  inline bool isValid () const { return valid; }

  /// \brief Marks this object as out of date.
  /// \post This object is no longer valid.
  inline void invalidate () { valid = false; }

private:
  /// The name of the cat.
  std::string name;
  /// The cat's infection level.

  // WK: you don't actually save any space here unless you reorder the class to have time come before infection
  //     as you noticed, uint8_t is a pain to deal with, so I personally just use unsigned int
  unsigned int infection;

  /// The time step at which the cat arrived.
  uint32_t time;
  //
  bool valid;

// WK: making the stream insertion operator an inline friend of the class results in your ability to call private
// members without needing to call public functions or if there was something in the representation you wanted to
// keep private
friend inline std::ostream& operator<< (std::ostream& os, const Cat& c) {
  os << c.name << " (" << c.infection << ", " << c.time << ")";
  return os;
}
};


/// \brief A functor for comparing pointers to cats.
/// The ordering implied here is that cats with a higher infection level have
///   a higher priority level, and among cats with the same infection level
///   those with earlier arrival times have higher priority.  Cats with the
///   same infection level and arrival time have equivalent priorities.
class CatPointerComparator {
public:
  /// \brief Compares two cats, through pointers to them.
  /// \param[in] x A pointer to one cat.
  /// \param[in] y A pointer to another cat.
  /// \return True if the first cat should come before the second cat in an
  ///   ordering.  (Have lower priority.)

  // WK: any member function that is non-modifying should be marked as const.
  //     specifically, the signature should also accept const pointers.
  inline bool operator() (const std::unique_ptr<Cat>& x, const std::unique_ptr<Cat>& y) const {
    if (x->getInfection () > y->getInfection ()) {
      return false;
    }
    if (x->getInfection () == y->getInfection ()) {
      return x->getTime () > y->getTime ();
    }
    return true;
  }
};

static CatPointerComparator catComparator{};


/// \brief Inserts a representation of a vector of cat pointers into an output
///   stream.
/// \param[inout] os The output stream.
/// \param[in] cats The vector.
/// \return The output stream.
/// \post Each pointed-to cat has been inserted into the stream, in order.
/// This only exists to aid in debugging.
static std::ostream& operator<< (std::ostream& os, const std::vector<Cat*>& cats) {
  // WK: range-based for-loops in C++ make this cleaner
  for (Cat* cat : cats) {
    // WK: flushing after each line is a bit unnecessary
    os << *cat << '\n'; 
  }
  // WK: flush at end
  os.flush();
  return os;
}


/// \brief A class that models the cat clinic.
class Clinic {
public:
  /// \brief Constructs a new clinic.
  /// \param[in] numCommands The number of commands the clinic can expect to see.
  Clinic (uint32_t numCommands)
    : pq (), cats () {
    pq.reserve (numCommands);
    cats.reserve (std::min (numCommands, MAX_CATS));
  }

  /// \brief A new cat arrives at the clinic.
  /// \param[in] command The arrival command.
  /// \param[in] time The current time step.
  /// \post The clinic will remember that the new cat is waiting.
  void arrival (const Command& command, uint32_t time) {
    // WK: switched to unique_ptr so no manual delete is necessary :)
    auto cat = std::make_unique<Cat>(command.name, command.level, time);
    cats[command.name] = cat.get();
    // very important to move the unique_ptr into the PQ
    pq.push_back (std::move (cat));
    std::push_heap (pq.begin (), pq.end (), catComparator);
  }

  /// \brief A cat's infection level has increased.
  /// \param[in] command The update command.
  /// \post The clinic knows about the cat's updated infection level.
  /// In practice, this means that the old entry in the priority queue has been
  ///   invalidated, and a new one inserted.
  void update (const Command& command) {
    if (command.level > 0) {
      Cat* oldCat = cats[command.name];
      oldCat->invalidate();
      auto newCat = std::make_unique<Cat>(oldCat->getName(), oldCat->getInfection() + command.level, oldCat->getTime());
      cats[command.name] = newCat.get();
      pq.push_back (std::move (newCat));
      std::push_heap (pq.begin (), pq.end (), catComparator);
    }
  }

  /// \brief A cat has been treated and discharged.
  /// \param[in] command The treated command.
  /// \post The clinic no longer stores information about the cat.
  /// In practice, this means that the entry in the priority queue has been
  ///   invalidated.
  void treated (const Command& command) {
    cats[command.name]->invalidate();
    cats.erase (command.name);
  }

  /// \brief Someone wants to know the highest-priority cat.
  /// \post The name of the highest priority cat has been printed, or if there
  ///   are no cats in the clinic "The clinic is empty" was printed instead.
  /// In practice, several invalid entries may have been removed from the
  ///   priority queue until a valid entry is found at its top.
  void query () {
    while (!pq.empty() && !pq.front()->isValid()) {
      std::pop_heap(pq.begin(), pq.end(), catComparator);
      pq.pop_back();
    }
    if (pq.empty ()) {
      std::cout << "The clinic is empty" << std::endl;
    }
    else {
      std::cout << pq.front()->getName () << std::endl;
    }
  }
  
private:
  /// A priority queue of entries about cats in the clinic, including invalid
  ///   entries that have been left in the priority queue because finding them
  ///   and removing them would be too expensive.
  std::vector<std::unique_ptr<Cat>> pq;
  /// A map from the names of cats currently waiting at the clinic to their
  ///   current valid priority queue entries.
  std::unordered_map<std::string, Cat*> cats;
};

/// \brief Runs the program.
/// \return Always 0, unless we exited earlier.
int main () {
  uint32_t commandCount;
  std::cin >> commandCount;

  Clinic clinic {commandCount};
  // WK: no need to make a list of commands -- just walk through them once
  for (uint32_t time {0}; time < commandCount; ++time) {
    const Command command = readCommand(std::cin);
    switch (command.type) {
    case CommandType::ArriveAtClinic:
      clinic.arrival (command, time);
      break;
    case CommandType::UpdateInfectionLevel:
      clinic.update (command);
      break;
    case CommandType::Treated:
      clinic.treated (command);
      break;
    case CommandType::Query:
      clinic.query ();
      break;
    default:
      throw std::runtime_error("invariant failed");
    }
  }
  return 0;
}
