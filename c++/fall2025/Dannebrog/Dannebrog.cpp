/// \file Dannebrog.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/dannebrog

#include <iostream>
#include <cassert>

// We are guaranteed that the answer is an integer, so I'd like to only ever use integer operations.
// It's pretty annoying to do so, though ...
int
main()
{
    int breadth;
    std::cin >> breadth;
    assert (breadth % 7 == 0);
    int whitePartOfBreadth = breadth / 7;
    int redPartOfBreadth = 6 * whitePartOfBreadth;
    int doubleShortDimension = redPartOfBreadth;
    int quadrupleOtherDimension = 3 * doubleShortDimension;
    assert ((doubleShortDimension * doubleShortDimension) % 2 == 0);
    int doubleSquare = doubleShortDimension * doubleShortDimension / 2;
    assert ((doubleShortDimension * quadrupleOtherDimension) % 4 == 0);
    int doubleRectangle = doubleShortDimension * quadrupleOtherDimension / 4;
    int area = doubleSquare + doubleRectangle;
    std::cout << area << "\n";
    return 0;
}