/// \file TornByGGE.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/tornbygge

#include <iostream>
#include <string>
#include <cassert>


int
main (int argc, char* argv[])
{
    int blockCount;
    std::cin >> blockCount;
    int towerCount = 0;
    int topWidth = 0;
    for (int i = 0; i < blockCount; ++i) {
        int width;
        std::cin >> width;
        if (width > topWidth) {
            ++towerCount;
        }
        topWidth = width;
    }
    std::cout << towerCount << "\n";
    return 0;
}