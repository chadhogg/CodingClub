/// \file HardwoodSpecies.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/hardwoodspecies

#include <iostream>
#include <string>
#include <map>

int
main (int argc, char* argv[])
{
    std::map<std::string, int> counts;
    int total = 0;
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (counts.count(line) == 0)
        {
            counts[line] = 0;
        }
        counts[line] = counts[line] + 1;
        total++;
    }

    for (auto pair : counts)
    {
        std::cout << pair.first << " " << 100.0 * (double)pair.second / (double)total << "\n";
    }

    return 0;
}