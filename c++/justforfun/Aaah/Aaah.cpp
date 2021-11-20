/// \file Aaah.cpp
/// \author Chad Hogg
/// \version 2021-11-16
/// \brief A solution to https://open.kattis.com/problems/aaah
/// \note It doesn't get much more trivial than this.

#include <iostream>
#include <string>

/// \brief Runs the program.
/// \return Always 0, no matter what.
int main () {
  std::string canDo, needed;
  std::cin >> canDo >> needed;
  if (canDo.length () >= needed.length ()) {
    std::cout << "go" << std::endl;
  }
  else {
    std::cout << "no" << std::endl;
  }
  return 0;
}
