/// \file HerbMixing.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/herbmixing

#include <iostream>
#include <string>
#include <cassert>


int
main (int argc, char* argv[])
{
    int g, r;
    std::cin >> g >> r;
    int boost = 0;
    int oneEach = std::min (g, r);
    boost += 10 * oneEach;
    g -= oneEach;
    boost += 10 * (g / 3);
    g = g % 3;
    boost += 3 * (g / 2);
    g = g % 2;
    boost += 1 * g;
    std::cout << boost << "\n";
    return 0;
}