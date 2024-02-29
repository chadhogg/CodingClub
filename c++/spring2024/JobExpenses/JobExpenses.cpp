/// \file JobExpenses.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/jobexpenses.


#include <iostream>

int
main ()
{
    int count, sum = 0, current;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        std::cin >> current;
        if (current < 0) { sum += current; }
    }
    std::cout << -sum << "\n";
    return 0;
}
