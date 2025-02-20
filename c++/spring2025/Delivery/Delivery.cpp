/// \file Delivery.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/delivery

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <numeric>

struct Location
{
    int where;
    int howMany;
};

bool
operator< (const Location& l1, const Location& l2)
{
    return l1.where < l2.where;
}

bool
operator== (const Location& l1, const Location& l2)
{
    return l1.where == l2.where;
}

struct Problem
{
    int capacity;
    std::vector<Location> locations;
};

Problem
readInput ()
{
    Problem prob;
    int x, y, count;
    std::cin >> count >> prob.capacity;
    assert (3 <= count && count <= 1000);
    assert (1 <= prob.capacity && prob.capacity <= 10000);
    prob.locations.reserve (count);
    for (int i = 0; i < count; ++i) {
        std::cin >> x >> y;
        assert (-1500 <= x && x <= 1500 && x != 0);
        assert (1 <= y && y <= 800);
        prob.locations.push_back ({x, y});
    }
    return prob;
}

long
dealWith (const Problem& prob, std::vector<Location>& locs)
{
    long distance = 0;
    int remainingPackages = std::accumulate (locs.begin (), locs.end (), 0, 
        [](int sum, const Location& l) { return sum + l.howMany; }
    );
    while (remainingPackages > 0) {
        int onTruck = std::min (remainingPackages, prob.capacity);
        remainingPackages -= onTruck;
        // Drive to furthest place
        int oldWhere = locs.back ().where;
        distance += std::abs (oldWhere);

        while (onTruck > 0) {
            if (onTruck >= locs.back ().howMany) {
                onTruck -= locs.back ().howMany;
                locs.pop_back ();
            }
            else {
                locs.back ().howMany -= onTruck;
                onTruck = 0;
            }
            if (onTruck > 0) {
                // Drive to the next furthest place.
                int newWhere = locs.back ().where;
                assert (newWhere != oldWhere);
                distance += std::abs (newWhere - oldWhere);
                oldWhere = newWhere;
            }
        }
        // Drive back to post office.
        distance += std::abs (oldWhere);
    }
    return distance;
}

// Assumption: taking as many letters as possible and delivering them as far as possible should always be optimal.
long
solve (const Problem& prob)
{
    std::vector<Location> negatives, positives;
    for (const Location& loc : prob.locations) {
        if (loc.where < 0) { negatives.push_back (loc); }
        else { positives.push_back (loc);}
    }
    std::sort (negatives.begin (), negatives.end ());
    std::reverse (negatives.begin (), negatives.end ());
    std::sort (positives.begin (), positives.end ());
    return dealWith (prob, negatives) + dealWith (prob, positives);
}

int
main()
{
    Problem prob = readInput ();
    std::cout << solve (prob) << "\n";
    return 0;
}