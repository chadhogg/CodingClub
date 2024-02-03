/// \file WaterJournal.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/waterjournal

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    int n, a, b;
    std::cin >> n >> a >> b;
    int min;
    std::cin >> min;
    int max = min;
    for (int i = 1; i < n - 1; ++i) {
        int x;
        std::cin >> x;
        min = std::min (min, x);
        max = std::max (max, x);
    }
    // Saw a cup outside the range.
    if (min < a || max > b) {
        std::cout << -1 << "\n";
    }
    // Did not see either min or max.
    else if (a < min && b > max) {
        std::cout << -1 << "\n";
    }
    // Did not see min.
    else if (a < min) {
        std::cout << a << "\n";
    }
    // Did not see max.
    else if (b > max) {
        std::cout << b << "\n";
    }
    // Any number in range should be good.
    else {
        for (int i = a; i <= b; ++i) {
            std::cout << i << "\n";
        }
    }
    return 0;
}