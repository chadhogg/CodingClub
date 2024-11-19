/// \file Pianissimo.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/pianissimo
/// \note Thanks to Marshall Feng / Evan Magill for suggesting that the answer might not fit in an int.

#include <iostream>
#include <string>
#include <cassert>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

int
strToVal (const std::string& str) {
    if (str == "ppp") { return 0; }
    if (str == "pp") { return 1; }
    if (str == "p") { return 2; }
    if (str == "mp") { return 3; }
    if (str == "mf") { return 4; }
    if (str == "f") { return 5; }
    if (str == "ff") { return 6; }
    if (str == "fff") { return 7; }
    assert (false);
}

// Somehow, this n^2 algorithm is too slow.
int
firstAttempt (const std::vector<int>& notes, const std::vector<int>& dynamics, int numNotes) {
    int badCount = 0;
    for (int i = 0; i < numNotes; i++) {
        for (int j = i + 1; j < numNotes; j++) {
            if (dynamics[j] > dynamics[i] && notes[j] <= notes[i]) { ++badCount; }
            else if (dynamics[j] < dynamics[i] && notes[j] >= notes[i]) { ++badCount; }
        }
    }
    return badCount;
}

// This seems fast enough, but produces the wrong answer on Test Case 12.
long
secondAttempt (const std::vector<int>& notes, const std::vector<int>& dynamics, int numNotes) {
    // Make a separate list of the notes in each dynamic range.
    std::map<int, std::vector<int>> groups;
    for (int i = 0; i < 8; ++i) {
        std::vector<int> empty;
        empty.reserve (numNotes);
    }
    for (int i = 0; i < numNotes; ++i) {
        groups[dynamics[i]].push_back (notes[i]);
    }
    // Sort the notes in each dynamic group from softest to loudest.
    for (int i = 0; i < 8; ++i) {
        std::sort (groups[i].begin (), groups[i].end ());
    }
    long badCount = 0;
    // Compare each pair of different dynamic groups once.
    for (int g1 = 0; g1 < 8; ++g1) {
        for (int g2 = g1 + 1; g2 < 8; ++g2) {
            // Make a single pass through both the softer group and the louder group.
            int index1 = 0, index2 = 0;
            while (index1 < groups[g1].size () && index2 < groups[g2].size ()) {
                // The supposedly-softer note really is softer than all unprocessed supposedly-louder notes,
                //   so there should be no more pairs involving it.
                if (groups[g1][index1] < groups[g2][index2]) {
                    ++index1;
                }
                // The supposedly-softer note is not softer than the supposedly-louder note.
                // Because they are sorted, none of the other unprocessed supposedly-softer notes can be either.
                // So make a pair for each unprocessed supposedly-softer note with this supposedly-louder note.
                // Then we don't need to think about that supposedly-louder note anymore.
                else {
                    badCount += groups[g1].size () - index1;
                    ++index2;
                }
            }
        }
    }
    return badCount;
}

int
main (int argc, char* argv[])
{
    int numNotes, numDynamics;
    std::cin >> numNotes >> numDynamics;
    std::vector<int> notes;
    notes.reserve (numNotes);
    int x;
    for (int i = 0; i < numNotes; ++i) {
        std::cin >> x;
        notes.push_back (x);
    }

    std::vector<std::pair<int, std::string>> inputDynamics;
    inputDynamics.reserve (numDynamics);
    std::string s;
    for (int i = 0; i < numDynamics; ++i) {
        std::cin >> x >> s;
        inputDynamics.push_back ({x, s});
    }

    // Make a parallel vector to notes, storing the desired dynamic at each note.
    std::vector<int> dynamics;
    dynamics.reserve (numNotes);
    auto it = inputDynamics.begin ();
    int curVal = -1;
    for (int i = 1; i <= numNotes; ++i) {
        if (i == it->first) { curVal = strToVal (it->second); ++it; }
        dynamics.push_back (curVal);
    }

    // Try (but fail!) to solve the problem.
    std::cout << secondAttempt (notes, dynamics, numNotes) << "\n";
    return 0;
}
