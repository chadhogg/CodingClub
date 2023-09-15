/// \file PermutedArithmeticSequence.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/permutedarithmeticsequence

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

bool
isArithmetic(const std::vector<int>& sequence)
{
    if (sequence.size() < 3) {
        return true;
    }
    int diff = sequence.at (1) - sequence.at (0);
    for (int index = 2; index < sequence.size (); ++index) {
        if (sequence.at (index) - sequence.at (index - 1) != diff) {
            return false;
        }
    }
    return true;
}

std::string
solveSequence() {
    int length, next;
    std::cin >> length;
    std::vector<int> sequence;
    sequence.reserve (length);
    while (length > 0) {
        std::cin >> next;
        sequence.push_back (next);
        --length;
    }
    if (isArithmetic (sequence)) {
        return "arithmetic";
    }
    std::sort (sequence.begin (), sequence.end ());
    if (isArithmetic (sequence)) {
        return "permuted arithmetic";
    }
    return "non-arithmetic";
}

int
main (int argc, char* argv[])
{
    int numSequences;
    std::cin >> numSequences;
    for (int i = 0; i < numSequences; ++i) {
        std::cout << solveSequence () << "\n";
    }
    return 0;
}