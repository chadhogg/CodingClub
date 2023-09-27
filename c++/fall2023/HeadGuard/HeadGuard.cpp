/// \file HeadGuard.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/headguard

#include <iostream>
#include <string>

int
main (int argc, char* argv[])
{
    std::string line;
    std::cin >> line;
    while (std::cin) {
        int count = 1;
        char previous = line[0];
        for (int i = 1; i < line.size (); ++i) {
            if (line[i] == previous) {
                ++count;
            }
            else {
                std::cout << count << previous;
                previous = line[i];
                count = 1;
            }
        }
        std::cout << count << previous << "\n";
        std::cin >> line;
    }
    return 0;
}