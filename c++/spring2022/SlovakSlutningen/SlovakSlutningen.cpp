/// \file SlovakSlutningen.cpp
/// \brief A solution to https://open.kattis.com/problems/slovakslutningen
/// \author Chad Hogg
/// \version 2022-04-01

#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <algorithm>

struct Problem {
  unsigned int numLines;
  unsigned int numColumns;
  unsigned int numClasses;
  std::vector<std::vector<char>> seatingChart;
};

unsigned int findMaxColors(Problem const& prob) {
  std::set<std::set<char>> hats;
  for (char hat = 'A'; hat < 'A' + prob.numClasses; ++hat) {
    std::set<char> singleton;
    singleton.insert (hat);
    hats.insert (singleton);
  }

  for (unsigned int column = 0; column < prob.numColumns; ++column) {
    std::set<char> columnContents;
    for (unsigned int row = 0; row < prob.numLines; ++row) {
      columnContents.insert (prob.seatingChart[row][column]);
    }
    // for each set in hats, if the intersection of it with columnContents is non-empty, remove it from hats and add its contents to columnContents
    for (std::set<std::set<char>>::iterator iter = hats.begin (); iter != hats.end (); ) {
      std::set<char> intersection;
      std::set_intersection (columnContents.begin (), columnContents.end (), iter->begin (), iter->end (), std::inserter (intersection, intersection.begin ()));
      if (!intersection.empty ()) {
        columnContents.insert (iter->begin (), iter->end ());
        iter = hats.erase (iter);
      }
      else {
        ++iter;
      }
    }
    // then add columnContents to hats
    hats.insert (columnContents);
  }

  return hats.size ();
}

Problem readInput () {
  Problem prob;
  char person;
  std::cin >> prob.numLines >> prob.numColumns >> prob.numClasses;
  for (unsigned int i = 0U; i < prob.numLines; ++i) { prob.seatingChart.push_back (std::vector<char> ()); }
  for (unsigned int line = 0U; line < prob.numLines; ++line) {
    for (unsigned int col = 0U; col < prob.numColumns; ++col) {
      std::cin >> person;
      prob.seatingChart[line].push_back (person);
    }
  }
  return prob;
}

int main(int argc, char* argv[]) {
  std::cout << findMaxColors (readInput ()) << "\n";
  return 0;
}