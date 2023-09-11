/// \file ClassFieldTrip.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/classfieldtrip

#include <iostream>
#include <string>
#include <cassert>


int
main (int argc, char* argv[])
{
    std::string first, second;
    std::cin >> first >> second;
    std::string result;
    int fIndex = 0, sIndex = 0;
    while (fIndex < first.size () || sIndex < second.size ()) {
        if (fIndex < first.size () && sIndex < second.size()) {
            if (first[fIndex] < second[sIndex]) {
                result.push_back (first[fIndex]);
                fIndex++;
            }
            else {
                result.push_back (second[sIndex]);
                sIndex++;
            }
        }
        else if(fIndex < first.size ()) {
            result.push_back (first[fIndex]);
            fIndex++;
        }
        else {
            result.push_back (second[sIndex]);
            sIndex++;
        }
    }
    std::cout << result << "\n";
    return 0;
}