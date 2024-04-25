/// \file FourDieRolls.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/fourdierolls.


#include <iostream>
#include <cmath>

const int SIDES = 6;
const int MAX_ROLLS = 4;

int
main ()
{
    int numRolls;
    int rolls[MAX_ROLLS];
    std::cin >> numRolls;
    bool duplicate = false;
    for (int i = 0; i < numRolls; ++i) {
        std::cin >> rolls[i];
        for (int j = 0; j < i; ++j) {
            if (rolls[i] == rolls[j]) { duplicate = true; }
        }
    }
    
    int total = pow (SIDES, MAX_ROLLS - numRolls);

    if (duplicate) {
        // Brandon wins no matter what.
        std::cout << 0 << " " << total << "\n";
    }
    else {
        int ashley = (numRolls == 1 ? 5 * 4 * 3 : (numRolls == 2 ? 4 * 3 : 3));
        int brandon = total - ashley;
        std::cout << ashley << " " << brandon << "\n";
    }

    return 0;
}
