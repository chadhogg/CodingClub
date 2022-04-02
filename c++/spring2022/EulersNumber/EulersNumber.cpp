/// \file EulersNumber.cpp
/// \brief A solution to https://open.kattis.com/problems/eulersnumber
/// \author Chad Hogg
/// \version 2022-04-01

#include <iostream>
#include <iomanip>

double approximate(unsigned long long seriesLength) {
  long double result = 1.0;
  unsigned long long factorial = 1ULL;
  for (unsigned long long counter = 1ULL; counter <= seriesLength; ++counter) {
    factorial *= counter;
    result += (1.0 / (long double)factorial);
  }
  return result;
}

double approximate2(unsigned long long seriesLength) {
  long double result = 1.0;
  long double term = 1.0;
  for (unsigned long long counter = 1ULL; counter <= seriesLength; ++counter) {
    term /= counter;
    result += term;
  }
  return result;
}

int main(int argc, char* argv[]) {
  unsigned long long input;
  std::cin >> input;
  std::cout << std::fixed << std::setprecision(20) << approximate2 (input) << "\n";
  return 0;
}