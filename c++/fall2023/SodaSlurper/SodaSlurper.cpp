/// \file SodaSlurper.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/sodaslurper

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

int
main (int argc, char* argv[])
{
    int e, f, c;
    std::cin >> e >> f >> c;
    int bottles = e + f;
    int sodas = 0;
    while (bottles >= c) {
        //std::cout << sodas << " " << bottles << "\n";
        int newSodas = bottles / c;
        bottles = bottles % c + newSodas;
        sodas += newSodas;
    }
    std::cout << sodas << "\n";
    return 0;
}