/// \file GeneralizedFizzBuzz.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/generalizedfizzbuzz

#include <iostream>

int
main()
{
    int n, a, b;
    std::cin >> n >> a >> b;
    int fizz = 0, buzz = 0, fizzbuzz = 0;
    for (int i = 1; i <= n; ++i) {
        if (i % a == 0 && i % b == 0) { ++fizzbuzz; }
        else if (i % a == 0) { ++fizz; }
        else if (i % b == 0) { ++buzz; }
    }
    std::cout << fizz << " " << buzz << " " << fizzbuzz << "\n";
    return 0;
}