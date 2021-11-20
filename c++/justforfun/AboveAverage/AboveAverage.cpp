/// \file AboveAverage.cpp
/// \author Chad Hogg
/// \version 2021-11-20
/// \brief A solution to https://open.kattis.com/problems/aboveaverage

#include <iostream>
#include <vector>
#include <iomanip>

/// \brief Reads in test scores, prints number above average.
void doCase () {
  unsigned int numStudents, total, score;
  std::vector<unsigned int> grades;
  std::cin >> numStudents;
  grades.reserve (numStudents);
  total = 0;
  for (unsigned int index {0U}; index < numStudents; ++index) {
    std::cin >> score;
    total += score;
    grades.push_back (score);
  }
  double average = total / (double)numStudents;
  unsigned int count = 0;
  for (unsigned int index {0U}; index < numStudents; ++index) {
    if (grades[index] > average) {
      ++count;
    }
  }
  std::cout << std::fixed << std::setprecision(3) << 100.0 * count / (double)numStudents << "%\n";
}

/// \brief Runs the program.
/// \return 0.
int main () {
  unsigned int numCases;
  std::cin >> numCases;
  for (unsigned int count {0U}; count < numCases; ++count) {
    doCase ();
  }
  
  return 0;
}
