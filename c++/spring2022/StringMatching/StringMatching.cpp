/// \file StringMatching.cpp
/// \brief A solution to https://open.kattis.com/problems/stringmatching
/// \author Chad Hogg
/// \version 2022-01-27

#include <string>
#include <iostream>
#include <vector>

struct Problem {
    std::string pattern;
    std::string text;
};

void bruteForceSolve (const Problem& prob) {
    for (std::size_t textIndex = 0; textIndex < prob.text.size () - prob.pattern.size () + 1; ++textIndex) {
        bool matching = true;
        for (std::size_t patternIndex = 0; patternIndex < prob.pattern.size () && matching; ++patternIndex) {
            if (prob.text[textIndex + patternIndex] != prob.pattern[patternIndex]) {
                matching = false;
            }
        }
        if (matching) {
            std::cout << textIndex << " ";
        }
    }
    std::cout << "\n";
}

/// \note This is a direct implementation of the pseudocode at https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm#Description_of_pseudocode_for_the_table-building_algorithm
std::vector<int> buildKMPTable (std::string pattern) {
    std::vector<int> prefixTable;
    prefixTable.push_back (-1);

    int backtrackAmount = 0;
    
    for (int nextTablePosition = 1; nextTablePosition < pattern.length (); ++nextTablePosition) {
        if (pattern[nextTablePosition] == prefixTable[backtrackAmount]) {
            prefixTable.push_back (prefixTable[backtrackAmount]);
        }
        else {
            prefixTable.push_back (backtrackAmount);
            while (backtrackAmount >= 0 && pattern[nextTablePosition] != pattern[backtrackAmount]) {
                backtrackAmount = prefixTable[backtrackAmount];
            }
        }
        ++backtrackAmount;
    }
    prefixTable.push_back (backtrackAmount);
    return prefixTable;
}

/// \note This is a direct implementation of the pseudocode at https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm#Description_of_pseudocode_for_the_search_algorithm
void KMPSolve (const Problem& prob) {
    std::vector<int> prefixTable = buildKMPTable (prob.pattern);
    int textIndex = 0;
    int patternIndex = 0;
    while (textIndex < prob.text.size ()) {
        if (prob.pattern[patternIndex] == prob.text[textIndex]) {
            textIndex++;
            patternIndex++;
            if (patternIndex == prob.pattern.size ()) {
                std::cout << (textIndex - patternIndex) << " ";
                patternIndex = prefixTable[patternIndex];
            }
        }
        else {
            patternIndex = prefixTable[patternIndex];
            if (patternIndex < 0) {
                textIndex++;
                patternIndex++;
            }
        }
    }
    std::cout << "\n";
}


int main(int argc, char* argv[]) {
    std::string line, line2;
    while (std::getline (std::cin, line)) {
        std::getline (std::cin, line2);
        KMPSolve ({line, line2});
    }

    return 0;
}