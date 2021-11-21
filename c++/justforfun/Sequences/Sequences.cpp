/// \file Sequences.cpp
/// \author Chad Hogg
/// \version 2021-11-20
/// \brief A solution to https://open.kattis.com/problems/sequences
/// \note This is currently a work in progress.

#include <iostream>
#include <string>

/// We are storing the sum of swap modulo this strangely chosen value.
const long MODULO {1000000007U};

/// \brief Adds two numbers, maintaining our modulo invariant.
/// \param x One number.
/// \param y Another number.
/// \return The sum of those numbers, modulo MODULO.
inline long addModulo (long x, long y) {
    return (x + y) % MODULO;
}

/// \brief Multiplies two numbers, maintaining our modulo invariant.
/// \param x One number.
/// \param y Another number.
/// \return The product of those numbers, modulo MODULO.
inline long multiplyModulo (long x, long y) {
    return (x % MODULO) * (y % MODULO) % MODULO;
}

/// \brief (Slowly) generates a string of a certain number of spaces, for debugging output.
/// \param index The recursion depth.
/// \return A string containing 2*index spaces.
std::string prefix(int index) {
    std::string s {};
    for(int x {0U}; x < index; ++x) {
        s += "  ";
    }
    return s;
}

/// \brief A way of solving the problem that's too slow to be practical, but will hopefully lead me somewhere.
/// \param[in] sequence The bit sequence to work on.
/// \param[in] index Where in the sequence we are currently working.  (Everything before is finished.)
/// \param[in] numOnesSoFar The number of ones that have been seen before the index.
/// \return A pair containing the total number of swaps needed for (real and theoretical) 0s at and beyond the index
///   and the total number of times the algorithm branched at or past the index.
/// \note The problem, of course, is that we have an exponential number of branch points.  I need to find a way to
///   handle the final else without two recursive calls.
/// \note I have an idea, not yet explored, that involves dividing the sequence in half, solving each half
///   separately, and combining the results.
std::pair<long, long> slowAlgorithm (std::string& sequence, int index, int numOnesSoFar) {
    std::pair<long, long> result;
    if (index == sequence.length ()) {
        // If we've reached the end of the sequence, there are no zeroes from here on out that
        //   need moved, and only a single branch.
        result.first = 0L;
        result.second = 1L;
    }
    else {
        result.first = 0L;
        result.second = 0L;
        if (sequence[index] == '1' || sequence[index] == '?') {
            // If there is (or could be) a 1 here, we don't have to move it (though we will have to move any
            //   future 0s through it).  
            std::pair<long, long> onesRecursion = slowAlgorithm (sequence, index + 1, numOnesSoFar + 1);
            result.first = addModulo (result.first, onesRecursion.first);
            result.second += onesRecursion.second;
        }
        if (sequence[index] == '0' || sequence[index] == '?') {
            // If there is (or could be) a 0 here, we have to move it but don't need to move future 0s
            //   through it.  Furthermore, we need to make those moves *for each* future branch.
            std::pair<long, long> zeroesRecursion = slowAlgorithm (sequence, index + 1, numOnesSoFar);
            result.first = addModulo (result.first, multiplyModulo (numOnesSoFar, zeroesRecursion.second));
            result.first = addModulo (result.first, zeroesRecursion.first);
            result.second += zeroesRecursion.second;
        }
    }
    return result;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    std::string sequence;
    std::cin >> sequence;
    std::cout << slowAlgorithm (sequence, 0, 0).first << "\n";
}