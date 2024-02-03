/// \file ICPCTeamGeneration.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/icpcteamgeneration
/// It is not fast enough for hidden test case #3.
/// I had originally tried just using maxTeams() with no partitioning, and that was too slow.
/// I added partitionAndSolve() to the pipeline because it seemed like a good way to take
///   advantage of the "if i < j, ai <= aj and bi <= bj" stipulation, but it hasn't helped.

#include <iostream>
#include <vector>
#include <cassert>
#include <map>

using Preference = std::pair<int, int>;
using Preferences = std::vector<Preference>;


/// @brief Calculates the maximum number of teams that can be created from a partially-solved problem.
///
/// @param comp The vector of competitor preferences.
/// @param going A vector telling us which competitors are already on teams.
/// @param next The index of the competitor to try to put on a team with people behind him
///   in the list.
/// @return The maximum number of teams that can be created, given the decisions already
///   encoded into going and next.
///
/// If there are not at least 3 competitors left, no more teams can be made. (Our base case.)
/// If the "next" competitor is already on a team (with at least one competitor above him in
///   the list), we simply move on.
/// Otherwise, the best way to extend the decisions already made will be to either not make a
///   team with the next competitor or to make one of the teams that can possibly work with
///   them and two lower competitors.  Consider all possibilities and choose the best.
int
maxTeams (const Preferences& comp, const std::vector<bool>& going, int next)
{
    // If this competitor is already on a team, move on to the next one that isn't.
    while (next < comp.size () - 2 && going[next]) {
        ++next;
    }

    // Memoization -- might not be enough overlap to make it worthwhile, but it doesn't
    //   seem to hurt.
    static std::map<std::pair<std::vector<bool>, int>, int> cache;
    if (cache.count ({going, next}) > 0) { return cache[{going, next}]; }

    // Base case: if we have considered every competitor, just count how many teams already
    //   exist.
    if (next >= comp.size () - 2) {
        int count = 0;
        for (int i = 0; i < comp.size (); ++i) {
            if (going[i]) { ++count; }
        }
        assert (count % 3 == 0);
        cache[{going, next}] = count / 3;
        return count / 3;
    }
    // Choose maximum of either not including this person on any teams or of 
    //   every team we could make with them and lower-ranked competitors.
    else {
        // Best option that doesn't include this competitor.
        int max = maxTeams (comp, going, next + 1);
        assert (!going[next]);
        // Consider each lower competitor who isn't already on a team and is compatible with
        //   the "next" competitor as a second team member.
        for (int second = next + 1; second < comp.size () - 1; ++second) {
            if (!going[second]
                && second >= comp[next].first && second <= comp[next].second
                && next >= comp[second].first && next <= comp[second].second) {
                // Consider each lower competitor who isn't already on a team and is compatible
                //   with both the "next" and "second" team members as a third team member.
                for (int third = second + 1; third < comp.size (); ++third) {
                    if (!going[third]
                        && third >= comp[next].first && third <= comp[next].second
                        && next >= comp[third].first && next <= comp[third].second
                        && third >= comp[second].first && third <= comp[second].second
                        && second >= comp[third].first && second <= comp[third].second) {
                        std::vector<bool> used (going);
                        used[next] = used[second] = used[third] = true;
                        // See if we can make more total teams using this team than what we
                        //   have tried so far.
                        max = std::max (max, maxTeams (comp, used, next + 1));
                    }
                }
            }
        }
        // Update cache.
        cache[{going, next}] = max;
        return max;
    }
}

/// @brief Partitions the problem into independent subproblems if possible, then solves them.
/// @param comp The vector of competitors.
/// @return The maximum number of teams that can be made from those competitors.
int
partitionAndSolve (const Preferences& comp)
{
    // Look for potential partition points and take the first one that works.
    for (int i = 0; i < comp.size (); ++i) {
        // Competitor i will not work with anyone above him on the list, which means that no
        //   one below him will work with them either.  We can safely partition above him.
        if (comp[i].first > i) {
            Preferences compA (comp.begin (), comp.begin () + i);
            Preferences compB (comp.begin () + i, comp.end ());
            for (int j = 0; j < compB.size (); ++j) {
                compB[j].first -= i;
                compB[j].second -= i;
            }
            return partitionAndSolve (compA) + partitionAndSolve (compB);
        }
        // Competitor i will not work with anyone below him on the list, which means that no
        //   one above him will work with them either.  We can safely partition below him.
        else if (comp[i].second < i) {
            Preferences compA (comp.begin (), comp.begin () + 1 + i);
            Preferences compB (comp.begin () + i + 1, comp.end ());
            for (int j = 0; j < compB.size (); ++j) {
                compB[j].first -= i + 1;
                compB[j].second -= i + 1;
            }
            return partitionAndSolve (compA) + partitionAndSolve (compB);
        }
    }
    // No partition points were found, so just solve the whole problem.
    std::vector<bool> going (comp.size (), false);
    return maxTeams (comp, going, 0);
}

int
main ()
{
    int n, a, b;
    std::cin >> n;
    Preferences competitors;
    for (int i = 0; i < n; ++i) {
        std::cin >> a >> b;
        // Subtracting here because their concept of team numbers is 1-based, not 0-based.
        competitors.emplace_back (a - 1, b - 1);
    }
    std::cout << partitionAndSolve (competitors) << "\n";
    return 0;
}
