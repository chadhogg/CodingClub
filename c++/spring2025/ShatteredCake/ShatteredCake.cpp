/// \file ShatteredCake.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/shatteredcake

#include <iostream>
#include <vector>
#include <cassert>


std::size_t
findLength (std::size_t width, const std::vector<std::pair<std::size_t, std::size_t>>& pieces)
{
    std::size_t totalArea = 0;
    for (const std::pair<std::size_t, std::size_t>& piece : pieces) {
        totalArea += piece.first * piece.second;
    }
    return totalArea / width;
}

int
main()
{
    std::size_t width, numPieces, w, l;
    std::cin >> width >> numPieces;
    std::vector<std::pair<std::size_t, std::size_t>> pieces;
    while (pieces.size () < numPieces) {
        std::cin >> w >> l;
        pieces.push_back ({w, l});
    }

    std::cout << findLength (width, pieces) << "\n";
    return 0;
}