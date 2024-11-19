/// \file TrianglesOfASquare.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/trianglesofasquare

#include <iostream>
#include <string>
#include <cassert>

const int MIN = 0;
const int MAX = 2024;

int
main (int argc, char* argv[])
{
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;

    // If from one corner to another, 0 new triangles needed.
    if ((x1 == MIN || x1 == MAX) && (y1 == MIN || y1 == MAX) && (x2 == MIN || x2 == MAX) && (y2 == MIN || y2 == MAX)) {
        std::cout << "0\n";
    }
    // If from one corner to somewhere else, 1 new triangle needed.
    else if (((x1 == MIN || x1 == MAX) && (y1 == MIN || y1 == MAX)) || ((x2 == MIN || x2 == MAX) && (y2 == MIN || y2 == MAX))) {
        std::cout << "1\n";
    }
    // In all other cases, 2 new triangles will be needed.
    else {
        std::cout << "2\n";
    }
    return 0;
}