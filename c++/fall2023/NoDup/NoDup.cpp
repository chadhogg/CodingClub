/// \file NoDup.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/nodup

#include <iostream>
#include <string>
#include <unordered_set>

int
main (int argc, char* argv[])
{
    std::unordered_set<std::string> words;
    std::string word;
    std::cin >> word;
    while (std::cin) {
        //std::cout << "\"" << word << "\"\n";
        if (words.count (word) != 0) {
            std::cout << "no\n";
            exit (0);
        }
        words.insert (word);
        std::cin >> word;
    }
    std::cout << "yes\n";
    return 0;
}