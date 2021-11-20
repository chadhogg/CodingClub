/// \file Generator.cpp
/// \author Chad Hogg
/// A program that generates instances of https://open.kattis.com/problems/10kindsofpeople

#include <iostream>
#include <cstdlib>

int main (int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " <numRows> <numCols> <numQueries>\n";
    return 1;
  }
  unsigned int numRows = atoi (argv[1]);
  unsigned int numCols = atoi (argv[2]);
  unsigned int numQueries = atoi (argv[3]);

  std::cout << numRows << " " << numCols << "\n";
  for (unsigned int row {0U}; row < numRows; ++row) {
    for (unsigned int col {0U}; col < numCols; ++col) {
      std::cout << (rand () % 2);
    }
    std::cout << "\n";
  }

  for (unsigned int query {0U}; query < numQueries; ++query) {
    std::cout << rand () % numRows + 1 << " " << rand () % numCols + 1 << " "
	      << rand () % numRows + 1 << " " << rand () % numCols + 1
	      << "\n";
  }
  return 0;
}
