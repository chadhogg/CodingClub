/// \file ItemSelection.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/itemselection

// Assumptions:
// 1: It will always be optimal to completely fix a page before leaving it.
// 2: Every page can be optimally fixed by either:
//   2a: Deselect All followed by clicking those that are wanted.
//   2b: Select All followed by clicking those that are not wanted.
//   2c: Clicking the elements that are in one set but not the other.
// 3: There's no reason to visit a page if neither it nor any beyond it need fixing.
// 4: It will always be optimal to move to the closer edge first, then double back to the further edge.

#include <iostream>
#include <cassert>
#include <string>
#include <set>
#include <algorithm>

struct Problem
{
    int n, m, s, p, q;
    std::set<int> preselected;
    std::set<int> want;
};

Problem
init ()
{
    Problem prob;
    int x;
    std::cin >> prob.n >> prob.m >> prob.s >> prob.p >> prob.q;
    --prob.s;
    for (int i = 0; i < prob.p; ++i) { std::cin >> x; prob.preselected.insert (x - 1); }
    for (int i = 0; i < prob.q; ++i) { std::cin >> x; prob.want.insert (x - 1); }
    return prob;
}

int
numToEqualize (const std::set<int>& a, const std::set<int>& b)
{
    if (a == b) { return 0; }
    std::set<int> onlyA, onlyB;
    std::set_difference (a.cbegin (), a.cend (), b.cbegin (), b.cend (), std::inserter (onlyA, onlyA.begin ()));
    std::set_difference (b.cbegin (), b.cend (), a.cbegin (), a.cend (), std::inserter (onlyB, onlyB.begin ()));

    // Try manually deselecting a \ b and selecting b \ a
    return onlyA.size () + onlyB.size ();
}

int
numToSolvePage (const Problem& prob, int pageNum)
{
    std::set<int> selectedOnPage;
    std::set<int> wantOnPage;
    std::set<int> all;
    std::set<int> none;
    for (int x = pageNum * prob.m; x < (pageNum + 1) * prob.m && x < prob.n; ++x) {
        all.insert (x);
        if (prob.preselected.count (x) != 0) { selectedOnPage.insert (x); }
        if (prob.want.count (x) != 0) { wantOnPage.insert (x); }
    }

    int best = numToEqualize (selectedOnPage, wantOnPage);
    best = std::min (best, numToEqualize (none, wantOnPage) + 1);
    best = std::min (best, numToEqualize (all, wantOnPage) + 1);
    return best;
}

int
main ()
{
    Problem prob = init ();
    std::vector<int> pageWork;
    for (int page = 0; page < prob.n / prob.m + (prob.n % prob.m == 0 ? 0 : 1); ++page) {
        pageWork.push_back (numToSolvePage (prob, page));
//        std::cout << "Page " << page << " has " << numToSolvePage (prob, page) << "\n";
    }
    int leftMoves = 0;
    int totalWork = pageWork[prob.s];
    bool furtherAwayPageNeedsReached = false;
    for (int page = 0; page < prob.s; ++page) {
        if (pageWork[page] > 0 || furtherAwayPageNeedsReached) {
            totalWork += pageWork[page];
            leftMoves += 1;
            furtherAwayPageNeedsReached = true;
        }
    }
    int rightMoves = 0;
    furtherAwayPageNeedsReached = false;
    for (int page = prob.n / prob.m + (prob.n % prob.m == 0 ? 0 : 1); page > prob.s; --page) {
        if (pageWork[page] > 0 || furtherAwayPageNeedsReached) {
            totalWork += pageWork[page];
            rightMoves += 1;
            furtherAwayPageNeedsReached = true;
        }
    }
//    std::cout << "total work without moves: " << totalWork << "\n";
//    std::cout << "total left moves: " << leftMoves << "\n";
//    std::cout << "total right moves: " << rightMoves << "\n";
    if (leftMoves == 0 && rightMoves == 0) {
        totalWork += 0;
    }
    else if (leftMoves == 0 && rightMoves > 0) {
        totalWork += rightMoves;
    }
    else if (leftMoves > 0 && rightMoves == 0) {
        totalWork += leftMoves;
    }
    else {
        totalWork += 2 * std::min (leftMoves, rightMoves) + std::max (leftMoves, rightMoves);
    }
    std::cout << totalWork << "\n";
    return 0;
}
