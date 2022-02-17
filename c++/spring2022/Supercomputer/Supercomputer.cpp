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

int main(int argc, char* argv[]) {
    Problem prob = readProblem ();
    solve (prob);
    return 0;
}