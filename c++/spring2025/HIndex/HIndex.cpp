/// \file HIndex.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/hindex

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>


// O(N^2) algorithm is too slow
void
attempt1(long numPapers)
{
    std::vector<long> citationNumbers;
    citationNumbers.reserve (numPapers);
    long x;
    for (long i = 0; i < numPapers; ++i) {
        std::cin >> x;
        citationNumbers.push_back (x);
    }

    for (long i = 1; i <= numPapers + 1; ++i) {
        long count = 0;
        for (long x : citationNumbers) {
            if (x >= i) { ++count; }
        }
        if (count < i) {
            std::cout << i - 1 << "\n";
            break;
        }
    }
}

// Works on all of the sample inputs, but apparently not on a hidden one.
// Not sure why, because the algorithm seems sound to me.
void
attempt2(long numPapers) {
    std::map<long, long> citationNumberCounts;
    long x;
    for (long i = 0; i < numPapers; ++i) {
        std::cin >> x;
        if (citationNumberCounts.count (x) == 0) { citationNumberCounts[x] = 1; }
        else { ++citationNumberCounts[x]; }
    }
    long numWithFewerCitations = 0;
    long lastThatHadEnough = 0;
    for (long i = 0; i <= numPapers; ++i) {
        if (citationNumberCounts.count (i) != 0) {
            if (numPapers - numWithFewerCitations >= i) {
                lastThatHadEnough = i;
            }
            else {
                break;
            }
            numWithFewerCitations += citationNumberCounts[i];
        }
    }
    std::cout << lastThatHadEnough << "\n";
}

void
attempt3 (long numPapers)
{
    std::vector<long> citationNumbers;
    citationNumbers.reserve (numPapers);
    long x;
    for (long i = 0; i < numPapers; ++i) {
        std::cin >> x;
        citationNumbers.push_back (x);
    }
    std::sort (citationNumbers.begin (), citationNumbers.end (), std::greater<>());
    long hIndex = 0;
    while (citationNumbers[hIndex] >= hIndex + 1) {
        ++hIndex;
    }
    std::cout << hIndex << "\n";
}

int
main()
{
    long numPapers;
    std::cin >> numPapers;
    attempt3 (numPapers);
    return 0;
}