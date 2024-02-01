/// \file Homework.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/homework

#include <iostream>
#include <string>
#include <unordered_set>

std::pair<unsigned, unsigned>
extractRange(std::string& str)
{
    size_t semi = str.find (';');
    std::string prefix = str.substr (0, semi);
    if (semi == std::string::npos) {
        str = "";
    }
    else {
        str = str.substr (semi + 1);
    }
    size_t dash = prefix.find('-');
    if (dash == std::string::npos) {
        return {stoi (prefix), stoi (prefix)};
    }
    else {
        return {stoi(prefix.substr(0, dash)), stoi(prefix.substr(dash + 1))};
    }
}

int main()
{
    std::string input;
    std::cin >> input;
    std::unordered_set<int> problems;
    while (!input.empty ()) {
        std::pair<unsigned, unsigned> range = extractRange(input);
        for (unsigned x = range.first; x <= range.second; ++x) {
            problems.insert (x);
        }
    }
    std::cout << problems.size () << "\n";
    return 0;
}