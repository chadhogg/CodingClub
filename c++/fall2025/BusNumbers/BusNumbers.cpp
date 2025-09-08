/// \file BusNumbers.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/busnumbers

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>


int
main()
{
    int count, x;
    std::cin >> count;
    std::vector<int> numbers;
    numbers.reserve (count);
    for (int i = 0; i < count; ++i) {
        std::cin >> x;
        numbers.push_back (x);
    }
    std::sort (numbers.begin (), numbers.end ());

    for (std::size_t startIndex = 0; startIndex < numbers.size (); ) {
        std::size_t endIndex;
        for (endIndex = startIndex + 1; endIndex < numbers.size () && numbers[endIndex] == numbers[endIndex - 1] + 1; ++endIndex) {
        }
        if (startIndex == endIndex - 1) {
            std::cout << numbers[startIndex] << " ";
            ++startIndex;
        }
        else if (startIndex == endIndex - 2) {
            std::cout << numbers[startIndex] << " " << numbers[startIndex + 1] << " ";
            startIndex += 2;
        }
        else {
            std::cout << numbers[startIndex] << "-" << numbers[endIndex - 1] << " ";
            startIndex = endIndex;
        }
    }
    std::cout << "\n";
    return 0;
}