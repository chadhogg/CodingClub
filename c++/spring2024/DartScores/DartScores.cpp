/// \file DartScores.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/dartscores.


#include <iostream>
#include <cmath>

// radius = 20 * (11 - p)
// radius / 20 = 11 - p
// p = 11 - radius / 20

const int COUNT = 10;
const int RADII[] = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200};

int
score (int x, int y) {
    for (int index = 0; index < COUNT; ++index) {
        if (sqrt (x * x + y * y) <= RADII[index]) {
            return 11 - (RADII[index] / 20);
        }
    }
    return 0;
}

int
main ()
{
    int T, n, sum;
    std::cin >> T;
    for (int i = 0; i < T; ++i) {
        int n, sum, x, y;
        std::cin >> n;
        sum = 0;
        for (int j = 0; j < n; ++j) {
            std::cin >> x >> y;
            sum += score(x, y);
        }
        std::cout << sum << "\n";
    }
    return 0;
}
