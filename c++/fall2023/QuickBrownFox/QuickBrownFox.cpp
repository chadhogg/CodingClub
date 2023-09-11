/// \file QuickBrownFox.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/quickbrownfox

#include <iostream>
#include <string>
#include <cassert>
#include <set>


int
main (int argc, char* argv[])
{
    int phraseCount;
    std::string junk;
    std::cin >> phraseCount;
    std::getline (std::cin, junk);
    for (int i = 0; i < phraseCount; ++i) {
        std::string phrase;
        std::getline (std::cin, phrase);
        std::set<char> appears;
        for (char c : phrase) {
            if (isalpha (c)) {
                appears.insert (tolower (c));
            }
        }
        if (appears.size () == 26) {
            std::cout << "pangram\n";
        }
        else {
            std::cout << "missing ";
            for (char c : "abcdefghijklmnopqrstuvwxyz") {
                if (appears.count (c) == 0) {
                    std::cout << c;
                }
            }
            std::cout << "\n";
        }
    }
    return 0;
}