/// \file ABC.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/abc

#include <iostream>
#include <string>
#include <algorithm>

/// How many numbers will be in the input.
const unsigned int NUM_NUMS {3U};

/// \brief Runs the program.
/// \return Always 0.
int main () {
  int nums[NUM_NUMS];
  std::string str;
  for (unsigned int index {0U}; index < NUM_NUMS; ++index) {
    std::cin >> nums[index];
  }
  std::cin >> str;
  
  std::sort (nums, nums + NUM_NUMS);
  
  for (char c : str) {
    std::cout << nums[c - 'A'] << " ";
  }
  std::cout << std::endl;

  return 0;
}
