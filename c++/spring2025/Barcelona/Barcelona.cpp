/// \file Barcelona.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/barcelona

#include <iostream>

int
main()
{
    int numBags, bennyBag, nextBag;
    std::cin >> numBags;
    std::cin >> bennyBag;
    for (int beenThrough = 0; beenThrough < numBags; ++beenThrough) {
        std::cin >> nextBag;
        if (bennyBag == nextBag) {
            if (beenThrough == 0) { std::cout << "fyrst\n"; }
            else if (beenThrough == 1) { std::cout << "naestfyrst\n"; }
            else { std::cout << (beenThrough + 1) << " fyrst\n"; }
        }
    }

    return 0;
}