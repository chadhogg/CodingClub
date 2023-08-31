/// \file MixedFractions.cpp
/// \author Chad Hogg
/// \brief Solution to https://open.kattis.com/problems/mixedfractions

#include <iostream>

struct mixed
{
    int whole;
    int numerator;
    int denominator;
};

mixed
simplify (int num, int den)
{
    return {num / den, num % den, den};
}

int
main (int argc, char* argv[])
{
    int n, d;
    std::cin >> n >> d;
    while (d != 0)
    {
        mixed answer = simplify(n, d);
        std::cout << answer.whole << " " << answer.numerator << " / " << answer.denominator << "\n";
        std::cin >> n >> d;
    }
    return 0;
}