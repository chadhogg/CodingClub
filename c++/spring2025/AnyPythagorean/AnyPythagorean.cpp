/// \file AnyPythagorean.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/anypythagorean

#include <iostream>
#include <cassert>
#include <cmath>

int
main()
{
    int circumference;
    std::cin >> circumference;
    for (int a = 1; a < circumference; ++a) {
        for (int b = 1; b < circumference; ++b) {
            int csquared = a * a + b * b;
            int c = (int)sqrt (csquared);
            if (a + b + c == circumference && c *c == csquared) {
                std::cout << a << " " << b << " " << c << "\n";
                return 0;
            }
        }
    }
    std::cout << 0 << " " << 0 << " " << 0 << "\n";
    return 0;
}