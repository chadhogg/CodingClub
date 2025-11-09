/// \file AlphabetSoup.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/alphabetsoup

#include <iostream>
#include <set>
#include <string>
#include <cassert>


int
main()
{
    std::set<char> letters;
    std::string str;
    std::cin >> str;
    for (char c : str) {
        assert (isupper (c));
        letters.insert (c);
    }
    if (letters.size () == 26) {
        std::cout << "Alphabet Soup!\n";
    }
    else {
        for (char c = 'A'; c <= 'Z'; ++c) {
            if (letters.count (c) == 0) {
                std::cout << c;
            }
        }
        std::cout << "\n";
    }
    return 0;
}