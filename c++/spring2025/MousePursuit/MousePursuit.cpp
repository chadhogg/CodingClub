/// \file MousePursuit.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/mousepursuit

#include <iostream>
#include <cassert>
#include <map>
#include <string>

int
main()
{
    int numEvents;
    std::cin >> numEvents;
    std::map<int, int> cheese;
    std::map<int, int> glory;
    std::string str;
    int s, c, g;
    for (int i = 0; i < numEvents; ++i) {
        std::cin >> str >> s >> c >> g;
        assert (!cheese.contains (s) && !glory.contains (s));
        if (str == "CAUGHT") {
            cheese[s] = c;
            glory[s] = g;
        }
        else {
            assert (str == "MISS");
            cheese[s] = -c;
            glory[s] = -g;
        }
    }
    int k;
    std::cin >> k;
    assert (!cheese.contains (k) && !glory.contains (k));
    long totalC = 0, totalG = 0;
    for (auto iter = cheese.cbegin (); iter != cheese.cend (); ++iter) {
        if (iter->first < k) {
            totalC += iter->second;
            totalG += glory.at (iter->first);
        }
    }
    std::cout << totalC << " " << totalG << "\n";
    return 0;
}