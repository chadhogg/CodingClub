/// \file ContestAdvancement.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/contestadvancement

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

int main()
{
    int n, k, c;
    std::cin >> n >> k >> c;
    std::vector<std::pair<int, int>> teams;
    std::vector<bool> going (n, false);
    for (int i = 0; i < n; ++i) {
        int t, s;
        std::cin >> t >> s;
        teams.emplace_back (t, s);
    }
    std::map<int, int> schoolCounts;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (count < k) {
            if (schoolCounts.count (teams[i].second) == 0) { schoolCounts[teams[i].second] = 0; }
            if (schoolCounts[teams[i].second] < c) {
                ++schoolCounts[teams[i].second];
                ++count;
                going[i] = true;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        if (count < k) {
            if (going[i] == false) {
                going[i] = true;
                ++count;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (going[i]) {
            std::cout << teams[i].first << "\n";
        }
    }
    return 0;
}