/// \file ContactTracing.cpp
/// \brief An attempt at solving https://open.kattis.com/problems/contacttracing.
/// \author Chad Hogg
/// \version 2022-12-02
/// Possible improvement: Maybe I can merge all of the new infected people in a single pass rather than multiple.

#include <iostream>
#include <list>
#include <set>
#include <cassert>
#include <stdexcept>
#include <vector>

using Range = std::pair<unsigned long, unsigned long>;

struct Person
{
  unsigned index;
  Range range;
};

bool
operator< (const Range& r1, const Range& r2)
{
  if (r1.first < r2.first) { return true; }
  if (r1.first == r2.first && r1.second < r2.second) { return true; }
  return false;
}

bool
operator< (const Person& p1, const Person& p2)
{
  return p1.range < p2.range || (p1.range == p2.range && p1.index < p2.index);
}

struct Problem
{
  /// The number of additional days to process.
  unsigned daysLeft;
  /// A set of the indices of the people who are infected.
  std::set<unsigned> infectedIndexes;
  /// A set of non-intersecting ranges during which an infected person is in the room.
  /// \note Maybe it should be sorted?
  std::set<Range> infectedRanges;
  /// A set of not-yet-infected people who will enter the room.
  std::set<Person> uninfected;
};

bool
contains (const Range& outer, const Range& inner)
{
  return (outer.first <= inner.first && outer.second >= inner.second);
}

bool
intersects (const Range& r1, const Range& r2)
{
  return 
    // r1 completely encloses r2
    (r1.first <= r2.first && r1.second >= r2.second)
    // r1's start point is inside of r2
    || (r1.first >= r2.first && r1.first <= r2.second)
    // r1's end point is inside of r2
    || (r1.second >= r2.first && r1.second <= r2.second);
}

void
mergeRange (std::set<Range>& ranges, const Range& additional)
{
  Range toAdd = additional;
  auto it = ranges.begin ();
  while (it != ranges.end ())
  {
    const Range& r = *it;
    if (contains (r, toAdd))
    {
      // We don't actually need to add this, as it is entirely inside an existing range.
      return;
    }
    if (intersects (r, toAdd))
    {
      // Remove the existing range, replace toAdd with their union.
      toAdd.first = std::min (toAdd.first, r.first);
      toAdd.second = std::max (toAdd.second, r.second);
      it = ranges.erase (it);
    }
    else if (toAdd.first > r.second)
    {
      // Past the last one that could possibly intersect it, so don't bother checking others.
      break;
    }
    else
    {
      ++it;
    }
  }
  ranges.insert (toAdd);
}

Problem readInput ()
{
  Problem prob;
  unsigned N, D, C, index;
  unsigned long s, t;
  std::cin >> N >> D >> C;
  prob.daysLeft = D;
  for (unsigned i = 0; i < C; ++i)
  {
    std::cin >> index;
    prob.infectedIndexes.insert (index);
  }
  for (unsigned i = 0; i < N; ++i)
  {
    std::cin >> s >> t;
    if (prob.infectedIndexes.count (i + 1) == 1)
    {
      mergeRange (prob.infectedRanges, {s, t});
    }
    else
    {
      prob.uninfected.insert ({i + 1, {s, t}});
    }
  }
  return prob;
}

void
solve (Problem& prob)
{
  while (prob.daysLeft > 0)
  {
    std::list<Person> newInfected;
    auto it = prob.uninfected.begin ();
    while (it != prob.uninfected.end ())
    {
      const Person& person = *it;
      bool found = false;
      for (const Range& range : prob.infectedRanges)
      {
        if (intersects (person.range, range))
        {
          found = true;
          break;
        }
      }
      if (found)
      {
        newInfected.push_back (person);
        it = prob.uninfected.erase (it);
      }
      else
      {
        ++it;
      }
    }
    for (Person& person : newInfected)
    {
      prob.infectedIndexes.insert (person.index);
      mergeRange (prob.infectedRanges, person.range);
    }
    --prob.daysLeft;
  }
}

const unsigned DELETED = 0;

void
solve2 (Problem& prob)
{
  // Switching to a sorted vector at the suggestion of WKK, so that iteration becomes faster.
  std::vector<Person> allPeople (prob.uninfected.begin (), prob.uninfected.end ());
  while (prob.daysLeft > 0)
  {
#ifdef DEBUG
    for (const Range& range : prob.infectedRanges)
    {
      std::cout << "(" << range.first << " - " << range.second << ") ";
    }
    std::cout << "\n";
    for (const Person& person : prob.uninfected)
    {
      std::cout << "(" << person.range.first << " - " << person.range.second << ") ";
    }
    std::cout << "\n\n";
#endif//DEBUG

    std::set<Person> newInfected;
    auto personIt = allPeople.begin ();
    auto rangeIt = prob.infectedRanges.begin ();
    bool changed = false;
    while (personIt != allPeople.end () && rangeIt != prob.infectedRanges.end ())
    {
      const Person& person = *personIt;
      const Range& range = *rangeIt;

      if (person.index == DELETED)
      {
        // Skip this person, as they are already infected.
        ++personIt;
      }
      else if (intersects (person.range, range))
      {
        // This uninfected person intersects with a infected range, so mark them as deleted and move on.
        newInfected.insert (person);
        personIt->index = DELETED;
        ++personIt;
        changed = true;
      }
      else if (person.range.first > range.second)
      {
        // This uninfected person doesn't show up until that range ends.
        // Every future person has a start time >= this person's start time.
        // So no future person can intersect this range, and we can move on to next range.
        ++rangeIt;
      }
      else
      {
        // This uninfected person is entirely before this range.
        // Every future range has an start time > this range's start time.
        // So no future range can intersect this person, and we can move on to the next person.
        if (person.range.second >= range.first) { throw std::runtime_error ("Impossible situation."); }
        ++personIt;
      }
    }
    for (const Person& person : newInfected)
    {
      prob.infectedIndexes.insert (person.index);
      mergeRange (prob.infectedRanges, person.range);
    }
    --prob.daysLeft;
    if (!changed) { break; }
  }
#ifdef DEBUG
  for (const Range& range : prob.infectedRanges)
  {
    std::cout << "(" << range.first << " - " << range.second << ") ";
  }
  std::cout << "\n";
  for (const Person& person : prob.uninfected)
  {
    std::cout << "(" << person.range.first << " - " << person.range.second << ") ";
  }
  std::cout << "\n\n";
#endif//DEBUG
}

/// \brief Runs the program.
/// \return Always 0, unless we exited earlier.
int main () {
  Problem prob = readInput ();
  solve2 (prob);
  for (unsigned index : prob.infectedIndexes)
  {
    std::cout << index << " ";
  }
  std::cout << "\n";
  return 0;
}
