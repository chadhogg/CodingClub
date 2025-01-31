/// \file Autori.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/autori

#include <iostream>
#include <string>


int
main()
{
    std::string input;
    std::cin >> input;
    for (char c : input) {
        if (isupper (c)) { std::cout << c; }
    }
    std::cout << "\n";
    return 0;
}