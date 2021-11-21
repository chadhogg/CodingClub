/// \file Generator.cpp
/// \author Chad Hogg
/// \brief A program that generates problems for https://open.kattis.com/problems/sequences

#include <cstdlib>
#include <iostream>

int main (int argc, char* argv[]) {
    unsigned int length = rand () % 500000 + 1;
    if (argc == 2) {
        length = atoi (argv[1]);
    }
    for(unsigned int i {0U}; i < length; ++i) {
        int result = rand () % 3;
        if (result == 2) {
            std::cout << '?';
        }
        else {
            std::cout << result;
        }
    }
    std::cout << "\n";
    return 0;
}