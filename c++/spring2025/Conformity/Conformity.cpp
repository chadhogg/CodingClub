/// \file Conformity.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/conformity

#include <iostream>
#include <set>
#include <map>
#include <cassert>


const std::size_t NUM_COURSES = 5;

int
main()
{
    std::size_t numStudents;
    std::cin >> numStudents;
    std::map<std::set<int>, int> comboCounts;
    for (std::size_t i = 0 ; i < numStudents; ++i) {
        std::set<int> combo;
        for (std::size_t j = 0; j < NUM_COURSES; ++j) {
            int x;
            std::cin >> x;
            combo.insert (x);
        }
        assert (combo.size () == 5);
        if (comboCounts.count (combo) == 0) {
            comboCounts[combo] = 0;
        }
        ++comboCounts[combo];
    }

    int maxPopularity = 0;
    int numAtMaxPopularity = 0;
    for (auto iter = comboCounts.begin (); iter != comboCounts.end (); ++iter) {
        if (iter->second > maxPopularity) { maxPopularity = iter->second; numAtMaxPopularity = iter->second; }
        else if (iter->second == maxPopularity) { numAtMaxPopularity += iter->second; }
    }
    std::cout << numAtMaxPopularity << "\n";
    return 0;
}