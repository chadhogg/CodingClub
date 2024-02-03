/// \file ICPCTeamGeneration.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/icpcteamgeneration
/// It is not fast enough.  I can think of optimizations that would end loops early, but
///   none of them would affect the number of recursive calls made so I don't think they
///   would make enough of a difference to matter.
/// The key to optimizing it must be "if i < j, ai <= aj and bi <= bj", because I'm not
///   using that fact in any way right now.

#include <iostream>
#include <vector>
#include <cassert>
#include <map>

int
maxTeams (const std::vector<std::pair<int, int>> comp, const std::vector<bool>& going, int next)
{
    static std::map<std::pair<std::vector<bool>, int>, int> cache;
    if (cache.count ({going, next}) > 0) { return cache[{going, next}]; }

    // If we have considered every competitor, just count how many are going.
    if (next == comp.size () - 2) {
        int count = 0;
        for (int i = 0; i < comp.size (); ++i) {
            if (going[i]) { ++count; }
        }
        assert (count % 3 == 0);
        cache[{going, next}] = count / 3;
        return count / 3;
    }
    // Choose maximum of either not including this person on any (additional) teams or of 
    //   every team we could make with them and lower-ranked competitors.
    else {
        int max = maxTeams (comp, going, next + 1);
        if (!going[next]) {
            for (int second = next + 1; second < comp.size () - 1; ++second) {
                if (!going[second]
                    && second >= comp[next].first && second <= comp[next].second
                    && next >= comp[second].first && next <= comp[second].second) {
                    for (int third = second + 1; third < comp.size (); ++third) {
                        if (!going[third]
                            && third >= comp[next].first && third <= comp[next].second
                            && next >= comp[third].first && next <= comp[third].second
                            && third >= comp[second].first && third <= comp[second].second
                            && second >= comp[third].first && second <= comp[third].second) {
                            std::vector<bool> used (going);
                            used[next] = used[second] = used[third] = true;
                            max = std::max (max, maxTeams (comp, used, next + 1));
                        }
                    }
                }
            }
        }
        cache[{going, next}] = max;
        return max;
    }
}

int
main ()
{
    int n, a, b;
    std::cin >> n;
    std::vector<std::pair<int, int>> competitors;
    for (int i = 0; i < n; ++i) {
        std::cin >> a >> b;
        competitors.emplace_back (a - 1, b - 1);
    }
    std::vector<bool> going (n, false);
    std::cout << maxTeams (competitors, going, 0) << "\n";
    return 0;
}
