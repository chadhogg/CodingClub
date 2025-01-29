/// \file ChampernowneSubsequence.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/champernownesubsequence

#include <iostream>
#include <cassert>

int
main()
{
    int n;
    std::string pattern;
    std::cin >> n >> pattern;
    assert (pattern.size () == n);
    for (char c : pattern) { assert (isdigit (c)); }
    std::string alreadyMatched = "";
    std::string remaining = pattern;
    int k = 1;
    while (!remaining.empty ()) {
        std::string integer = std::to_string (k);
        for (char c : integer) {
            if (!remaining.empty () && c == remaining[0]) {
                alreadyMatched.push_back (c);
                remaining = remaining.substr (1);
            }
        }
        ++k;
    }
    std::cout << (k - 1) << "\n";
    return 0;
}