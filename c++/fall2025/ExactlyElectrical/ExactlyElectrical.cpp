/// \file ExactlyElectrical.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/exactlyelectrical

#include <iostream>
#include <cassert>


int
main()
{
    int startX, startY, endX, endY, charge;
    std::cin >> startX >> startY >> endX >> endY >> charge;
    int distance = std::abs (startX - endX) + std::abs (startY - endY);
    // You can return to a location in 2 moves (u, u) or 4 moves (r, r, r, r)
    if (charge == distance) {
        std::cout << "Y\n";
    }
    else if(charge > distance && (charge - distance) % 2 == 0) {
        std::cout << "Y\n";
    }
    else {
        std::cout << "N\n";
    }
    return 0;
}