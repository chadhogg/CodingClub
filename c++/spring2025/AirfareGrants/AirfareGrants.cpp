/// \file AirfareGrants.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/airfaregrants

#include <iostream>
#include <cassert>


int
main()
{
    int numPrices, smallest, largest, current;
    std::cin >> numPrices >> current;
    smallest = largest = current;
    for (int i = 1; i < numPrices; ++i) {
        std::cin >> current;
        smallest = std::min (smallest, current);
        largest = std::max (largest, current);
    }
    std::cout << std::max (smallest - (largest / 2), 0) << "\n";
    return 0;
}