/// \file ChocolateChipFabrication.cpp
/// \author Chad Hogg
/// My attempt at a solution to https://open.kattis.com/problems/chocolatechipfabrication.
/// This works, but is too slow on big grids.


#include <iostream>
#include <vector>
#include <cassert>

const char PATTERN = 'X';
const char DOUGH = 'D';
const char CHIPS = 'C';
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
    //std::cin >> junk;
    //assert (junk == '\n');
    for (int i = 0; i < n; ++i) {
        std::vector<char> row;
        row.reserve (m);
        for (int j = 0; j < m; ++j) {
            std::cin >> c;
            assert (c == PATTERN || c == EMPTY);
            row.push_back (c);
        }
        pan.push_back (row);
        //std::cin >> junk;
        //assert (junk == '\n');
    }
    return pan;
}

bool
containsOnly (const Grid& grid, char c) {
    for (const std::vector<char>& row : grid) {
        for (const char& ch : row) {
            if (ch != c && ch != EMPTY) { return false; }
        }
    }
    return true;
}

// Let's hope that putting down dough as late as possible will always be optimal.
// It seems like it should be, because chipifying is only blocked by too much, never too little.
int
greedyBackwards(const Grid& grid)
{
    Grid current = grid;
    int generations = 0;
    while (!containsOnly (current, EMPTY)) {
        Grid next;
        for (size_t i = 0; i < grid.size (); ++i) {
            next.push_back ({});
            for (size_t j = 0; j < grid[i].size (); ++j) {
                if (current[i][j] == PATTERN) {
                    std::vector<std::pair<int, int>> neighbors;
                    neighbors.push_back ({i - 1, j});
                    neighbors.push_back ({i + 1, j});
                    neighbors.push_back ({i, j - 1});
                    neighbors.push_back ({i, j + 1});
                    bool atLeastOneEmpty = false;
                    for (std::pair<int, int>& n : neighbors) {
                        if (n.first < 0 || n.first >= grid.size () || n.second < 0 || n.second >= grid[i].size () || current[n.first][n.second] == EMPTY) {
                            atLeastOneEmpty = true;
                        }
                    }
                    if (atLeastOneEmpty) { next[i].push_back (EMPTY); }
                    else { next[i].push_back (PATTERN); }
                }
                else { next[i].push_back (EMPTY); }
            }
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
