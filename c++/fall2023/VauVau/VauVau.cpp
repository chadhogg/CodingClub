/// \file NoDup.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/vauvau

#include <iostream>

int
main (int argc, char* argv[])
{
    int A, B, C, D, P, M, G;
    std::cin >> A >> B >> C >> D >> P >> M >> G;
    for (int time : {P, M, G}) {
        int afterDog1Cycles = time % (A + B);
        int afterDog2Cycles = time % (C + D);
        bool dog1Attacks = (afterDog1Cycles <= A && afterDog1Cycles != 0);
        bool dog2Attacks = (afterDog2Cycles <= C && afterDog2Cycles != 0);
        if (dog1Attacks && dog2Attacks) {
            std::cout << "both\n";
        }
        else if (dog1Attacks || dog2Attacks) {
            std::cout << "one\n";
        }
        else {
            std::cout << "none\n";
        }
    }
    return 0;
}