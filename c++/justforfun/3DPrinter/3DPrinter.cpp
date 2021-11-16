/// \file 3DPrinter.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/3dprinter

#include <iostream>

/// \brief Runs the program.
/// \return Always 1.
int main () {
  unsigned int numStatuesToPrint;
  std::cin >> numStatuesToPrint;

  unsigned int numPrinters {1U};
  unsigned int days {0U};

  // It's always worthwhile to double the number of printers until you can
  //   print them all in one day.
  // I suppose you could figure this out with a bit shift or something instead.
  while (numPrinters < numStatuesToPrint) {
    numPrinters *= 2;
    ++days;
  }

  ++days;

  std::cout << days << std::endl;
  
  return 0;
}
