/// \file FibonacciGjjof.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/fibonaccigjof
/// NOTE: This only solves the first 87/99 test cases within limits.
///   Everything I have tried gets either TLE or MLE on test 88.

#include <cstddef>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <cmath>

using NUMBER = unsigned long;

const NUMBER MOD = 1'000'000'007;

// This works, but runs out of memory.
NUMBER
slowFibLookup (NUMBER index)
{
    static std::unordered_map<NUMBER, NUMBER> cache;
    assert (index > 0);
    if (cache.count (index) == 0) {
        if (index <= 2) { cache[index] = 1; }
        else { cache[index] = (slowFibLookup (index - 1) + slowFibLookup (index - 2)) % MOD; }
    }
    return cache[index];
}


// This seems hopeless due to precision errors one n gets fairly high.
NUMBER
binetFormulaFibCompute (NUMBER index)
{
    const static long double GOLD_RATIO = 0.5 * (1 + std::sqrt (5));
    const static long double CONJUGATE = 0.5 * (1 - std::sqrt (5));
    long double grNth = 1.0;
    long double cNth = 1.0;
    for (NUMBER i = 0; i < index; ++i) {
        grNth *= GOLD_RATIO;
        cNth *= CONJUGATE;
    }
    assert (grNth > cNth);
    long double result = (grNth - cNth) / std::sqrt (5);
    while (result > MOD) {
        result -= MOD;
    }
    NUMBER intResult = (result + 0.5);
    return intResult % MOD;
}

// Based on the "fast doubling" technique described at https://www.nayuki.io/page/fast-fibonacci-algorithms
// It works because all operations can be done on integers and are modulo congruent.
// But it still either takes too much space (with memoization) or too much time (without).
NUMBER
fastDoublingFibCompute (NUMBER index, std::unordered_map<NUMBER, NUMBER>& cache)
{
    //static std::unordered_map<NUMBER, NUMBER> cache;
    assert (index > 0);
    if (cache.count (index) == 0) {
        if (index <= 2) {
            cache[index] = 1;
        }
        else if (cache.count (index - 1) == 1 && cache.count (index - 2) == 1) {
            // If we can compute this the "normal" way based on data we already have,
            //   that will actually be faster than the "fast" way.
            // Hopefully the indexes in the input are in increasing order ...
            cache[index] = (cache[index - 1] + cache[index - 2]) % MOD;
        }
        else if (index % 2 == 0) {
            NUMBER half = fastDoublingFibCompute (index / 2, cache);
            NUMBER halfPlusOne = fastDoublingFibCompute (index / 2 + 1, cache);
            // This is needed because otherwise we get a negative number.
            if (2 * halfPlusOne < half) {
                halfPlusOne += MOD;
            }
            cache[index] = half * (2 * halfPlusOne - half) % MOD;
        }
        else {
            NUMBER half = fastDoublingFibCompute (index / 2, cache);
            NUMBER halfPlusOne = fastDoublingFibCompute (index / 2 + 1, cache);
            cache[index] = (half * half + halfPlusOne * halfPlusOne) % MOD;
        }
    }
    return cache[index];

}

size_t
fibs (const std::vector<NUMBER>& fibIndexes, NUMBER left, NUMBER right, std::unordered_map<NUMBER, NUMBER>& cache)
{
    NUMBER total = 0;
    for (NUMBER i = left; i <= right; ++i) {
        total = (total + fastDoublingFibCompute (fibIndexes.at (i), cache)) % MOD;
    }
    return total;
}

int
main()
{
    NUMBER arrayLength, numOperations;
    std::cin >> arrayLength >> numOperations;
    std::vector<NUMBER> fibonacciIndices;
    fibonacciIndices.reserve (arrayLength + 1);
    fibonacciIndices.push_back (0); // Irrelevant data, gets us effectively 1-based indexing.
    for (NUMBER i = 0; i < arrayLength; ++i) {
        NUMBER x;
        std::cin >> x;
        fibonacciIndices.push_back (x);
    }
    for (NUMBER i = 0; i < numOperations; ++i) {
        int type;
        std::cin >> type;
        assert (type == 1 || type == 2);
        if (type == 1) {
            NUMBER left, right, value;
            std::cin >> left >> right >> value;
            assert (left <= right);
            for (size_t j = left; j <= right; ++j) {
                fibonacciIndices[j] += value;
            }
        }
        else {
            NUMBER left, right;
            std::cin >> left >> right;
            // NOTE: Tried making this cache new for each operation 2, trying to avoid MLE -- just got me TLE instead.
            std::unordered_map<NUMBER, NUMBER> cache;
            NUMBER total = fibs (fibonacciIndices, left, right, cache);
            std::cout << total % MOD << "\n";
        }
    }
    return 0;
}