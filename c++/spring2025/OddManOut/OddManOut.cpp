/// \file OddManOut.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/oddmanout

#include <iostream>
#include <map>
#include <cassert>

void
doCase (int num)
{
    int numGuests;
    std::cin >> numGuests;
    long current;
    std::map<long, int> counts;
    for (int i = 0; i < numGuests; ++i) {
        std::cin >> current;
        if (counts.contains (current)) {
            assert (counts.at (current) == 1);
            counts.erase (current);
        }
        else {
            counts[current] = 1;
        }
    }
    assert (counts.size () == 1);
    std::cout << "Case #" << num << ": " << counts.begin ()->first << "\n";
}

int
main()
{
    int numCases;
    std::cin >> numCases;
    for (int i = 0; i < numCases; ++i) { doCase (i + 1); }

    return 0;
}