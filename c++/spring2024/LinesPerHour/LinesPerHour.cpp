/// \file LinesPerHour.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/linesperhour

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    const int HOURS = 5;
    int numCandidates, lph;
    std::cin >> numCandidates >> lph;
    std::vector<int> probDifficulties;
    for (int i = 0; i < numCandidates; ++i) {
        int x;
        std::cin >> x;
        probDifficulties.push_back (x);
    }
    std::sort (probDifficulties.begin (), probDifficulties.end ());
    int sum = 0;
    int count = 0;
    for (int index = 0; index < numCandidates; ++index) {
        if (sum + probDifficulties[index] <= lph * HOURS) {
            sum += probDifficulties[index];
            ++count;
        }
        else {
            break;
        }
    }
    std::cout << count << "\n";
    return 0;
}