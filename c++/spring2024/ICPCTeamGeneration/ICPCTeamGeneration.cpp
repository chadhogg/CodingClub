/// \file ICPCTeamGeneration.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/icpcteamgeneration

// Most of this code is from an approach that was too slow, described here:
//   It is not fast enough for hidden test case #3.
//   I had originally tried just using maxTeams() with no partitioning, and that was too slow.
//   I added partitionAndSolve() to the pipeline because it seemed like a good way to take
//     advantage of the "if i < j, ai <= aj and bi <= bj" stipulation, but it hasn't helped.
//   I was originally using a map as my memoization cache, changed to unordered_map at 
//     suggestion of William Killian.
//   I was originally using a vector<bool> as my record of who is on a team; changed to 
//     bit-packed int at suggestion of William Killian.
// The final code is just the much simpler greedy algorithm at the bottom.
//   The insights there came from Marshall Feng.
// I'm disappointed that the fancy way wasn't fast enough.

#define NDEBUG

#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <set>

using Preference = std::pair<int, int>;
using Preferences = std::vector<Preference>;
using Going = uint64_t;
using State = std::pair<Going, int>;
using Cache = std::unordered_map<State, int>;

template<>
struct std::hash<State>
{
    std::size_t operator() (const State& state) const
    {
        return state.first + state.second;
    }
};

inline bool
isBitOn (const Going& bools, int index)
{
    return bools & (1 << index);
}

inline void
turnBitOn (Going& bools, int index)
{
    bools |= (1 << index);
}

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
maxTeams (const Preferences& comp, const Going& going, int next, Cache& cache)
{
    // If this competitor is already on a team, move on to the next one that isn't.
    while (next < comp.size () - 2 && isBitOn (going, next)) {
        ++next;
    }

    // Memoization -- might not be enough overlap to make it worthwhile, but it doesn't
    //   seem to hurt.
    if (cache.count ({going, next}) > 0) { return cache[{going, next}]; }

    // Base case: not enough unconsidered people to make any more teams, so just count how 
    //   many teams already exist.
    if (next >= comp.size () - 2) {
        int count = 0;
        for (int i = 0; i < comp.size (); ++i) {
            if (isBitOn (going, i)) { ++count; }
        }
        assert (count % 3 == 0);
        cache[{going, next}] = count / 3;
        return count / 3;
    }
    // Choose maximum of either not including this person on any teams or of 
    //   every team we could make with them and lower-ranked competitors.
    else {
        // Best option that doesn't include this competitor.
        int max = maxTeams (comp, going, next + 1, cache);
        assert (!going[next]);
        // Consider each lower competitor who isn't already on a team and is compatible with
        //   the "next" competitor as a second team member.
        for (int second = next + 1; second < comp.size () - 1; ++second) {
            if (!isBitOn (going, second)
                && second >= comp[next].first && second <= comp[next].second
                && next >= comp[second].first && next <= comp[second].second) {
                // Consider each lower competitor who isn't already on a team and is compatible
                //   with both the "next" and "second" team members as a third team member.
                for (int third = second + 1; third < comp.size (); ++third) {
                    if (!isBitOn (going, third)
                        && third >= comp[next].first && third <= comp[next].second
                        && next >= comp[third].first && next <= comp[third].second
                        && third >= comp[second].first && third <= comp[second].second
                        && second >= comp[third].first && second <= comp[third].second) {
                        Going used (going);
                        turnBitOn (used, next);
                        turnBitOn (used, second);
                        turnBitOn (used, third);
                        // See if we can make more total teams using this team than what we
                        //   have tried so far.
                        max = std::max (max, maxTeams (comp, used, next + 1, cache));
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
    Going going = 0;
    Cache cache;
    return maxTeams (comp, going, 0, cache);
}

bool
compatible (const Preferences& comp, int a, int b)
{
    return b >= comp.at (a).first && b <= comp.at (a).second && a >= comp.at (b).first && a <= comp.at (b).second;
}

// Observations:
//   Suppose that A and B are competitors, with A ranked higher than B.
//   If A considers B too low-ranked, A also considers everyone below B too low-ranked. (obvious)
//   If B considers A too high-ranked, B also considers everyone above A too high-ranked. (obvious)
//   If A considers B too low-ranked, everyone above A also considers B too low-ranked.
//   If B considers A too high-ranked, everyone below B also considers A too high-ranked.
// Conclusion:
//   Just going down the list and matching everyone with the highest pair that can work with them should
//     be enough.
int
greedy (const Preferences& comp)
{
    std::set<int> onATeam;
    for (int a = 0; a < comp.size (); ++a) {
        if (onATeam.count (a) == 0) {
            for (int b = a + 1; b < comp.size () && onATeam.count (a) == 0; ++b) {
                if (onATeam.count (b) == 0 && compatible (comp, a, b)) {
                    for (int c = b + 1; c < comp.size () && onATeam.count(b) == 0; ++c) {
                        if (onATeam.count (c) == 0 && compatible (comp, a, c) && compatible (comp, b, c)) {
                            onATeam.insert (a);
                            onATeam.insert (b);
                            onATeam.insert (c);
                        }
                    }
                }
            }
        }
    }
    return onATeam.size () / 3;
}

int
main ()
{
    int n, a, b;
    std::cin >> n;
    Preferences competitors;
    for (int i = 0; i < n; ++i) {
        std::cin >> a >> b;
        // Subtracting here because their concept of competitor numbers is 1-based, not 0-based.
        competitors.emplace_back (a - 1, b - 1);
    }
    //std::cout << partitionAndSolve (competitors) << "\n";
    std::cout << greedy (competitors) << "\n";
    return 0;
}
