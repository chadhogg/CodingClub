/// \file MuchRoomForMushrooms.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/muchroomformushrooms

#include <iostream>
#include <cassert>

// 1x1 uses any single mushroom
// 1x2 uses part of the cap of any single mushroom
// 1x3 uses part of the cap of any single mushroom
// 1x4 uses parts of 2 mushrooms
// 2x1 uses any single mushroom
// 2x2 uses parts of 2 mushrooms
// 2x3 uses parts of 2 mushrooms (example from writeup)
// 3x2 should not be possible ... right?


int
main()
{
    int r, c;
    std::cin >> r >> c;
    if (r == 1) {
        // We can accomplish this with a bunch of mushrooms with adjacent caps.
        std::cout << (c + 2) / 3 << "\n";
    }
    else if (c == 1) {
        // We can always accomplish this with a single mushroom.
        std::cout << 1 << "\n";
    }
    else if (r == 2) {
        // The first mushroom gives us 1 column, each successive one adds 2 columns.
        std::cout << (c / 2) + 1 << "\n";
    }
    else {
        // Doesn't seem possible.
        std::cout << -1 << "\n";
    }
    return 0;
}

