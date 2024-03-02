/// \file CCFGen.cpp
/// \author Chad Hogg
/// A generator for ChocolateChipFabrication problems.


#include <iostream>
#include <cstdlib>

const char PATTERN = 'X';
const char EMPTY = '-';


int
main ()
{
    srand (time (NULL));
    int n, m, p;
    std::cin >> n >> m >> p;
    std::cout << n << " " << m << "\n";
    for( int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (rand() % 100 < p) { std::cout << PATTERN; }
            else { std::cout << EMPTY; }
        }
        std::cout << "\n";
    }
    return 0;
}
