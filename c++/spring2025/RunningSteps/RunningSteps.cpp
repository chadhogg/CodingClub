/// \file RunningSteps.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/runningsteps

#include <iostream>
#include <cassert>
#include <map>

// Rules:
// - Must run exactly the correct number of steps.
// - Even-numbered strides are with left leg, odd-numbered with even leg.
// - Both legs do same number of two-step strides as each other.
// - Both legs do same number of one-step strides as each other.
// - Num two-step strides >= num one-step strides.

// Examples:
// - 2 steps (1 per leg) *must be* 0 2-step, 1 1-step each.
//     1 order for left * 1 order for right = 1 order
// - 4 steps (2 per leg) *must be* 1 2-step, 0 1-step each.
//     1 order for left * 1 order for right = 1 order
// - 6 steps (3 per leg) *must be* 1 2-step, 1 1-step each.
//     2 orders for left * 2 orders for right = 4 orders
// - 8 steps (4 per leg) *must be* 2 2-step, 0 1-step each.
//     1 order for left * 1 order for right = 1 order
// - 10 steps (5 per leg) *must be* 2 2-step, 1 1-step each.
//     3 orders for left * 3 orders for right = 9 orders
// - 12 steps (6 per leg) *could be* 3 2-step, 0 1-step each *or* 2 2-step, 2 1-step each.
//     doing no 1-steps: 1 order for left * 1 order for right = 1 order
//     doing 2 1-steps: 6 orders for left * 6 orders for right = 36 orders
//                                                        total 37 orders
// - 14 steps (7 per leg) *must be* 3 2-step, 1 1-step each.
//     4 orders for left * 4 orders for right = 16 orders
// - 16 steps (8 per leg) *could be* 4 2-step, 0 1-step each *or* 3 2-step, 2 1-step each.
//     doing no 1-steps: 1 order for left * 1 order for right = 1 order
//     doing 2 1-steps: 10 orders for left * 10 orders for right = 100 orders
//                                                        total 101 orders
// - 18 steps (9 per leg) *could be* 4 2-step, 1 1-step each *or* 3 2-step, 3 1-step each

// Insight: once we have decided how many 2-step and 1-step strides must be made, we just need
//   (total strides) choose (1-step strides) to find all orderings.

long choose (long n, long k)
{
    // Note: Formula taken from https://en.wikipedia.org/wiki/Binomial_coefficient#Multiplicative_formula
    // Note: had to split up numerator and denominator to avoid integer division.
    long numer = 1, denom = 1;
    for (long i = 1; i <= k; ++i) {
        numer *= (n + 1 - i);
        denom *= i;
    }
    assert (numer % denom == 0); // <-- breaking on some hidden test case
    return numer / denom;
}

long chooseSlow (long n, long k)
{
    static std::map<std::pair<long, long>, long> cache;
    std::pair<long, long> args = {n, k};
    if (cache.contains (args)) { return cache[args]; }
    else if (k == 0 || k == n) { return 1; }
    else { 
        long answer = chooseSlow (n - 1, k - 1) + chooseSlow (n - 1, k);
        cache[args] = answer;
        return answer;
    }
}

long permutations (int num2Steps, int num1Steps)
{
    return chooseSlow (num2Steps + num1Steps, num1Steps);
}

long
countWays (int stepsToRun) {
    int stepsPerLeg = stepsToRun / 2;
    long totalWays = 0;
    // I'm sure there is a way to compute a better lower bound, but it isn't occurring to me.
    for (int num2Steps = 1; num2Steps <= stepsPerLeg / 2; ++num2Steps) {
        int num1Steps = stepsPerLeg - num2Steps * 2;
        if (num1Steps >= 0 && num2Steps >= num1Steps) {
            long oneLegOptions = permutations (num2Steps, num1Steps);
            totalWays += oneLegOptions * oneLegOptions;
        }
    }
    return totalWays;
}

int
main()
{
    int numDatasets, setNum, stepsToRun;
    std::cin >> numDatasets;
    assert (numDatasets >= 1 && numDatasets <= 1000);
    for (int i = 0; i < numDatasets; ++i) {
        std::cin >> setNum >> stepsToRun;
        assert (stepsToRun % 2 == 0 && stepsToRun >= 2 && stepsToRun <= 100);
        std::cout << setNum << " " << countWays (stepsToRun) << "\n";
    }
    return 0;
}