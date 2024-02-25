/// \file Champernowne.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/champernowne

#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <sstream>

int
main ()
{
    std::string input;
    std::stringstream stream;
    std::cin >> input;
    int x = 1;
    while (true) {
        stream.str ({});
        stream << x;
        std::string num = stream.str ();
        if (input == num) { std::cout << x << "\n"; break; }
        if (input.size () < num.size ()) { std::cout << -1 << "\n"; break; }
        if (input.substr (0, num.size ()) != num) { std::cout << -1 << "\n"; break; }
        input = input.substr (num.size ());
        ++x;
    }
    return 0;
}
