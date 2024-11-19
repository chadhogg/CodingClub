/// \file WelcomeSign.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/welcomesign

#include <iostream>
#include <string>
#include <cassert>


int
main (int argc, char* argv[])
{
    int r, c, unbalanced;
    std::cin >> r >> c;
    unbalanced = 0;
    for (int count = 0; count < r; ++count) {
        std::string word;
        std::cin >> word;
        assert (word.length() <= c);
        while (word.length() < c) {
            if (word.length() + 2 <= c) {
                word = "." + word + ".";
            }
            else if(unbalanced % 2 == 0) {
                word = word + ".";
                ++unbalanced;
            }
            else {
                word = "." + word;
                ++unbalanced;
            }
        }
        std::cout << word << "\n";
    }
    return 0;
}