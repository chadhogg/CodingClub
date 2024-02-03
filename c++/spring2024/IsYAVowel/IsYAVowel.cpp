/// \file IsYAVowel.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/isyavowel

#include <iostream>
#include <string>

int main()
{
    std::string text;
    std::cin >> text;
    int vowels = 0, ys = 0;
    for (char c : text) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') { ++vowels; }
        if (c == 'y') { ++ys; }
    }
    std::cout << vowels << " " << vowels + ys << "\n";
    return 0;
}