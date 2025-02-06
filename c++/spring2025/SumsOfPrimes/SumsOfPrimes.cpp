/// \file SumsOfPrimes.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/sumsofprimes

#include <iostream>
#include <vector>

int
main()
{
    long n, total;
    std::cin >> n;
    total = 0L;
    std::vector<bool> isPrime (n, true);
    for (int i = 2; i < n; ++i) {
        if (isPrime.at (i)) {
            total += i;
            for (int k = i + i; k < n; k += i) {
                isPrime[k] = false;
            }
        }
    }
    std::cout << total << "\n";
    return 0;
}