/// \file Supercomputer.cpp
/// \brief A solution to https://open.kattis.com/problems/supercomputer
/// \author Chad Hogg
/// \version 2022-02-17
/// \note My idea here is that we don't need to keep track of the status of individual bits if we can instead store contiguous ranges that match.
///   This should cut down both space and time requirements dramatically.
///   This idea is heavily inspired by the way we manage free blocks in the CSCI380 malloc lab.
/// \note Currently this appears to produce correct answers but runs too slowly to be accepted.

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <set>

enum Type { FLIP, QUERY };

struct Instruction {
    Type type;
    int flipBit;
    int startBit;
    int endBit;
};

struct Range {
    int start;
    int end;
    bool on;
};

using Ranges = std::list<Range>;
using Instructions = std::vector<Instruction>;

struct Problem {
    Ranges ranges;
    Instructions instructions;
};

Problem readProblem () {
    int N, K;
    std::cin >> N >> K;
    Problem prob { { {0, N - 1, false} }, {}};
    prob.instructions.reserve (K);
    for(int i = 0; i < K; ++i) {
        std::string type;
        std::cin >> type;
        if (type == "F") {
            int bit;
            std::cin >> bit;
            prob.instructions.push_back ({FLIP, bit, 0, 0});
        }
        else if (type == "C") {
            int first, last;
            std::cin >> first >> last;
            prob.instructions.push_back ({QUERY, 0, first, last});
        }
        else {
            throw std::runtime_error ("Unknown type " + type);
        }
    }
    return prob;
}

void printRanges (Ranges const& ranges) {
    for (Range const& range : ranges) {
        std::cout << range.start << "-" << range.end << (range.on ? 'T' : 'F') << " ";
    }
    std::cout << "\n";
}

void processFlip (Ranges & ranges, Instruction const& inst) {
    Ranges::iterator iter = ranges.begin ();
    // Find the range that includes this bit.
    while (inst.flipBit > iter->end) {
        ++iter;
    }
    // If this range was a single bit wide, we can merge it with a surrounding range.
    if (iter->start == iter->end) {
        Ranges::iterator prev = iter;
        Ranges::iterator next = iter;
        --prev;
        ++next;
        // Unless, of course, it is the only range ...
        if (prev == ranges.end () && next == ranges.end ()) {
            iter->on = !iter->on;
        }
        // If it was the first range, just combine it with the following one.
        else if (prev == ranges.end ()) {
            --(next->start);
            ranges.erase (iter);
        }
        // If it was the last range, just combine it with the previous one.
        else if (next == ranges.end ()) {
            ++(prev->end);
            ranges.erase (iter);
        }
        // If it was somewhere in the middle, combine it with both its predecessor and successor.
        else {
            iter->start = prev->start;
            iter->end = next->end;
            iter->on = !iter->on;
            ranges.erase (prev);
            ranges.erase (next);
        }
    }
    // If the bit being flipped is the very first one in the range ...
    else if (iter->start == inst.flipBit) {
        Ranges::iterator prev = iter;
        --prev;
        // ... and it was the first range, then cut off that number to form a new singleton range.
        if (prev == ranges.end ()) {
            ranges.push_front ({inst.flipBit, inst.flipBit, !iter->on});
            ++(iter->start);
        }
        // ... and there is a predecessor range, just move one bit from this range to it.
        else {
            assert (prev->on != iter->on);
            assert (prev->end + 1 == iter->start);
            ++(prev->end);
            ++(iter->start);
        }
    }
    // If the bit being flipped is the very last one in the range ...
    else if (iter->end == inst.flipBit) {
        Ranges::iterator next = iter;
        ++next;
        // ... and it was the last range, then cut off that number to form a new singleton range.
        if (next == ranges.end ()) {
            ranges.push_back ({inst.flipBit, inst.flipBit, !iter->on});
            --(iter->end);
        }
        // ... and there is a successor range, just move one bit from this range to it.
        else {
            assert (next->on != iter->on);
            assert (iter->end + 1 == next->start);
            --(next->start);
            --(iter->end);
        }
    }
    // If the bit being flipped is in the middle of a range, split that range.
    else {
        Range before {iter->start, inst.flipBit - 1, iter->on};
        Range current {inst.flipBit, inst.flipBit, !iter->on};
        Range after {inst.flipBit + 1, iter->end, iter->on};
        ranges.insert (iter, before);
        ranges.insert (iter, current);
        ranges.insert (iter, after);
        ranges.erase (iter);
    }
}

