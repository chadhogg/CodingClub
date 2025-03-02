/// \file Tarifa.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/tarifa

#include <iostream>
#include <cassert>

int
main()
{
    int x, n;
    std::cin >> x >> n;
    assert (1 <= x && x <= 100);
    assert (1 <= n && n <= 100);
    int p;
    int total = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> p;
        total += (x - p);
    }
    std::cout << total + x << "\n";
    return 0;
}