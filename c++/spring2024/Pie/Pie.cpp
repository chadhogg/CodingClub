/// \file Pie.cpp
/// \author Chad Hogg
/// My attempt at a solution to https://open.kattis.com/problems/pie
/// NOTE: This passes the provided test cases but fails the hidden ones.

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

// Strategy: start by assigning everybody to the largest pie.
// While it is not harmful to do so, take someone from the most congested pie and move them to the least congested pie.

double
getSize (int numPeople, int radius)
{
    return radius * radius * M_PI / numPeople;
}

double solve (int numPeople, const std::vector<int>& pies)
{
    std::vector<int> people (pies.size (), 0);
    people[0] = numPeople;
    double currentSize = getSize (numPeople, pies.at (0));
    if (pies.size () == 1) { return currentSize; }
    bool improved = true;
    while (improved) {
        int mostCongested = 0, leastCongested = 0;
        double mostSize, leastSize;
        mostSize = leastSize = getSize (people.at (0), pies.at (0));
        for (int i = 1; i < pies.size (); ++i) {
            if (people.at (i) == 0) {
                if (leastSize != INFINITY) {
                    leastSize = INFINITY;
                    leastCongested = i;
                }
            }
            else {
                double thisSize = getSize (people.at (i), pies.at (i));
                if (thisSize < mostSize) {
                    mostCongested = i;
                    mostSize = thisSize;
                }
                if (thisSize > leastSize) {
                    leastCongested = i;
                    leastSize = thisSize;
                }
            }
        }

        double newSize = std::min (getSize (people.at (mostCongested) - 1, pies.at (mostCongested)), getSize (people.at (leastCongested) + 1, pies.at (leastCongested)));
        if (newSize > currentSize) {
            --people[mostCongested];
            ++people[leastCongested];
            currentSize = newSize;
        }
        else {
            improved = false;
        }
    }
    return currentSize;
}

int main()
{
    int numCases;
    std::cin >> numCases;
    for (int i = 0; i < numCases; ++i) {
        int numPies, numPeople;
        std::cin >> numPies;
        std::cin >> numPeople;
        ++numPeople;
        std::vector<int> radii;
        radii.reserve (numPies);
        int radius;
        for (int j = 0; j < numPies; ++j) {
            std::cin >> radius;
            radii.push_back (radius);
        }
        std::sort (radii.begin (), radii.end (), std::greater<int>());
        std::cout << std::fixed << std::setprecision (9) << solve (numPeople, radii) << "\n";
    }
    return 0;
}