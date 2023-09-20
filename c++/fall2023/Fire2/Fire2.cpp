/// \file Fire2.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/fire2

#include <iostream>
#include <string>
#include <cassert>
#include <iomanip>
#include <vector>

const char ROOM = '.';
const char WALL = '#';
const char ME = '@';
const char FIRE = '*';

const int IS_A_WALL = -1;
const int NOT_ON_FIRE_YET = -2;
const int BURNING_BEFORE_WE_ARRIVE = -3;
const int NOT_REACHED_YET = -4;

// <row, column>
using Position = std::pair<int, int>;
// <<row, column>, <seconds passed>
using State = std::pair<Position, int>;
// Uses non-negative numbers to say when a cell (at earliest) can be reached, negatives otherwise.
using TimeMap = std::vector<std::vector<int>>;

struct Puzzle
{
    std::vector<std::vector<char>> map;

    void
    validate () const
    {
        int numMes = 0;
        // Sizes should be within the bounds described in the problem.
        assert (map.size () >= 1 && map.size () <= 1000);
        assert (map.at (0).size () >= 1 && map.at (0).size () <= 1000);
        // Each row should be the same width.
        for (int row = 1; row < map.size (); ++row) {
            assert (map.at (0).size () == map.at (row).size ());
        }
        for (int row = 0; row < map.size (); ++row) {
            for (int col = 0; col < map.at (row).size (); ++col) {
                // Each cell should contain one of the allowable characters.
                assert (map.at (row).at (col) == ROOM
                        || map.at (row).at (col) == WALL
                        || map.at (row).at (col) == ME
                        || map.at (row).at (col) == FIRE);
                if (map.at (row).at (col) == ME) {
                    numMes++;
                }
            }
        }
        // There should be exactly one starting location.
        assert (numMes == 1);
    }

