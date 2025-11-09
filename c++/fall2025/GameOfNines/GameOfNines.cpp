/// \file GameOfNines.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/gameofnines

#include <iostream>
#include <cassert>
#include <set>
#include <vector>

// Applying this to the original input did not make tooSlow() any faster.
void
greedilyReduce (std::vector<int>& counts) {
    //if (counts[0] > 1) { counts[0] = 1; }
    while (counts[1] > 1 && counts[8] > 1) { --counts[1]; --counts[8]; }
    while (counts[2] > 1 && counts[7] > 1) { --counts[2]; --counts[7]; }
    while (counts[3] > 1 && counts[6] > 1) { --counts[3]; --counts[6]; }
    while (counts[4] > 1 && counts[5] > 1) { --counts[4]; --counts[5]; }
}

// TLE on case 38
void
tooSlow ()
{
    int numDigits;
    std::cin >> numDigits;
    std::vector<int> digitCounts (9, 0);
    for (int i = 0; i < numDigits; ++i) {
        int x;
        std::cin >> x;
        ++digitCounts[x];
    }

    greedilyReduce (digitCounts);

    std::set<std::vector<int>> frontier;
    std::set<std::vector<int>> explored;
    frontier.insert (digitCounts);
    while (!frontier.empty ()) {
        std::vector<int> current = *frontier.begin ();
        frontier.erase (current);
        explored.insert (current);
        // Try adding two of the same number.
        for (int x = 0; x < 9; ++x) {
            if (current[x] >= 2 ) {
                std::vector<int> next = current;
                next[x] -= 1;
                int newDigit = (2 * x) % 10;
                if (newDigit != 9) { ++next[newDigit]; }
                if (explored.count (next) == 0) { frontier.insert (next); }
            }
        }
        // Try adding two different numbers.
        for (int x = 0; x < 9; ++x) {
            for (int y = x + 1; y < 9; y++) {
                if (current[x] >= 1 && current[y] >= 1) {
                    std::vector<int> next = current;
                    int newDigit = (x + y) % 10;
                    if (newDigit != 9) { ++next[newDigit]; }
                    next[x] -= 1;
                    if (explored.count (next) == 0) { frontier.insert (next); }
                    next[x] += 1;
                    next[y] -= 1;
                    if (explored.count (next) == 0) { frontier.insert (next); }
                }
            }
        }
    }

    int smallest = numDigits;
    for (const std::vector<int>& current : explored) {
        int count = current[0] + current[1] + current[2] + current[3] + current[4] + current[5] + current[6] + current[7] + current[8];
        smallest = std::min (smallest, count);
    }
    std::cout << smallest << "\n";
}


// Never worked; abandoned
void
superGreedy (std::vector<int>& counts) {
    bool reduced = true;
    while (reduced) {
        // If we can eliminate an 8 by adding 1, do it.
        if (counts[1] >= 1 && counts[8] >= 1) {
            --counts[8];
            continue;
        }
        // If we can eliminate a 7 by adding 2, do it.
        if (counts[2] >= 1 && counts[7] >= 1) {
            --counts[7];
            continue;
        }
        // If we can eliminate a 6 by adding 3, do it.
        if (counts[3] >= 1 && counts[6] >= 1) {
            --counts[6];
            continue;
        }
        // If we can eliminate a 5 by adding 4, do it.
        if (counts[4] >= 1 && counts[5] >= 1) {
            --counts[5];
            continue;
        }
        // If we can convert something to an 8 and still have a 1 left, do it.
        if (counts[1] >= 1) {
            if (counts[7] >= 1 && counts[1] >= 1) {
                --counts[7];
                ++counts[8];
                continue;
            }
            if (counts[6] >= 1 && counts[2] >= 1) {
                --counts[6];
                ++counts[8];
                continue;
            }
            if (counts[5] >= 1 && counts[3] >= 1) {
                --counts[5];
                ++counts[8];
                continue;
            }
            if (counts[4] >= 2) {
                --counts[4];
                ++counts[8];
                continue;
            }
        }
    }
}

// Inspired by John Hershey
void
logical ()
{
    int numDigits;
    std::cin >> numDigits;
    std::vector<int> digitCounts (9, 0);
    for (int i = 0; i < numDigits; ++i) {
        int x;
        std::cin >> x;
        ++digitCounts[x];
    }

    // If we have any ones at all, we can use one (as often as we want) to convert everything else to a nine.
    if (digitCounts[1] > 0) { std::cout << 1 << "\n"; }
    // If we have any threes at all, we can use one (as often as we want) to convert something else to a one or nine.
    // 2 + (3 * 3) = 11, 3 + (3 * 2) = 9, 4 + (3 * 5) = 19, 5 + (3 * 2) = 11, 6 + (3 * 1) = 9, 7 + (3 * 4) = 19, 8 + (3 * 1) = 11
    else if (digitCounts[3] > 0) { std::cout << 1 << "\n"; }
    // If we have any sevens at all, we can use one (as often as we want) to convert something else to a one or a nine.
    // 2 + (7 * 1) = 9, 3 + (7 * 4) = 31, 4 + (7 * 1) = 11, 5 + (7 * 2) = 19, 6 + (7 * 5) = 41, 7 + (7 * 2) = 21, 8 + (7 * 3) = 29
    else if (digitCounts[7] > 0) { std::cout << 1 << "\n"; }
    // If we have any fives at all and any even number, we can use the even number (as often as we want) to convert the five to a one, three, seven, or nine.
    // 5 + (2 * 1) = 7, 5 + (4 * 1) = 9, 5 + (6 * 1) = 11, 5 + (8 * 1) = 13
    else if (digitCounts[5] > 0 && (digitCounts[2] > 0 || digitCounts[4] > 0 || digitCounts[6] > 0 || digitCounts[8] > 0)) { std::cout << 1 << "\n"; }
    // In other cases, no matter what we do we will never reach a nine.  (Or one, three, five, or seven).
    else { std::cout << numDigits << "\n"; }
}


int
main()
{
    logical ();
    return 0;
}

