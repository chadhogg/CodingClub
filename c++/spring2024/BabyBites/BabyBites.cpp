/// \file BabyBites.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/babybites

#include <iostream>
#include <string>

int main()
{
    int bites;
    std::cin >> bites;
    int counter = 1;
    bool good = true;
    for (int i = 0; i < bites && good; ++i) {
        std::string input;
        std::cin >> input;
        if (input != "mumble") {
            if (counter != atoi (input.c_str ())) {
                good = false;
            }
        }
        ++counter;
    }
    if (good) {
        std::cout << "makes sense\n";
    }
    else {
        std::cout << "something is fishy\n";
    }
    return 0;
}