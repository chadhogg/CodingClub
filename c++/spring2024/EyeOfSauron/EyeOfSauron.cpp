/// \file EyeOfSauron.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/eyeofsauron

#include <iostream>
#include <string>

int main()
{
    std::string text;
    std::cin >> text;
    std::size_t paren = text.find ('(');
    if (paren == text.size () - paren - 2) {
        std::cout << "correct\n";
    }
    else {
        std::cout << "fix\n";
    }
    return 0;
}