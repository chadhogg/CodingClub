/// \file PhotoEncoding.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/photocoding

#include <iostream>
#include <cassert>
#include <map>


//  0  1  2  3  4  5  6
//  1  2  3  4  5  6  7
//  2  3  4  5  6  7  8
//  3  4  5  6  7  8  9
//  4  5  6  7  8  9 10
//  5  6  7  8  9 10 11
//  6  7  8  9 10 11 12

//  0s: 1 at 1x1
//  1s: 2 at 2x2
//  2s: 1 at 2x2, 2 at 3x3
//  3s: 2 at 3x3, 2 at 4x4
//  4s: 1 at 3x3, 2 at 4x4, 2 at 5x5
//  5s: 2 at 4x4, 2 at 5x5, 2 at 6x6
//  6s: 1 at 4x4, 2 at 5x5, 2 at 6x6, 2 at 7x7

// If distance is odd, we can put 2 at size d/2 + 2 and 2 at each greater size up to d+1
// If distance is even, we can put 1 at size d/2 + 1 and 2 at each greater size up to d+1

int
minWidth (int distance, int count) {
    int size = distance / 2 + 1;
    if (distance % 2 == 0) { count -= 1; }
    else { count -= 2; size += 1; }
    while (count > 0) {
        ++size;
        count -= 2;
    }
    return size;
}

int
main()
{
    int numBlacks;
    std::cin >> numBlacks;
    int currentDistance;
    std::map<unsigned int, unsigned int> distanceCounts;
    for (int i = 0; i < numBlacks; ++i) {
        std::cin >> currentDistance;
        if (distanceCounts.count (currentDistance) == 0) { distanceCounts[currentDistance] = 0; }
        ++distanceCounts[currentDistance];
    }
    int minSize = 0;
    for (auto it = distanceCounts.begin (); it != distanceCounts.end (); ++it) {
        minSize = std::max (minSize, minWidth (it->first, it->second));
    }
    std::cout << minSize << "\n";
    return 0;
}

