/// \file ABCString.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/abcstring.

// Strategy:
//   Use dynamic programming to try every possible assignment of chars to substrings, see if any work,
//     one character at a time from the beginning of the string.
//   If we reach a character that cannot be legally added to any substring, we can backtrack.
//   Once any substring has all three letters, we can clear it without changing the result.
//   If we reach the end of the string without backtracking, we have found a partition.
//   Caching results may be helpful.
//   There is no meaningful order among the substrings, so we can reduce the combinations to explore.

// New, much simpler idea:
//   If at any point the difference between the number of As and Bs is X, we need at least X substrings.
//   (And similarly for every other pair of letters.)
//   This idea is courtesy of John Hershey.

#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <set>
#include <string>
#include <map>

struct MyMultiSet
{
    std::map<std::set<char>, std::size_t> substrs;

    void
    removeSet (const std::set<char>& s)
    {
        --substrs[s];
        if (substrs.at (s) == 0) {
            substrs.erase (s);
        }
    }

    void
    addSet (const std::set<char>& s)
    {
        if (substrs.count (s) == 0) {
            substrs.insert ({s, 0});
        }
        ++substrs[s];
    }

};

std::ostream& operator<< (std::ostream& out, const MyMultiSet& mms)
{
    for (const std::pair<std::set<char>, std::size_t>& opt : mms.substrs) {
        out << "{";
        for (const char& c : opt.first) {
            out << c;
        }
        out << "}: " << opt.second << " ";
    }
    return out;
}


struct Partial
{
    unsigned int num;
    MyMultiSet substrs;
    unsigned int index;
};

template<>
struct std::hash<Partial>
{
    std::size_t operator() (const Partial& part) const
    {
        std::size_t result = part.index + part.num * 30000;
        for (const std::pair<std::set<char>, std::size_t>& s : part.substrs.substrs) {
            std::size_t x = 0;
            for (const char& c : s.first) {
                x += c;
            }
            result += x * s.second;
        }
        return result;
    }
};

bool operator== (const Partial& a, const Partial& b)
{
    return a.num == b.num && a.index == b.index && a.substrs.substrs == b.substrs.substrs;
}

std::unordered_map<Partial, bool> g_cache;
bool
canContinue (const std::string& str, const Partial& part)
{
    std::cout << "Trying " << part.substrs << " at index " << part.index << "\n";
    if (part.index == str.size ()) { return true; }
    if (g_cache.count (part) != 0) { return g_cache.at (part); }
    bool foundASolution = false;
    for (const std::pair<std::set<char>, std::size_t>& option : part.substrs.substrs) {
        // This is a type of string that exists in the partial solution that does not have this letter.
        if (option.first.count (str.at (part.index)) == 0) {
            Partial next ({part.num, part.substrs, part.index + 1});
            next.substrs.removeSet (option.first);
            if (option.first.size () == 2) {
                next.substrs.addSet ({});
            }
            else {
                std::set<char> s {option.first};
                s.insert (str.at (part.index));
                next.substrs.addSet (s);
            }
            if (canContinue (str, next)) { return true; }
        }
    }
    g_cache[part] = foundASolution;
    std::cout << "Rejecting " << part.substrs << " at index " << part.index << "\n";
    return foundASolution;
}

bool
canPartition (const std::string& str, unsigned int num)
{
    Partial part {num, {}, 0};
    for (unsigned int i = 0; i < num; ++i) {
        part.substrs.addSet ({});
    }
    return canContinue (str, part);
}

int
main ()
{
    std::string str;
    std::cin >> str;
    /*
    for (unsigned int k = 1; k < str.size () / 3; ++k) {
        g_cache.clear ();
        if (canPartition (str, k)) {
            std::cout << k << "\n";
            break;
        }
    }
     */
    int as = 0, bs = 0, cs = 0;
    int maxDiff = 0;
    for (unsigned int index = 0; index < str.size (); ++index) {
        if (str.at (index) == 'A') { ++as; }
        else if (str.at (index) == 'B') { ++bs; }
        else { ++cs; }
        maxDiff = std::max (maxDiff, std::abs (as - bs));
        maxDiff = std::max (maxDiff, std::abs (as - cs));
        maxDiff = std::max (maxDiff, std::abs (bs - cs));
    }
    std::cout << maxDiff << "\n";

    return 0;
}
