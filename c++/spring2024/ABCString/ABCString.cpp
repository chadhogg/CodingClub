/// \file ABCString.cpp
/// \author Chad Hogg
/// A first attempt at https://open.kattis.com/problems/abcstring, which is too slow (and probably exceeds recursion depth limits).

// Strategy:
//   Use dynamic programming to try every possible assignment of chars to substrings, see if any work,
//     one character at a time from the beginning of the string.
//   If we reach a character that cannot be legally added to any substring, we can backtrack.
//   Once any substring has all three letters, we can empty it out.
//   If we reach the end of the string without backtracking, we have found a partition.
//   Caching results may be helpful.
//   If multiple substrings are in the same state, there's no reason to try applying the next char to more than one of them.

#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <set>
#include <string>


struct Partial
{
    unsigned int num;
    std::vector<std::set<char>> substrs;
    unsigned int index;
};

template<>
struct std::hash<Partial>
{
    std::size_t operator() (const Partial& part) const
    {
        std::size_t result = part.index + part.num * 30000;
        for (unsigned int sub = 0; sub < part.num; ++sub) {
            std::size_t x = 0;
            for (const char& c : part.substrs.at (sub)) {
                x += c;
            }
            result += x * (sub + 1);
        }
        return result;
    }
};

bool operator== (const Partial& a, const Partial& b)
{
    return a.num == b.num && a.index == b.index && a.substrs == b.substrs;
}

bool
hasAnEarlierDuplicate (const std::vector<std::set<char>>& substrs, int choice)
{
    for (unsigned int other = 0; other < choice; ++other) {
        if (substrs.at (other) == substrs.at (choice)) { return true; }
    }
    return false;
}

std::unordered_map<Partial, bool> g_cache;
bool
canContinue (const std::string& str, const Partial& part)
{
    if (part.index == str.size ()) { return true; }
    if (g_cache.count (part) != 0) { return g_cache.at (part); }
    bool foundASolution = false;
    for (unsigned int choice = 0; choice < part.num && !foundASolution; ++choice) {
        if (part.substrs.at (choice).count (str.at (part.index)) == 0 && !hasAnEarlierDuplicate (part.substrs, choice)) {
            Partial next({part.num, part.substrs, part.index + 1});
            next.substrs[choice].insert (str.at (part.index));
            if (next.substrs[choice].size () == 3) {
                next.substrs[choice].clear ();
            }
            foundASolution |= canContinue (str, next);
        }
    }
    g_cache[part] = foundASolution;
    return foundASolution;
}

bool
canPartition (const std::string& str, unsigned int num)
{
    return canContinue (str, {num, {num, std::set<char>()}, 0});
}

int
main ()
{
    std::string str;
    std::cin >> str;
    for (unsigned int k = 1; k < str.size () / 3; ++k) {
        g_cache.clear ();
        if (canPartition (str, k)) {
            std::cout << k << "\n";
            break;
        }
    }
    return 0;
}
