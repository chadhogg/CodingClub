/// \file TheBusCard.cpp
/// \brief An attempt at solving https://open.kattis.com/problems/busskortet.
/// \author Chad Hogg
/// \version 2022-12-02

#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>




/// \brief Runs the program.
/// \return Always 0, unless we exited earlier.
int main () {
  int K;
  std::cin >> K;
  int transactions = 0;
  /*
  for (int denom : {500, 200, 100})
  {
    transactions += K / denom;
    K %= denom;
  }
  if (K > 0)
  {
    ++transactions;
  }
  */
  while (K > 400)
  {
    K -= 500;
    ++transactions;
  }
  while (K > 100)
  {
    K -= 200;
    ++transactions;
  }
  while (K > 0)
  {
    K -= 100;
    ++transactions;
  }
  std::cout << transactions << "\n";
  return 0;
}
