/// \file Stafsetning.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/stafsetning

#include <iostream>

int
main()
{
    int numProblems, minutesPerFixedTypo, workMinutesPerDay;
    int temp;
    std::cin >> numProblems >> minutesPerFixedTypo >> workMinutesPerDay;
    long long totalTypos = 0;
    for (int i = 0; i < numProblems; ++i) {
        std::cin >> temp;
        totalTypos += temp;
    }
    if (minutesPerFixedTypo > workMinutesPerDay) {
        std::cout << ":(\n";
    }
    else {
        long long typosFixedPerDay = workMinutesPerDay / minutesPerFixedTypo;
        long long daysNeeded = totalTypos / typosFixedPerDay + (totalTypos % typosFixedPerDay == 0 ? 0 : 1);
        std::cout << daysNeeded << "\n";
    }
    return 0;
}