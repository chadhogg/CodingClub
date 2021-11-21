/// \file Sequences.cpp
/// \author Chad Hogg
/// \version 2021-11-20
/// \brief A solution to https://open.kattis.com/problems/sequences

#include <iostream>
#include <string>

/// We are storing the sum of swap modulo this strangely chosen value.
const long MODULO {1000000007U};

/// \brief Adds two numbers, maintaining our modulo invariant.
/// \param x One number.
/// \param y Another number.
/// \return The sum of those numbers, modulo MODULO.
static inline long addModulo (long x, long y) {
    return (x + y) % MODULO;
}

/// \brief Multiplies two numbers, maintaining our modulo invariant.
/// \param x One number.
/// \param y Another number.
/// \return The product of those numbers, modulo MODULO.
static inline long multiplyModulo (long x, long y) {
    return (x % MODULO) * (y % MODULO) % MODULO;
}

/// \brief (Slowly) generates a string of a certain number of spaces, for debugging output.
/// \param index The recursion depth.
/// \return A string containing 2*index spaces.
static std::string prefix(int index) {
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
static std::pair<long, long> slowAlgorithm (std::string& sequence, int index, int numOnesSoFar) {
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

/// \brief An attempt at a faster apporach through divide-and-conquer.
/// \param[in] sequence The string we are working with.
/// \param[in] begin The first index in the substring we are responsible for.
/// \param[in] end The first index past the substring we are responsible for.
/// \return A pair in which the first element is the number of swaps this subsequence will require per '1' that comes before it,
///   and the second element is the number of '1's in the subsequence.
/// \note I haven't yet thought about how to deal with '?'s.  It seems that perhaps this isn't going to work.
static std::pair<long, long> splitAlgorithm (std::string& sequence, int begin, int end) {
    //std::cout << prefix (sequence.length() - (end - begin)) << "Working on [" << begin << ", " << end << ")\n";
    std::pair<long, long> result;
    if (begin >= end) {
        result.first == 0L;
        result.second == 0L;
    }
    else if(begin == end - 1) {
        if (sequence[begin] == '0') {
            result.first = 0L;
            result.second = 0L;
        }
        else if(sequence[begin] == '1') {
            result.first = 0L;
            result.second = 1L;
        }
        else {
            // TODO
        }
    }
    else {
        std::pair<long, long> firstHalf = splitAlgorithm (sequence, begin, (begin + end) / 2);
        std::pair<long, long> secondHalf = splitAlgorithm (sequence, (begin + end) / 2, end);
        long zeroesInFirstHalf = (begin + end) / 2 - begin - firstHalf.second;
        long onesInFirstHalf = firstHalf.second;
        long zeroesInSecondHalf = end - (begin + end) / 2 - secondHalf.second;
        long onesInSecondHalf = secondHalf.second;
        long swapsWithinFirstHalf = firstHalf.first;
        long swapsWithinSecondHalf = secondHalf.first;
        long totalSwaps = addModulo (swapsWithinFirstHalf, swapsWithinSecondHalf);
        totalSwaps = addModulo (totalSwaps, multiplyModulo (zeroesInSecondHalf, onesInFirstHalf));
        result.first = totalSwaps;
        result.second = firstHalf.second + secondHalf.second;
    }
    //std::cout << prefix (sequence.length() - (end - begin)) << "Returning {" << result.first << ", " << result.second << "}\n";
    return result;
}


/// \brief A technique similar to slowAlgorithm() , but it simulates all of the branches without processing them individually.
/// \param[in] sequence The bit sequence.
/// \return The total number of swaps needed, modulo MODULO.
/// \note This looked like it worked on every one of my test cases, but failed kattis's case 15.
static unsigned long fastForward (std::string& sequence) {
    // Twice the number of '1' seen thus far; future '0's will have to swap once with each of them.
    // The reason we use "twice" is because on '?' marks we want to add half a '1' but only use integer arithmetic.
    double doubledOnesPerBranch {0UL};
    // The number of times the search would have branched so far; future work will simulate what is happening on each branch simultaneously.
    unsigned long numBranches {1UL};
    // The number of swaps we have already observed, along all branches.
    unsigned long numSwapsSoFar {0UL};


    for (unsigned int index {0U}; index < sequence.length(); ++index) {
        if (sequence[index] == '1') {
            // Each branch has another '1' that future '0's will have to move past.
            doubledOnesPerBranch = addModulo (doubledOnesPerBranch, 2UL);
        }
        else if (sequence[index] == '0') {
            // This '0' will need to move through all '1's on all branches.
            numSwapsSoFar = addModulo (numSwapsSoFar, multiplyModulo (doubledOnesPerBranch, numBranches) / 2UL);
        }
        else if (sequence[index] == '?') {
            // We are about to double the number of branches, which means we need to double the number of swaps that have happened so far.
            numSwapsSoFar = multiplyModulo (numSwapsSoFar, 2UL);
            // One copy of each branch now ends with a '0', which needs to be moved past all of the '1's.
            numSwapsSoFar = addModulo (numSwapsSoFar, multiplyModulo (doubledOnesPerBranch, numBranches) / 2UL);
            // One copy of each branch now ends with a '1', so we have another '1' in half of them.
            doubledOnesPerBranch = addModulo (doubledOnesPerBranch, 1UL);
            // We now have twice as many branches as we did before.
            numBranches = multiplyModulo (numBranches, 2UL);
        }
        else {
            throw std::invalid_argument("sequence should have contained a " + sequence[index]);
        }
    }
    return numSwapsSoFar;
}


/// \brief A variant of fastForward() that stores the total number of 1s across all branches, so I don't have to deal with divide.
/// \param[in] sequence The bit sequence.
/// \return The total number of swaps needed, modulo MODULO.
static unsigned long fastForward2 (std::string& sequence) {
    // The total number of '1's seen on all branches combined.
    double totalOnes {0UL};
    // The number of times the search would have branched so far; future work will simulate what is happening on each branch simultaneously.
    unsigned long numBranches {1UL};
    // The number of swaps we have already observed, along all branches.
    unsigned long numSwapsSoFar {0UL};


    for (unsigned int index {0U}; index < sequence.length(); ++index) {
        if (sequence[index] == '1') {
            // Each branch has another '1' that future '0's will have to move past.
            totalOnes = addModulo (totalOnes, numBranches);
        }
        else if (sequence[index] == '0') {
            // This '0' will need to move through all '1's on all branches.
            numSwapsSoFar = addModulo (numSwapsSoFar, totalOnes);
        }
        else if (sequence[index] == '?') {
            // We are about to double the number of branches, which means we need to double the number of swaps that have happened so far.
            numSwapsSoFar = multiplyModulo (numSwapsSoFar, 2UL);
            // One copy of each branch now ends with a '0', which needs to be moved past all of the '1's.
            numSwapsSoFar = addModulo (numSwapsSoFar, totalOnes);
            // We are about to double the number of branches, which means we are doubling the number of '1's across all branches.
            totalOnes = multiplyModulo (totalOnes, 2UL);
            // One copy of each branch now ends with a '1', so we have another '1' in half of them.
            totalOnes = addModulo (totalOnes, numBranches);
            // We now have twice as many branches as we did before.
            numBranches = multiplyModulo (numBranches, 2UL);
        }
        else {
            throw std::invalid_argument("sequence should have contained a " + sequence[index]);
        }
    }
    return numSwapsSoFar;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    std::string sequence;
    std::cin >> sequence;
    //std::cout << slowAlgorithm (sequence, 0, 0).first << "\n";
    //std::cout << splitAlgorithm (sequence, 0, sequence.length()).first << "\n";
    std::cout << fastForward2 (sequence) << "\n";
    return 0;
}