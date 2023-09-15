/// \file Pizza2.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/pizza2

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <iomanip>

double
area (double radius)
{
    return M_PI * radius * radius;
}

int
main (int argc, char* argv[])
{
    double C, R;
    std::cin >> R >> C;
    std::cout << std::fixed << std::setprecision(6) << area(R - C) / area(R) * 100 << "\n"; 
    return 0;
}