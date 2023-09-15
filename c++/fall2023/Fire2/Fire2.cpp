/// \file Fire2.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/fire2

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

const char ROOM = '.';
const char WALL = '#';
const char ME = '@';
const char FIRE = '*';

struct Puzzle
{
    std::vector<std::vector<char>> map;
    int meRow;
    int meCol;
    int secondsPassed;

    void
    validate () const
    {
        assert (map.size () >= 1 && map.size () <= 1000);
        assert (map.at (0).size () >= 1 && map.at (0).size () <= 1000);
        assert (meRow >= 0 && meRow < map.size ());
        assert (meCol >= 0 && meCol < map.at (0).size ());
        for (int row = 1; row < map.size (); ++row) {
            assert (map.at (0).size () == map.at (row).size ());
        }
        for (int row = 0; row < map.size (); ++row) {
            for (int col = 0; col < map.at (row).size (); ++col) {
                assert (map.at (row).at (col) == ROOM
                        || map.at (row).at (col) == WALL
                        || map.at (row).at (col) == ME
                        || map.at (row).at (col) == FIRE);
                if (map.at (row).at (col) == ME) {
                    assert (row == meRow && col == meCol);
                }
            }
        }
    }

    bool
    isSolved () const
    {
        assert (map.at (meRow).at (meCol) == ME);
        return meRow == 0 || meRow == map.size () - 1 || meCol == 0 || meCol == map.at (0).size () - 1;
    }
};

void
solve (const Puzzle& puzzle) {
    std::cout << "TODO\n";
}

int
main (int argc, char* argv[])
{
    int numTestCases;
    std::cin >> numTestCases;
    while (numTestCases > 0) {
        int w, h;
        std::cin >> w >> h;
        Puzzle puzzle;
        puzzle.meRow = -1;
        puzzle.meCol = -1;
        puzzle.secondsPassed = 0;
        while (h > 0) {
            std::string line;
            std::cin >> line;
            assert (line.length () == w);
            for (char c : line) {
                assert (c == ROOM || c == WALL || c == ME || c == FIRE);
                if (c == ME) {
                    puzzle.meRow = puzzle.map.size ();
                    puzzle.meCol = line.find (ME);
                }
            }
            puzzle.map.push_back (std::vector<char> (line.begin (), line.end()));
            --h;
        }
        puzzle.validate ();
        solve (puzzle);
        --numTestCases;
    }
    return 0;
}