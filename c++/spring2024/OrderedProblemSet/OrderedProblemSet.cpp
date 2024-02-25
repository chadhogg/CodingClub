/// \file OrderedProblemSet.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/orderedproblemset

#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <sstream>
#include <set>

int
main ()
{
    std::vector<int> problems;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        problems.push_back (x);
    }

    bool some = false;
    for (int k = 2; k <= problems.size (); ++k) {
        if (n % k == 0) {
            bool good = true;
            std::set<int> previous;
            std::set<int> current;
            int index = 0;
            for (int part = 0; part < k && good; ++part) {
                for (int index = 0; index < n / k && good; ++index) {
                    int x = problems[index + part * n / k];
                    for (const int& p : previous) { if (x < p) { good = false; } }
                    current.insert (x);
                }
                previous = current;
                current.clear ();
            }
            if (good) {
                std::cout << k << "\n";
                some = true;
            }
        }
    }
    if (!some) {
        std::cout << "-1\n";
    }
    return 0;
}
