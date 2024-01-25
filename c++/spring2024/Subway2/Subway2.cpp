/// \file Subway2.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/subway2
/// Actually, it is not a solution.  It gets the correct answer on the sample input but
///   the wrong answer on some hidden input.  I have no idea why.
/// My strategy:
///   - A* search, in which we don't bother updating the cost of nodes in the frontier but
///     instead think of them as lazily deleted.
///   - Path compression, where instead of considering walking one meter at a time we only
///     ever walk between home, subway stations, and school.
/// My second approach (solve2) does not get the correct answer either.  Which of my
///   assumptions could be wrong?
/// Assumptions:
///   - There's no reason to ever stop anywhere other than a subway station, home, or school.
///   - There's no reason to ever walk between two stops on the same subway.
///   - My implementation of A* is correct.
///   - A* should make it so that the first time I visit a location is the least-cost way 
///     to get there.
///   - Manhattan distance at subway-speed cost is a consistent, admissible heuristic.

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

using Num = unsigned;
using Location = std::pair<Num, Num>;
using Subway = std::unordered_set<Location>;

const int MINUTES_PER_HOUR = 60;
const int METERS_PER_KILOMETER = 1000;
const double WALK_SPEED = 10 * METERS_PER_KILOMETER;
const double SUBWAY_SPEED = 40 * METERS_PER_KILOMETER;

namespace std {
    template<>
    struct hash<Location> {
        inline size_t
        operator() (const Location& l) const
        {
            return l.first + 10000 * l.second;
        }
    };
}

struct SearchNode
{
    Location loc;
    double f;
    double g;

    bool
    operator> (const SearchNode& o) const
    {
        return f + g > o.f + o.g;
    }
};

struct Problem
{
    Location home;
    Location school;
    std::vector<Subway> subways;
    std::unordered_set<Location> places;
};

double
subwayTime (const Location& start, const Location& stop)
{
    return (abs(start.first - stop.first) + abs(start.second - stop.second)) / SUBWAY_SPEED;
}

double
walkTime (const Location& start, const Location& stop)
{
    return (abs(start.first - stop.first) + abs(start.second - stop.second)) / WALK_SPEED;
}

std::vector<SearchNode>
successors (const Problem& prob, const SearchNode& curr)
{
    std::vector<SearchNode> result;
    // If we are already at a subway stop, we should consider riding the subway to each other stop.
    for (const Subway& subway : prob.subways) {
        if (subway.count(curr.loc) == 1) {
            for (const Location& other : subway) {
                if (other != curr.loc) {
                    result.push_back ({other, curr.f + subwayTime(curr.loc, other), subwayTime(other, prob.school)});
                }
            }
        }
    }

    // We could consider walking to the nearest stop on each subway (if we aren't already there).
    for (const Subway & subway : prob.subways) {
        Location closest = *subway.begin ();
        for (const Location& loc : subway) {
            if (walkTime (curr.loc, loc) < walkTime (curr.loc, closest)) {
                closest = loc;
            }
        }
        if (closest != curr.loc) {
            result.push_back ({closest, curr.f + walkTime(curr.loc, closest), subwayTime(closest, prob.school)});
        }
    }

    // I don't think this should be necessary, but we could consider walking to *every* subway stop.
    for (const Subway & subway : prob.subways) {
        for (const Location& loc : subway) {
            if (loc != curr.loc) {
                result.push_back ({loc, curr.f + walkTime(curr.loc, loc), subwayTime(loc, prob.school)});
            }
        }
    }

    // We could consider walking directly to the goal.
    result.push_back ({prob.school, curr.f + walkTime(curr.loc, prob.school), 0.0});
    return result;
}

double
solve (const Problem& prob)
{
    std::priority_queue<SearchNode, std::vector<SearchNode>, std::greater<SearchNode>> frontier;
    frontier.push ({prob.home, 0, subwayTime(prob.home, prob.school)});
    std::unordered_map<Location, double> fastestToHere;
    while (!frontier.empty ()) {
        SearchNode current = frontier.top ();
        frontier.pop ();
        //std::cout << "Considering (" << current.loc.first << ", " << current.loc.second << ") at a cost of " << current.f << " and a heuristic value of " << current.g << "\n";
        if (prob.school == current.loc) {
            return current.f;
        }
        // Don't bother if we had previously found a faster way to here.
        if (fastestToHere.count (current.loc) == 1 && current.f >= fastestToHere.at (current.loc)) {
            continue;
        }
        fastestToHere[current.loc] = current.f;
        for (const SearchNode& next: successors (prob, current)) {
            if (fastestToHere.count (next.loc) == 0 || fastestToHere.at (next.loc) > next.f) {
                frontier.push (next);
            }
        }
    }
    return INFINITY;
}

double
solve2 (const Problem& prob)
{
    std::priority_queue<SearchNode, std::vector<SearchNode>, std::greater<SearchNode>> frontier;
    frontier.push ({prob.home, 0, subwayTime(prob.home, prob.school)});
    std::unordered_set<Location> visited;
    while (!frontier.empty ()) {
        SearchNode current = frontier.top ();
        frontier.pop ();
        //std::cout << "Considering (" << current.loc.first << ", " << current.loc.second << ") at a cost of " << current.f << " and a heuristic value of " << current.g << "\n";
        visited.insert (current.loc);
        if (prob.school == current.loc) {
            return current.f;
        }
        for (const Location& place: prob.places) {
            if (place != current.loc && visited.count (place) == 0) {
                bool found = false;
                for (const Subway & sub : prob.subways) {
                    if (sub.count (place) == 1 && sub.count(current.loc) == 1) {
                        found = true;
                        frontier.push ({place, current.f + subwayTime (current.loc, place), subwayTime (place, prob.school)});
                        break;
                    }
                }
                if (!found) {
                    frontier.push ({place, current.f + walkTime (current.loc, place), subwayTime(place, prob.school)});
                }
            }
        }
    }
    return INFINITY;
}

Problem
readInput ()
{
    int homeX, homeY, schoolX, schoolY, x, y;
    std::vector<Subway> subways;
    Subway currentSubway;
    std::cin >> homeX >> homeY >> schoolX >> schoolY >> x >> y;
    while (std::cin) {
        if (x == -1 || y == -1) {
            subways.push_back (currentSubway);
            currentSubway.clear ();
        }
        else {
            currentSubway.insert ({x, y});
        }
        std::cin >> x >> y;
    }
    std::unordered_set<Location> places;
    places.insert ({homeX, homeY});
    places.insert ({schoolX, schoolY});
    for (const Subway& sub : subways) {
        for (const Location& loc : sub) {
            places.insert(loc);
        }
    }
    return {{homeX, homeY}, {schoolX, schoolY}, subways, places};
}

int main()
{
    Problem prob = readInput ();
    double hours = solve2(prob);
    std::cout << (int)round(hours * MINUTES_PER_HOUR) << "\n";
    return 0;
}