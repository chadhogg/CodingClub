/// \file AboveSeaLevel.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/abovesealevel

#include <iostream>
#include <cassert>

int
main()
{
    double level;
    std::cin >> level;
    std::cout << (level - 0.3) << "\n";
    return 0;
}