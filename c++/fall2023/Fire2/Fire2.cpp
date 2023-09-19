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
#include <map>
#include <list>
#include <set>

const char ROOM = '.';
const char WALL = '#';
const char ME = '@';
const char FIRE = '*';

const int CANT_BE_ON_FIRE = -1;
const int NOT_ON_FIRE_YET = -2;

// <row, column>
using Position = std::pair<int, int>;
// <<row, column>, <seconds passed>
using State = std::pair<Position, int>;
using FireMap = std::vector<std::vector<int>>;

struct Puzzle
{
    std::vector<std::vector<char>> map;
    Position me;

    void
    validate () const
    {
        assert (map.size () >= 1 && map.size () <= 1000);
        assert (map.at (0).size () >= 1 && map.at (0).size () <= 1000);
        assert (me.first >= 0 && me.first < map.size ());
        assert (me.second >= 0 && me.second < map.at (0).size ());
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
                    assert (row == me.first && col == me.second);
                }
            }
        }
    }

    bool
    isSolved () const
    {
        assert (map.at (me.first).at (me.second) == ME);
        return me.first == 0 || me.first == map.size () - 1 || me.second == 0 || me.second == map.at (0).size () - 1;
    }

    FireMap
    generateFireMap () const
    {
        FireMap fireMap;
        fireMap.reserve (map.size ());
        for (int row = 0; row < map.size (); ++row) {
            std::vector<int> fireRow;
            fireRow.reserve (map.at (row).size ());
            for (int col = 0; col < map.at (row).size (); ++col) {
                if (map.at (row).at (col) == WALL) {
                    fireRow.push_back (CANT_BE_ON_FIRE);
                }
                else if (map.at (row).at (col) == FIRE) {
                    fireRow.push_back (0);
                }
                else {
                    fireRow.push_back (NOT_ON_FIRE_YET);
                }
            }
            fireMap.push_back (fireRow);
        }

        bool changed = true;
        int timePassed = 0;
        while (changed) {
            changed = false;
            for (int row = 0; row < map.size (); ++row) {
                for (int col = 0; col < map.at (row).size (); ++col) {
                    if (fireMap.at (row).at (col) == NOT_ON_FIRE_YET) {
                        std::vector<Position> neighbors;
                        if (row > 0) { neighbors.push_back ({row - 1, col}); }
                        if (row < map.size () - 1) { neighbors.push_back ({row + 1, col}); }
                        if (col > 0) { neighbors.push_back ({row, col - 1}); }
                        if (col < map.at (row).size () - 1) { neighbors.push_back ({row, col + 1}); }
                        for (const Position & neighbor : neighbors) {
                            int value = fireMap.at (neighbor.first).at (neighbor.second);
                            if (value == timePassed) {
                                fireMap[row][col] = timePassed + 1;
                                changed = true;
                            }
                        }
                    }
                }
            }
            ++timePassed;
        }
        return fireMap;
    }
};

void
printFireMap(const FireMap& fireMap) {
    for (int row = 0; row < fireMap.size (); ++row) {
        for (int col = 0; col < fireMap.at (row).size (); ++col) {
            std::cout << std::setw(4) << fireMap.at (row).at (col);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void
solve (const Puzzle& puzzle) {
    FireMap fireMap = puzzle.generateFireMap ();
    //printFireMap (fireMap);

    std::map<State, std::set<Position>> solutions;
    std::list<std::pair<State, std::set<Position>>> frontier;
    frontier.push_back ({{puzzle.me, 0}, {}});

    while (!frontier.empty ()) {
        State current = frontier.front ().first;
        std::set<Position> previous = frontier.front ().second;
        frontier.pop_front ();
        //std::cout << "(" << current.first.first << ", " << current.first.second << "): " << current.second << "\n";

        if (current.first.first == 0 || current.first.first == puzzle.map.size () - 1 || current.first.second == 0 || current.first.second == puzzle.map.at (0).size () - 1) {
            // We made it out!
            if (solutions.count (current) == 0 || solutions.at (current).size () > previous.size ()) {
                solutions[current] = previous;
            }
        }
        else {
            std::vector<Position> neighbors;
            if (current.first.first > 0) { neighbors.push_back ({current.first.first - 1, current.first.second}); }
            if (current.first.first < puzzle.map.size () - 1) { neighbors.push_back ({current.first.first + 1, current.first.second}); }
            if (current.first.second > 0) { neighbors.push_back ({current.first.first, current.first.second - 1}); }
            if (current.first.second < puzzle.map.at (0).size () - 1) { neighbors.push_back ({current.first.first, current.first.second + 1}); }
            // We could move to any adjacent square
            for (const Position& pos : neighbors) {
                // ... as long as it isn't a wall
                if (puzzle.map.at (pos.first).at (pos.second) != WALL) {
                    // ... and it isn't already on fire
                    if (fireMap.at (pos.first).at (pos.second) > current.second) {
                        // ... and (for efficiency) we haven't already been there
                        if (previous.count (pos) == 0) {
                            // Then we can try it!
                            frontier.push_back ({{pos, current.second + 1}, {previous}});
                            frontier.back ().second.insert (current.first);
                        }
                    }
                }
            }
        }
    }
    if (solutions.size () == 0) {
        std::cout << "IMPOSSIBLE\n";
    }
    else {
        int best = INT16_MAX;
        for (const std::pair<State, std::set<Position>>& solution : solutions) {
            if (solution.first.second < best) { best = solution.first.second; }
        }
        std::cout << best + 1 << "\n";
    }
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
        puzzle.me.first = -1;
        puzzle.me.second = -1;
        while (h > 0) {
            std::string line;
            std::cin >> line;
            assert (line.length () == w);
            for (char c : line) {
                assert (c == ROOM || c == WALL || c == ME || c == FIRE);
                if (c == ME) {
                    puzzle.me.first = puzzle.map.size ();
                    puzzle.me.second = line.find (ME);
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