    // This map will tell us at what second fire reaches each reach-able space.
    TimeMap
    generateFireMap () const
    {
        TimeMap fireMap;
        fireMap.reserve (map.size ());
        // Set up a map showing how things are at time 0.
        for (int row = 0; row < map.size (); ++row) {
            std::vector<int> fireRow;
            fireRow.reserve (map.at (row).size ());
            for (int col = 0; col < map.at (row).size (); ++col) {
                // Some cells are walls and will never be burnable.
                if (map.at (row).at (col) == WALL) {
                    fireRow.push_back (IS_A_WALL);
                }
                // Some cells are already on fire.
                else if (map.at (row).at (col) == FIRE) {
                    fireRow.push_back (0);
                }
                // Some cells will (probably) catch fire in the future.
                else {
                    fireRow.push_back (NOT_ON_FIRE_YET);
                }
            }
            fireMap.push_back (fireRow);
        }

        bool changed = true;
        int timePassed = 0;
        // Iterate until a second passes with no additional cells catching fire.
        // Any that are still unburned at that point can never be burned.
        while (changed) {
            changed = false;
            for (int row = 0; row < map.size (); ++row) {
                for (int col = 0; col < map.at (row).size (); ++col) {
                    //  If a cell is not yet burning, it has the potential to catch fire this turn.
                    if (fireMap.at (row).at (col) == NOT_ON_FIRE_YET) {
                        std::vector<Position> neighbors;
                        if (row > 0) { neighbors.push_back ({row - 1, col}); }
                        if (row < map.size () - 1) { neighbors.push_back ({row + 1, col}); }
                        if (col > 0) { neighbors.push_back ({row, col - 1}); }
                        if (col < map.at (row).size () - 1) { neighbors.push_back ({row, col + 1}); }
                        for (const Position & neighbor : neighbors) {
                            // If a direct neighbor caught fire last time, I catch fire this time.
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

    // This map will tell us what second is the earliest at which the person could reach a cell
    //   (without walking through fire).
    TimeMap
    generateFleeMap (const TimeMap& fireMap) const
    {
        TimeMap fleeMap;
        fleeMap.reserve (map.size ());
        // Set up a map showing how things are at time 0.
        for (int row = 0; row < map.size (); ++row) {
            std::vector<int> fleeRow;
            fleeRow.reserve (map.at (row).size ());
            for (int col = 0; col < map.at (row).size (); ++col) {
                // Some cells are walls, and thus never enterable.
                if (map.at (row).at (col) == WALL) {
                    fleeRow.push_back (IS_A_WALL);
                }
                // Some cells are already on fire and thus not enterable.
                else if (map.at (row).at (col) == FIRE) {
                    fleeRow.push_back (BURNING_BEFORE_WE_ARRIVE);
                }
                // Some cells (only one if we passed validation) are where we start.
                else if (map.at (row).at (col) == ME) {
                    fleeRow.push_back (0);
                }
                // Everywhere else we don't yet know about.
                else {
                    fleeRow.push_back (NOT_REACHED_YET);
                }
            }
            fleeMap.push_back (fleeRow);
        }

        bool changed = true;
        int timePassed = 0;
        // Iterate until a second passes with no additional cells being reached.
        // Any cells we haven't reached at that point cannot be reached.
        while (changed) {
            changed = false;
            for (int row = 0; row < map.size (); ++row) {
                for (int col = 0; col < map.at (row).size (); ++col) {
                    // Cells about which we have no information are potentially interesting.
                    if (fleeMap.at (row).at (col) == NOT_REACHED_YET) {
                        // If it catches fire at or before now, we can't go there.
                        if (fireMap.at (row).at (col) >= 0 && fireMap.at (row).at (col) <= timePassed) {
                            fleeMap.at (row).at (col) = BURNING_BEFORE_WE_ARRIVE;
                        }
                        else {
                            std::vector<Position> neighbors;
                            if (row > 0) { neighbors.push_back ({row - 1, col}); }
                            if (row < map.size () - 1) { neighbors.push_back ({row + 1, col}); }
                            if (col > 0) { neighbors.push_back ({row, col - 1}); }
                            if (col < map.at (row).size () - 1) { neighbors.push_back ({row, col + 1}); }
                            for (const Position & neighbor : neighbors) {
                                // If we could reach one of its neighbors last turn, we can reach it this turn.
                                int value = fleeMap.at (neighbor.first).at (neighbor.second);
                                if (value == timePassed) {
                                    fleeMap[row][col] = timePassed + 1;
                                    changed = true;
                                    // Minor optimization: the first exit we find should be the best, so don't look for others.
                                    if (row == 0 || row == map.size () - 1 || col == 0 || col == map.at (row).size () - 1) {
                                        return fleeMap;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            ++timePassed;
        }
        return fleeMap;
    }
};

void
printTimeMap(const TimeMap& timeMap) {
    for (int row = 0; row < timeMap.size (); ++row) {
        for (int col = 0; col < timeMap.at (row).size (); ++col) {
            if (timeMap.at (row).at (col) == IS_A_WALL) {
                std::cout << std::setw (4) << WALL;
            }
            else if (timeMap.at (row).at (col) == BURNING_BEFORE_WE_ARRIVE) {
                std::cout << std::setw (4) << FIRE;
            }
            else if (timeMap.at (row).at (col) == NOT_ON_FIRE_YET || timeMap.at (row).at (col) == NOT_REACHED_YET) {
                std::cout << std::setw (4) << ROOM;
            }
            else {
                std::cout << std::setw (4) << timeMap.at (row).at (col);
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void
solve (const Puzzle& puzzle) {
    TimeMap fireMap = puzzle.generateFireMap ();
    TimeMap fleeMap = puzzle.generateFleeMap (fireMap);
    //printTimeMap (fireMap);
    //printTimeMap (fleeMap);

    // Search for cells on the border that are reachable, choose the first reachable one.
    int best = INT16_MAX;
    for (int row = 0; row < puzzle.map.size (); ++row) {
        for (int col = 0; col < puzzle.map.at (row).size (); ++col) {
            if (row == 0 || row == puzzle.map.size () - 1 || col == 0 || col == puzzle.map.at (row).size () - 1) {
                if (fleeMap.at (row).at (col) >= 0) {
                    best = std::min (best, fleeMap.at (row).at (col));
                }
            }
        }
    }

    if (best == INT16_MAX) {
        std::cout << "IMPOSSIBLE\n";
    }
    else {
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
        while (h > 0) {
            std::string line;
            std::cin >> line;
            puzzle.map.push_back (std::vector<char> (line.begin (), line.end()));
            --h;
        }
        //puzzle.validate ();
        solve (puzzle);
        --numTestCases;
    }
    return 0;
}