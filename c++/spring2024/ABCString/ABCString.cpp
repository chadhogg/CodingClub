/// \file ABCString.cpp
/// \author Chad Hogg
/// A first attempt at https://open.kattis.com/problems/abcstring, which is too slow (and probably exceeds recursion depth limits).


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

bool
canContinue (const std::string& str, const Partial& part)
{
    if (part.index == str.size ()) { return true; }
    bool foundASolution = false;
    for (unsigned int choice = 0; choice < part.num && !foundASolution; ++choice) {
        if (part.substrs.at (choice).count (str.at (part.index)) == 0) {
            Partial next({part.num, part.substrs, part.index + 1});
            next.substrs[choice].insert (str.at (part.index));
            if (next.substrs[choice].size () == 3) {
                next.substrs[choice].clear ();
            }
            foundASolution |= canContinue (str, next);
        }
    }
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
        if (canPartition (str, k)) {
            std::cout << k << "\n";
            break;
        }
    }
    return 0;
}
