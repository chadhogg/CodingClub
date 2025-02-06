/// \file Sibice.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/sibice

#include <iostream>
#include <cassert>
#include <cmath>


int
main()
{
    int numMatches, width, height, temp;
    std::cin >> numMatches >> width >> height;
    double longest = sqrt(width * width + height * height);
    for (int i = 0; i < numMatches; ++i) {
        std::cin >> temp;
        if (temp <= longest) { std::cout << "DA\n"; }
        else { std::cout << "NE\n"; }
    }
    return 0;
}