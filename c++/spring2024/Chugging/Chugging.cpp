/// \file Chugging.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/chugging.


#include <iostream>

/// @brief Computes time needed to chug all bottles.
/// @param numBottles How many bottles to chug.
/// @param timePerBottle The time needed to chug one bottle.
/// @param slowdownPerBottle The increase in difficulty per bottle previously chugged.
/// @return The total time to chug all of the bottles.
/// This is computing the sum from bottle b=0 to bottle b=n-1 of t + b * d
int
totalTime(int numBottles, int timePerBottle, int slowdownPerBottle)
{
    return numBottles * timePerBottle + (numBottles - 1) * numBottles / 2 * slowdownPerBottle;
}

int
main ()
{
    int N, tA, dA, tB, dB;
    std::cin >> N >> tA >> dA >> tB >> dB;
    int alice = totalTime(N, tA, dA);
    int bob = totalTime(N, tB, dB);
    if (alice < bob) { std::cout << "Alice\n"; }
    else if (bob < alice) { std::cout << "Bob\n"; }
    else { std::cout << "=\n"; }
    return 0;
}
