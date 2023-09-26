/// \file Fire2.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/fire2

#include <iostream>
#include <string>
#include <cassert>
#include <iomanip>
#include <vector>
#include <queue>

const char ROOM = '.';
const char WALL = '#';
const char ME = '@';
const char FIRE = '*';

const int IS_A_WALL = -1;
const int NOT_ON_FIRE_YET = -2;
const int BURNING_BEFORE_WE_ARRIVE = -3;
const int NOT_REACHED_YET = -4;
const int IMPOSSIBLE = -5;

// <row, column>
using Position = std::pair<int, int>;
using Map = std::vector<std::vector<char>>;
// Uses non-negative numbers to say when a cell (at earliest) can be reached, negatives otherwise.
using TimeMap = std::vector<std::vector<int>>;

std::vector<Position>
getValidNeighbors (Position current, int height, int width)
{
    std::vector<Position> neighbors;
    if (current.first > 0) { neighbors.push_back ({current.first - 1, current.second}); }
    if (current.first < height - 1) { neighbors.push_back ({current.first + 1, current.second}); }
    if (current.second > 0) { neighbors.push_back ({current.first, current.second - 1}); }
    if (current.second < width - 1) { neighbors.push_back ({current.first, current.second + 1}); }
    return neighbors;
}

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

TimeMap
floodFillMakeFireMap (const Map& map)
{
    std::queue<Position> unexpandedFire;
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
                unexpandedFire.push ({row, col});
            }
            // Some cells will (probably) catch fire in the future.
            else {
                fireRow.push_back (NOT_ON_FIRE_YET);
            }
        }
        fireMap.push_back (fireRow);
    }

    while (!unexpandedFire.empty ()) {
        Position pos = unexpandedFire.front ();
        unexpandedFire.pop ();
        int myValue = fireMap.at (pos.first).at (pos.second);
        assert (myValue >= 0 && map.at (pos.first).at (pos.second) != WALL);
        for (const Position & neighbor : getValidNeighbors (pos, map.size (), map.at (0).size ())) {
            // Each of my direct neighbors that is not already on fire / a wall becomes on fire in the next time step.
            int neighborValue = fireMap.at (neighbor.first).at (neighbor.second);
            if (neighborValue == NOT_ON_FIRE_YET) {
                fireMap[neighbor.first][neighbor.second] = myValue + 1;
                unexpandedFire.push (neighbor);
            }
            else if (neighborValue != IS_A_WALL) {
                assert (neighborValue == myValue || neighborValue == myValue - 1 || neighborValue == myValue + 1);
            }
        }
    }
    return fireMap;
}

int floodFillSolve (const Map& map) {
    const TimeMap fireMap = floodFillMakeFireMap (map);

    //printTimeMap (fireMap);

    std::queue<Position> unexpandedMe;
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
            // Exactly one cell is where we start.
            else if (map.at (row).at (col) == ME) {
                assert (unexpandedMe.empty ());
                fleeRow.push_back (0);
                unexpandedMe.push ({row, col});
                if (row == 0 || row == map.size () - 1 || col == 0 || col == map.at (0).size () - 1) {
                    return 1;
                }
            }
            // Everywhere else we don't yet know about.
            else {
                assert (map.at (row).at (col) == ROOM);
                fleeRow.push_back (NOT_REACHED_YET);
            }
        }
        fleeMap.push_back (fleeRow);
    }

    while (!unexpandedMe.empty ()) {
        Position current = unexpandedMe.front ();
        unexpandedMe.pop ();
        int myValue = fleeMap.at (current.first).at (current.second);
        assert (myValue >= 0 && map.at (current.first).at (current.second) != WALL && map.at (current.first).at (current.second) != FIRE);
        for (const Position & neighbor : getValidNeighbors (current, map.size (), map.at (0).size ())) {
            // Each of my direct neighbors that is not already visited / on fire / a wall becomes visited.
            int neighborValue = fleeMap.at (neighbor.first).at (neighbor.second);
            if (neighborValue == NOT_REACHED_YET) {
                // If it will already be on fire by this point, we can't reach it.
                if (fireMap.at (neighbor.first).at (neighbor.second) >= 0 && fireMap.at (neighbor.first).at (neighbor.second) <= myValue + 1) {
                    fleeMap[neighbor.first][neighbor.second] = BURNING_BEFORE_WE_ARRIVE;
                }
                // Otherwise, it is reachable in the next time step.
                else {
                    fleeMap[neighbor.first][neighbor.second] = myValue + 1;
                    unexpandedMe.push (neighbor);
                    if (neighbor.first == 0 || neighbor.first == map.size () - 1 || neighbor.second == 0 || neighbor.second == map.at (0).size () - 1) {
                        //printTimeMap (fleeMap);
                        return myValue + 2;
                    }
                }
            }
            else if (neighborValue != IS_A_WALL && neighborValue != BURNING_BEFORE_WE_ARRIVE) {
                assert (neighborValue == myValue || neighborValue == myValue - 1 || neighborValue == myValue + 1);
            }
        }
    }
    //printTimeMap (fleeMap);
    return IMPOSSIBLE;
}

void
validateMap (const Map& map) {
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

int
main (int argc, char* argv[])
{
    int numTestCases;
    std::cin >> numTestCases;
    while (numTestCases > 0) {
        int w, h;
        std::cin >> w >> h;
        Map map;
        while (h > 0) {
            std::string line;
            std::cin >> line;
            map.push_back (std::vector<char> (line.begin (), line.end()));
            --h;
        }
        validateMap (map);
        int result = floodFillSolve (map);
        if (result == IMPOSSIBLE) {
            std::cout << "IMPOSSIBLE\n";
        }
        else {
            std::cout << result << "\n";
        }
        --numTestCases;
    }
    return 0;
}