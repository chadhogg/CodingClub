/// \file BigBoxes.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/bigboxes
/// There are lots of works-but-too-slow solutions here.
/// The final, fast-enough one is indebted to ChatGPT by way of Marshall Feng.

#include <iostream>
#include <vector>
#include <set>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cassert>

struct Problem
{
    int numItems;
    int numBoxes;
    std::vector<int> weights;
};

Problem
readInput ()
{
    Problem prob;
    std::cin >> prob.numItems >> prob.numBoxes;
    for (int i = 0; i < prob.numItems; ++i) {
        int x;
        std::cin >> x;
        prob.weights.push_back (x);
    }
    return prob;
}

std::ostream&
operator<< (std::ostream& out, const std::set<std::size_t> s)
{
    for (std::size_t t : s) {
        out << t << " ";
    }
    return out;
}

struct PartialSplit
{
    std::vector<int> items;
    int moreSplitsToDo;
    auto operator<=> (const PartialSplit& other) const = default;
};

int
sum (const PartialSplit& split) {
    int total = 0;
    for (int i : split.items) {
        total += i;
    }
    return total;
}

std::ostream&
operator<< (std::ostream& out, const PartialSplit& split)
{
    out << "( ";
    for (int i : split.items) {
        out << i << " ";
    }
    out << ") with " << split.moreSplitsToDo << " to go";
    return out;
}

int
divideAndConquer (const struct PartialSplit& split) {
    static std::map<PartialSplit, int> cache;
    if (cache.contains (split)) { return cache.at (split); }
    if (split.moreSplitsToDo == 0) {
        cache[split] = sum (split);
        return cache[split];
    }
    int best = INT32_MAX;
    int firstHalfSplitsToDo = (split.moreSplitsToDo - 1) / 2;
    int secondHalfSplitsToDo = split.moreSplitsToDo / 2;
    for (int where = 1 + firstHalfSplitsToDo; where <= split.items.size () - 1 - secondHalfSplitsToDo; ++where) {
        PartialSplit before, after;
        before.moreSplitsToDo = firstHalfSplitsToDo;
        after.moreSplitsToDo = secondHalfSplitsToDo;
        for (int i = 0; i < split.items.size (); ++i) {
            if (i < where) { before.items.push_back (split.items[i]); }
            else { after.items.push_back (split.items[i]); }
        }
        int largest = std::max (divideAndConquer (before), divideAndConquer (after));
        best = std::min (best, largest);
    }
    cache[split] = best;
    return best;
}

struct NewPartialSplit
{
    std::size_t firstIndex;
    std::size_t lastIndex;
    int numRemainingSplits;
    auto operator<=> (const NewPartialSplit& x) const = default;
};

template<>
struct std::hash<NewPartialSplit>
{
    std::size_t operator()(const NewPartialSplit& s) const noexcept
    {
        return s.firstIndex + s.lastIndex * 37 + s.numRemainingSplits * 89;
    }
};

int
divideAndConquer2 (const Problem& prob, const NewPartialSplit& current)
{
    // Note: If the problem changes, this cache becomes very invalid.
    static std::unordered_map<NewPartialSplit, int> cache;
    
    // If in cache, use cached result.
    if (cache.contains (current)) { return cache.at (current); }

    // If no splits to do, answer is just the weight of this box.
    if (current.numRemainingSplits == 0) {
        int sum = 0;
        for (std::size_t i = current.firstIndex; i <= current.lastIndex; ++i) {
            sum += prob.weights.at (i);
        }
        cache[current] = sum;
        return sum;
    }

    // Try each possible location for a middle split.
    int best = INT32_MAX;
    int firstHalfSplitsToDo = (current.numRemainingSplits - 1) / 2;
    int secondHalfSplitsToDo = current.numRemainingSplits / 2;
    for (std::size_t where = current.firstIndex + 1 + firstHalfSplitsToDo; where <= current.lastIndex - secondHalfSplitsToDo; ++where) {
        NewPartialSplit before {current.firstIndex, where - 1, firstHalfSplitsToDo};
        NewPartialSplit after {where, current.lastIndex, secondHalfSplitsToDo};
        int largest = std::max (divideAndConquer2 (prob, before), divideAndConquer2 (prob, after));
        best = std::min (best, largest);
    }
    cache[current] = best;
    return best;
}

int
greedyKindOf (const Problem& prob)
{
    int start = *std::max_element (prob.weights.begin (), prob.weights.end ());
    int sum = 0;
    for (int x : prob.weights) { sum += x; }
    start = std::max (start, sum / prob.numBoxes);
    for (int maxWeight = start; ; ++maxWeight) {
        int totalWeightSoFar = 0;
        int weightInThisBox = 0;
        int boxesSoFar = 0;
        bool finished = true;
        for (int index = 0; index < prob.numItems && finished; ++index) {
            if (weightInThisBox + prob.weights.at (index) <= maxWeight) {
                weightInThisBox += prob.weights.at (index);
            }
            else if (boxesSoFar < prob.numBoxes - 1) {
                ++boxesSoFar;
                totalWeightSoFar += weightInThisBox;
                weightInThisBox = prob.weights.at (index);
            }
            else {
                finished = false;
            }
        }
        if (finished) { return maxWeight; }
    }
    return INT32_MAX;
}

bool
possible (const Problem& prob, int weightLimit)
{
    int totalWeightSoFar = 0;
    int weightInThisBox = 0;
    int boxesSoFar = 0;
    bool finished = true;
    for (int index = 0; index < prob.numItems && finished; ++index) {
        if (weightInThisBox + prob.weights.at (index) <= weightLimit) {
            weightInThisBox += prob.weights.at (index);
        }
        else if (boxesSoFar < prob.numBoxes - 1) {
            ++boxesSoFar;
            totalWeightSoFar += weightInThisBox;
            weightInThisBox = prob.weights.at (index);
        }
        else {
            return false;
        }
    }
    return true;
}

int binSearch (const Problem& prob)
{
    int lowerBound = *std::max_element (prob.weights.begin (), prob.weights.end ());
    int sum = 0;
    for (int x : prob.weights) { sum += x; }
    lowerBound = std::max (lowerBound, sum / prob.numBoxes);
    int upperBound = sum;
    assert (possible (prob, upperBound));
    while (lowerBound < upperBound) {
        //std::cout << lowerBound << " - " << upperBound << "\n";
        int center = (lowerBound + upperBound) / 2;
        if (possible (prob, center)) {
            upperBound = center;
        }
        else {
            lowerBound = center + 1;
        }
    }
    return upperBound;
}

int
main()
{
    Problem prob = readInput ();
//    std::cout << divideAndConquer ({prob.weights, prob.numBoxes - 1}) << "\n";
//    std::cout << divideAndConquer2 (prob, {0, (std::size_t)prob.numItems - 1, prob.numBoxes - 1}) << "\n";
//    std::cout << greedyKindOf (prob) << "\n";
    std::cout << binSearch (prob) << "\n";
    return 0;
}