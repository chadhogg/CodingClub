/// \file Sok.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/sok

#include <iostream>
#include <iomanip>
#include <cassert>


int
main()
{
    int boughtA, boughtB, boughtC;
    int neededA, neededB, neededC;
    std::cin >> boughtA >> boughtB >> boughtC >> neededA >> neededB >> neededC;
    int neededTotal = neededA + neededB + neededC;
    double ratioA = boughtA / (double)neededA;
    double ratioB = boughtB / (double)neededB;
    double ratioC = boughtC / (double)neededC;
    double unitsMade = std::min (ratioA, std::min (ratioB, ratioC));
    double remainingA = boughtA - (neededA * unitsMade);
    double remainingB = boughtB - (neededB * unitsMade);
    double remainingC = boughtC - (neededC * unitsMade);
    std::cout << std::setprecision (8) << remainingA << " " 
              << std::setprecision (8) << remainingB << " " 
              << std::setprecision (8) << remainingC << "\n";
    return 0;
}