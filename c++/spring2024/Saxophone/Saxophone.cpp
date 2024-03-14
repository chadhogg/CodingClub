/// \file Saxophone.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/saxophone

#include <iostream>
#include <string>
#include <map>
#include <set>

int main()
{
    std::map<char, std::set<int>> mapping;
    mapping['c'] = {2, 3, 4, 7, 8, 9, 10};
    mapping['d'] = {2, 3, 4, 7, 8, 9};
    mapping['e'] = {2, 3, 4, 7, 8};
    mapping['f'] = {2, 3, 4, 7};
    mapping['g'] = {2, 3, 4};
    mapping['a'] = {2, 3};
    mapping['b'] = {2};
    mapping['C'] = {3};
    mapping['D'] = {1, 2, 3, 4, 7, 8, 9};
    mapping['E'] = {1, 2, 3, 4, 7, 8};
    mapping['F'] = {1, 2, 3, 4, 7};
    mapping['G'] = {1, 2, 3, 4};
    mapping['A'] = {1, 2, 3};
    mapping['B'] = {1, 2};

    int count;
    std::string text;
    std::cin >> count;
    std::getline (std::cin, text);
    for (int i = 0; i < count; ++i) {
        std::getline (std::cin, text);
        std::set<int> fingers = {};
        int counts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (char c : text) {
            std::set<int> note = mapping[c];
            for (int key = 1; key <= 10; ++key) {
                if (note.count (key) == 1 && fingers.count (key) == 0) {
                    ++counts[key - 1];
                }
            }
            fingers = note;
        }
        for (int key = 1; key <= 10; ++key) {
            std::cout << counts[key - 1] << (key == 10 ? "\n" : " ");
        }
    }
    return 0;
}