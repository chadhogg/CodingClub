/// \file JibJob.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/jibjob

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

struct Crane {
    int x;
    int y;
    int h;
};

std::vector<Crane>
readInput ()
{
    int n, x, y, h;
    std::cin >> n;
    std::vector<Crane> cranes;
    cranes.reserve (n);
    for (int i = 0; i < n; ++i) {
        std::cin >> x >> y >> h;
        cranes.push_back ({x, y, h});
    }
    return cranes;
}

int
distance (const Crane& c1, const Crane& c2)
{
    int xDist = c1.x - c2.x;
    int yDist = c1.y - c2.y;
    double dist = sqrt (xDist * xDist + yDist * yDist);
    return dist;
    //if (dist == floor (dist)) { return dist - 1; }
    //else { return dist; }
}

std::vector<int>
solve (const std::vector<Crane>& cranes)
{
    std::vector<int> jibs;
    for (std::size_t i = 0; i < cranes.size (); ++i) {
        int length = cranes.at (i).h;
        for (std::size_t j = 0; j < cranes.size (); ++j) {
            if (cranes.at (j).h > cranes.at (i).h) {
                length = std::min (length, distance (cranes.at (i), cranes.at (j)));
            }
        }
        jibs.push_back (length);
    }
    return jibs;
}

void
printOutput (const std::vector<int>& jibs)
{
    for (int i : jibs) {
        std::cout << i << "\n";
    }
}

int
main()
{
    std::vector<Crane> cranes = readInput ();
    std::vector<int> jibs = solve (cranes);
    printOutput (jibs);
    return 0;
}