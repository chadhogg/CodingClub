/// \file A1Paper.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/a1paper

#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <cassert>

/// The length of the long side of a sheet of A1 paper.
const double BASE_LONG_SIDE = pow (2, -0.75);
/// The length of the short side of a sheet of A1 paper.
const double BASE_SHORT_SIDE = pow (2, -1.25);
/// The largest size of paper involved in the problem.
const unsigned int SMALLEST_SIZE {30};

/// \brief A stream insertion operator for vectors, for debugging.
/// \param[inout] os The stream into which it should be inserted.
/// \param[in] container The vector that should be inserted.
/// \return The stream.
/// \post A representation like [1, 2, 3] has been inserted into the stream.
template<typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& container) {
  os << "[";
  bool first = true;
  for (auto it = container.cbegin (); it != container.cend (); ++it) {
    if (!first) {
      os << ", ";
    }
    os << *it;
    first = false;
  }
  os << "]";
  return os;
}

static double longSide (unsigned int);
static double shortSide (unsigned int);

/// \brief Calculates the length of the long side of an Ax piece of paper.
/// \param[in] index The paper size minus 1.
/// \return The length of its longer side, in meters.
static double longSide (unsigned int index) {
  // Memoization, because some algorithms will call this quite frequently.
  static double cache[SMALLEST_SIZE] {};
  static bool valid[SMALLEST_SIZE] {};
  if (index == 0U) {
    throw std::invalid_argument ("You shouldn't be asking for the size of A1.");
  }
  else if (index == 1U) {
    return BASE_LONG_SIDE;
  }
  else if (valid[index]) {
    return cache[index];
  }
  else {
    if (!valid[index]) {
      // All A-sizes maintain a sqrt(2):1 ratio of long to short side.
      cache[index] = shortSide (index) * sqrt (2.0);
      valid[index] = true;
    }
    return cache[index]; 
  }
}

/// \brief Calculates the length of the short side of an Ax piece of paper.
/// \param[in] The paper size minus 1.
/// \return The length of its shorter side, in meters.
static double shortSide (unsigned int index) {
  if (index == 0U) {
     throw std::invalid_argument ("You shouldn't be asking for the size of A1.");
  }
  else if (index == 1U) {
    return BASE_SHORT_SIDE;
  }
  else {
    // The short side of any paper is half the long side of the next largest.
    return 0.5 * longSide (index - 1U);
  }
}

/// \brief Calculates the amount of tape needed to make an A1 sheet of paper.
/// \param[inout] counts A vector showing the number of A1s, A2s, ... we have.
/// \return The total amount of tape needed to combine smaller sizes into a A1.
/// \note This runs too slowly on large problems, because it only combines 2
///   sheets of paper at a time.  See fasterToMake () for a version that runs
///   exponentially faster.  This one is useful for confirming that the more
///   complicated algorithm produces the right answer though.
static double toMake (std::vector<unsigned long>& counts) {
  double total {0.0};
  // We must keep combining until we have an A1.
  while (counts[0] == 0) {
    bool found {false};
    // Search for the largest 2 sheets of paper we could combine.
    for (unsigned int index = 1; index < counts.size () && !found; ++index) {
      if (counts[index] >= 2) {
	// Combine them and count the tape used to do so.
	counts[index] -= 2;
	counts[index - 1] += 1;
	total += longSide (index);
	found = true;
      }
    }
    // If we couldn't combine any paper, the problem is unsolveable.
    if (!found) {
      throw std::runtime_error("impossible");
    }
  }
  return total;
}

/// \brief Combines several sheets of paper, if possible.
/// \param[inout] counts A vector showing the number of A1s, A2s, ... we have.
/// \param[in] index The type of paper we want to make (0 -> A1, 1 -> A2, ...).
/// \param[in] needed How many of that paper we want to make.
/// \return The total amount of tape needed to make it.
static double fasterToMake (std::vector<unsigned long>& counts, unsigned int index, unsigned long needed) {
  // Beyond the bounds of the array can't be made.
  if (index == counts.size ()) {
    throw std::runtime_error ("impossible");
  }
  else {
    double tape = 0.0;
    if (counts[index] < needed) {
      unsigned int additional = needed - counts[index];
      // If I don't already have enough, make them from the next smallest size.
      // I will need 2 of the smaller size for each of this size I want.
      tape += fasterToMake (counts, index + 1, additional << 1);
    }
    assert (counts[index] >= needed);
    
    if (index != 0) {
      // Further combine all the sheets of paper of this size into the next
      //   largest.
      counts[index] -= needed;
      counts[index - 1] += needed / 2;
      tape += needed / 2 * longSide (index);
    }
    return tape;
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  unsigned int smallest_size, temp;
  std::cin >> smallest_size;

  std::vector<unsigned long> counts {};
  counts.reserve (smallest_size);
  counts.push_back (0);
  for (unsigned int index = 1; index < smallest_size; index++) {
    std::cin >> temp;
    counts.push_back (temp);
  }

  try {
    double result {fasterToMake (counts, 0U, 1UL)};
    std::cout.precision (std::numeric_limits<double>::digits10);
    std::cout << result << std::endl;
  }
  catch (std::runtime_error e) {
    std::cout << "impossible" << std::endl;
  }
  return 0;
}