void processQuery (Ranges & ranges, Instruction const& inst) {
    int total = 0;
    Ranges::iterator iter = ranges.begin ();
    // Find the first range that intersects with our target.
    while (iter->end < inst.startBit) {
        ++iter;
    }
    // If it is the only range that intersects with our target, answer and bail out.
    if (iter->end > inst.endBit) {
        if (iter->on) {
            total += inst.endBit - inst.startBit + 1;
        }
        std::cout << total << "\n";
        return;
    }
    // Otherwise process this partial range and move on.
    if (iter->on) {
        total += iter->end - inst.startBit + 1;
    }
    ++iter;
    // Process all of the ranges that are fully inside the target.
    while (iter != ranges.end () && iter->end <= inst.endBit) {
        if (iter->on) {
            total += iter->end - iter->start + 1;
        }
        ++iter;
    }
    // Process the final range that partially intersects the target, if one exists.
    if (iter != ranges.end () && iter->on) {
        total += inst.endBit - iter->start + 1;
    }
    std::cout << total << "\n";
}

void sanityCheck (Ranges const& ranges) {
    assert (!ranges.empty ());
    Ranges::const_iterator iter = ranges.begin ();
    assert (iter->start == 0);
    assert (iter->end >= iter->start);
    bool prevOn = iter->on;
    int prevEnd = iter->end;
    ++iter;
    while (iter != ranges.end ()) {
        assert (iter->on != prevOn);
        assert (iter->start == prevEnd + 1);
        assert (iter->end >= iter->start);
        prevOn = iter->on;
        prevEnd = iter->end;
        ++iter;
    }
}

void printInstruction (Instruction const& inst) {
    if (inst.type == FLIP) {
        std::cout << "F " << inst.flipBit << "\n";
    }
    else {
        std::cout << "C " << inst.startBit << " " << inst.endBit << "\n";
    }
}

void solve (Problem & prob) {
    for (Instruction const& inst : prob.instructions) {
        //printRanges (prob.ranges);
        //SsanityCheck (prob.ranges);
        //printInstruction (inst);
        if (inst.type == FLIP) {
            processFlip (prob.ranges, inst);
        }
        else {
            processQuery (prob.ranges, inst);
        }
    }
}

void alternSolve (Instructions const& instructions) {
    std::set<int> onBits;
    for (Instruction const& inst : instructions) {
        if (inst.type == FLIP) {
            if (onBits.find (inst.flipBit) != onBits.end ()) {
                onBits.erase (inst.flipBit);
            }
            else {
                onBits.insert (inst.flipBit);
            }
        }
        else {
            int count = 0;
            std::set<int>::const_iterator iter = onBits.lower_bound (inst.startBit);
            std::set<int>::const_iterator stop = onBits.upper_bound (inst.endBit);
            while (iter != stop) {
                ++count;
                ++iter;
            }
            std::cout << count << "\n";
        }
    }
}

struct OnRange {
    int firstBit;
    int lastBit;
};

