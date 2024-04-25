/// \file RampantGrowth.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/rampantgrowth.


#include <iostream>
#include <cmath>

const unsigned long long MOD = 998244353;

int
main ()
{
    int r, c;
    std::cin >> r >> c;

    // The plant in column 0 can go in any of r rows.
    // The plant in each other column can go in any of r-1 rows.

    unsigned long long result = r;
    for (int i = 0; i < c - 1; ++i) {
        result *= (r - 1);
        result %= MOD;
    }
    std::cout << result << "\n";
    return 0;
}
