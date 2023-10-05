/// \file Speeding.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/speeding

#include <iostream>


int
main (int argc, char* argv[])
{
    int numPhotos;
    std::cin >> numPhotos;
    int tPrev, dPrev, tCurr, dCurr;
    std::cin >> tCurr >> dCurr;
    int maxSpeed = -1;
    for (int i = 1; i < numPhotos; ++i) {
        tPrev = tCurr;
        dPrev = dCurr;
        std::cin >> tCurr >> dCurr;
        int speed = (dCurr - dPrev) / (tCurr - tPrev);
        maxSpeed = std::max (maxSpeed, speed);
    }
    std::cout << maxSpeed << "\n";
    return 0;
}