using OnRanges = std::list<OnRange>;
/*
1-1	3-7	10-12	14-18	20-20

keep iterating as long as flip > last

searching for 0, stops on 1-1 and extends 1-1 to 0-1
searching for 1, stops on 1-1 and erases it
searching for 2, stops on 3-7 and merges 1-1 with 3-7 into 1-7
searching for 3, stops on 3-7 and shrinks it to 4-7
searching for 4, stops on 3-7 and splits it to 3-3 and 5-7
searching for 5, stops on 3-7 and splits it to 3-4 and 6-7
searching for 6, stops on 3-7 and splits it to 3-5 and 7-7
searching for 7, stops on 3-7 and shrinks it to 3-6
searching for 8, stops on 10-12 and extends 3-7 to 3-8
searching for 9, stops on 10-12 and extends 10-12 to 9-12
searching for 10, stops on 10-12 and shrinks it to 11-12
searching for 11, stops on 10-12 and splits it to 10-10 and 12-12
searching for 12, stops on 10-12 and shrinks it to 10-11
searching for 13, stops on 14-18 and merges 10-12 with 14-18 into 10-18
searching for 14, stops on 14-18 and shrinks it to 15-18
searching for 15, stops on 14-18 and splits it to 14-14 and 16-18
searching for 16, stops on 14-18 and splits it to 14-15 and 17-18
searching for 17, stops on 14-18 and splits it to 14-16 and 18-18
searching for 18, stops on 14-18 and shrinks it to 14-17
searching for 19, stops on 20-20 and merges 14-18 with 20-20 into 14-20
searching for 20, stops on 20-20 and erases it
searching for 21, stops at end and adds 21-21
*/
void processFlip2 (OnRanges & onRanges, Instruction const& inst) {
    OnRanges::iterator iter = onRanges.begin ();
    // Find the range that includes this bit.
    while (iter != onRanges.end () && inst.flipBit > iter->lastBit) {
        ++iter;
    }
    // If we got all the way to the end of the list, add a new range at the end.
    if (iter == onRanges.end ()) {
        onRanges.push_back ({inst.flipBit, inst.flipBit});
    }
    // Or if it was the only one in a singleton range, it gets removed from the list.
    else if (iter->firstBit == inst.flipBit && iter->lastBit == inst.flipBit) {
        onRanges.erase (iter);
    }
    // Or if it was the first one in a larger range, shrink that range.
    else if (iter->firstBit == inst.flipBit && iter->lastBit > inst.flipBit) {
        ++(iter->firstBit);
    }
    // Or if it was the last one in a larger range, shrink that range.
    else if(iter->firstBit < inst.flipBit && iter->lastBit == inst.flipBit) {
        --(iter->lastBit);
    }
    // Or if it was in the middle of a range, split that range in two.
    else if(iter->firstBit < inst.flipBit && iter->lastBit > inst.flipBit) {
        int first = iter->firstBit;
        iter->firstBit = inst.flipBit + 1;
        onRanges.insert (iter, {first, inst.flipBit - 1});
    }
    // Or this means that the bit was off, and we are turning it on.
    else {
        OnRanges::iterator prev {iter};
        --prev;
        // If this has a range directly before and another directly after it, merge them.
        if (prev != onRanges.end () && prev->lastBit == inst.flipBit - 1 && iter->firstBit == inst.flipBit + 1) {
            prev->lastBit = iter->lastBit;
            onRanges.erase (iter);
        }
        // Or if this only has a range directly before it, extend it to include this bit.
        else if (prev != onRanges.end () && prev->lastBit == inst.flipBit - 1) {
            ++(prev->lastBit);
        }
        // Or if this only has a range directly after it, extend it to include this bit.
        else if (iter->firstBit == inst.flipBit + 1) {
            --(iter->firstBit);
        }
        // Or just make a new singleton range.
        else {
            onRanges.insert (iter, {inst.flipBit, inst.flipBit});
        }
    }
}


void processQuery2 (OnRanges & onRanges, Instruction const& inst) {
    int total = 0;
    OnRanges::iterator iter = onRanges.begin ();
    // Loop through every range that is before the end of the target
    while (iter != onRanges.end () && iter->firstBit <= inst.endBit) {
        // If there is an overlap, add to the total
        total += std::max (0, std::min (inst.endBit, iter->lastBit) - std::max (inst.startBit, iter->firstBit) + 1); 
        ++iter;
    }
    std::cout << total << "\n";
}

void solve3 (Instructions const& instructions) {
    OnRanges onRanges;
    for (Instruction const& inst : instructions) {
        if (inst.type == FLIP) {
            processFlip2 (onRanges, inst);
        }
        else {
            processQuery2 (onRanges, inst);
        }
    }
}

int main(int argc, char* argv[]) {
    Problem prob = readProblem ();
    //solve (prob);
    alternSolve (prob.instructions);
    //solve3 (prob.instructions);
    return 0;
}