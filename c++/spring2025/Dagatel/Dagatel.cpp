/// \file Dagatel.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/dagatel

#include <iostream>
#include <cassert>

const int JANUARY = 1;
const int FEBRUARY = 2;
const int MARCH = 3;
const int APRIL = 4;
const int MAY = 5;
const int JUNE = 6;
const int JULY = 7;
const int AUGUST = 8;
const int SEPTEMBER = 9;
const int OCTOBER = 10;
const int NOVEMBER = 11;
const int DECEMBER = 12;

int
main()
{
    int month;
    std::cin >> month;
    assert (month >= 1 && month <= 12);
    // 30 days has September, April, June, and November
    switch (month) {
        case SEPTEMBER:
        case APRIL:
        case JUNE:
        case NOVEMBER:
        std::cout << 30 << "\n"; break;
        case FEBRUARY:
        std::cout << 28 << "\n"; break;
        default:
        std::cout << 31 << "\n"; break;
    }
    return 0;
}