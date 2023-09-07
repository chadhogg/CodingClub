/// \file FizzBuzz.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/fizzbuzz

#include <iostream>

void
fizzbuzz (int fizzDiv, int buzzDiv, int last)
{
    for (int i = 1; i <= last; ++i)
    {
        if (i % fizzDiv == 0)
        {
            if (i % buzzDiv == 0)
            {
                std::cout << "FizzBuzz\n";
            }
            else
            {
                std::cout << "Fizz\n";
            }
        }
        else if (i % buzzDiv == 0)
        {
            std::cout << "Buzz\n";
        }
        else
        {
            std::cout << i << "\n";
        }
    }
}

int
main (int argc, char* argv[])
{
    int X, Y, N;
    std::cin >> X >> Y >> N;
    fizzbuzz (X, Y, N);
    return 0;
}