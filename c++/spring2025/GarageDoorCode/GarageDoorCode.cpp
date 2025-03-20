/// \file GarageDoorCode.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/garagedoorcode

// Oh cool, it's a variant on the longest common subsequence problem!

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <set>

std::unordered_set<std::string>
findCommonSubsequences (const std::string& first, const std::string& second, std::size_t codeLength, std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>& memo)
{
    if (memo.contains (first) && memo[first].contains (second)) { return memo[first][second]; }
    std::unordered_set<std::string> results;
    if (!first.empty ()) {
        std::unordered_set<std::string> skipFirst = findCommonSubsequences (first.substr (1), second, codeLength, memo);
        results.insert (skipFirst.cbegin (), skipFirst.cend ());
    }
    if (!second.empty ()) {
        std::unordered_set<std::string> skipSecond = findCommonSubsequences (first, second.substr (1), codeLength, memo);
        results.insert (skipSecond.cbegin (), skipSecond.cend ());
    }
    if (!first.empty () && !second.empty () && first[0] == second[0]) {
        std::unordered_set<std::string> useBoth = findCommonSubsequences (first.substr (1), second.substr (1), codeLength, memo);
        for (const std::string& suffix : useBoth) {
            results.insert (first[0] + suffix);
        }
    }
    if (first.empty () && second.empty ()) {
        results.insert ("");
    }
    if (!memo.contains (first)) { memo[first] = {}; }
    memo[first][second] = results;
    return results;
}

std::unordered_set<std::string>
findCommonSubequences (const std::vector<std::string>& observations, std::size_t codeLength)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> memo;
    std::unordered_set<std::string> possible = findCommonSubsequences (observations[0], observations[1], codeLength, memo);
    for (auto iter = possible.begin (); iter != possible.end (); ) {
        if (iter->length () == codeLength) { ++iter; }
        else { iter = possible.erase (iter); }
    }
    for (int m = 0; m < observations.size (); ++m) {
        for (int n = m + 1; n < observations.size (); ++n) {
            memo.clear ();
            std::unordered_set<std::string> these = findCommonSubsequences (observations[m], observations[n], codeLength, memo);
            for (auto iter = possible.begin (); iter != possible.end (); ) {
                if (these.contains (*iter)) { ++iter; }
                else { iter = possible.erase (iter); }
            }
        }
    }    
    return possible;
}

int
main()
{
    std::size_t codeLength, numObservations;
    std::vector<std::string> observations;
    std::cin >> codeLength >> numObservations;
    while (observations.size () < numObservations) {
        std::string str;
        std::cin >> str;
        assert (str.size () >= codeLength);
        for (char c : str) { assert (isdigit (c)); }
        observations.push_back (str);
    }
    std::unordered_set<std::string> possibleCodes = findCommonSubequences (observations, codeLength);
    std::cout << possibleCodes.size () << "\n";
    std::set<std::string> sorted (possibleCodes.cbegin (), possibleCodes.cend ());
    for (const std::string& s : sorted) {
        std::cout << s << "\n";
    }
    return 0;
}