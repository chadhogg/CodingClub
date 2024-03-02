/// \file Bus.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/bus.


#include <iostream>
#include <cmath>

int
solve (int numStops)
{
    int halfPassengers = 0;
    for (int i = 0; i < numStops; ++i) {
        halfPassengers += 1;
        halfPassengers *= 2;
    }
    return halfPassengers / 2;
}

int
main ()
{
    int T, k;
    std::cin >> T;
    for (int i = 0; i < T; ++i) {
        std::cin >> k;
        std::cout << solve (k) << "\n";
    }
    return 0;
}
