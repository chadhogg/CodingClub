/// \file CircularPainting.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/circularpainting

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

double
sectorArea (int angle, int radius)
{
    return angle / 360.0 * M_PI * radius * radius;
}

int
main()
{
    double totalArea = 0;
    int numSectors, angle, r1, r2;
    std::cin >> numSectors;
    for (int i = 0; i < numSectors; ++i) {
        std::cin >> angle >> r1 >> r2;
        totalArea += sectorArea (angle, r2) - sectorArea (angle, r1);
    }
    std::cout << std::setprecision (8) << totalArea << "\n";
    return 0;
}