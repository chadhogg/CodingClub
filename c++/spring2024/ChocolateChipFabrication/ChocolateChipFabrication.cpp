/// \file ChocolateChipFabrication.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/chocolatechipfabrication.


#include <iostream>
#include <vector>
#include <cassert>

const char PATTERN = 'X';
const char EMPTY = '-';

using Grid = std::vector<std::vector<char>>;

Grid
readInput ()
{
    int n, m;
    char c, junk;
    std::cin >> n >> m;
    Grid pan;
    pan.reserve (n);
    for (int i = 0; i < n; ++i) {
        std::vector<char> row;
        row.reserve (m);
        for (int j = 0; j < m; ++j) {
            std::cin >> c;
            assert (c == PATTERN || c == EMPTY);
            row.push_back (c);
        }
        pan.push_back (row);
    }
    return pan;
}

bool
containsOnly (const Grid& grid, char c) {
    for (const std::vector<char>& row : grid) {
        for (const char& ch : row) {
            if (ch != c) { return false; }
        }
    }
    return true;
}

// Let's hope that putting down dough as late as possible will always be optimal.
// It seems like it should be, because chipifying is only blocked by too much, never too little.
//
// New idea to speed it up -- for each step backwards, strip the outer border off.
// In fact, strip off entirely empty rows even if they aren't on the outer border.
int
greedyBackwards(const Grid& grid)
{
    Grid current = grid;
    int generations = 0;
    while (!containsOnly (current, EMPTY)) {
        Grid next;
        for (size_t i = 1; i < current.size () - 1; ++i) {
            std::vector<char> row;
            bool nonEmpty = false;
            for (size_t j = 1; j < current[i].size () - 1; ++j) {
                if (current[i][j] == PATTERN && current[i + 1][j] == PATTERN && current[i - 1][j] == PATTERN && current[i][j - 1] == PATTERN && current[i][j + 1] == PATTERN) {
                    row.push_back (PATTERN);
                    nonEmpty = true;
                }
                else { row.push_back (EMPTY); }
            }
            if (nonEmpty) { next.push_back (row); }
        }
        current = next;
        ++generations;
    }
    return generations;
}

int
main ()
{
    Grid grid = readInput ();
    std::cout << greedyBackwards (grid) << "\n";
    return 0;
}
