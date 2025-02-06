/// \file DiverseContest.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/diversecontest

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>

struct Problem
{
    std::set<std::string> topics;
};

struct Contest
{
    int probUsed;
    std::vector<Problem> problems;
};

Contest
readInput () 
{
    int probAvail, probUsed;
    std::cin >> probAvail >> probUsed;
    std::vector<Problem> problems;
    problems.reserve (probAvail);
    for (int i = 0; i < probAvail; ++i) {
        int numTopics;
        std::cin >> numTopics;
        std::string topic;
        Problem p {{}};
        for (int k = 0; k < numTopics; ++k) {
            std::cin >> topic;
            p.topics.insert (topic);
        }
        problems.push_back (p);
    }
    return {probUsed, problems};
}

Contest
removeIrrelevantTopics(const Contest& original)
{
    std::map<std::string, int> counts;
    for (const Problem& p : original.problems) {
        for (const std::string& s : p.topics) {
            if (counts.count (s) == 0) { counts[s] = 0; }
            ++counts[s];
        }
    }
    Contest copy;
    copy.probUsed = original.probUsed;
    copy.problems.reserve (original.problems.size ());
    for (const Problem& p : original.problems) {
        Problem modified ({});
        for (const std::string& s : p.topics) {
            if (counts[s] >= original.probUsed / 2) {
                modified.topics.insert (s);
            }
        }
        copy.problems.push_back (p);
    }
    return copy;
}

struct PartialSolution
{
    std::vector<bool> used;
    std::map<std::string, int> topicCounts;
    int numUsed;
};

long
bruteForce(const Contest& contest)
{
    long count = 0;
    std::vector<PartialSolution> frontier;
    frontier.push_back ({{}, {}, 0});
    while (!frontier.empty ()) {
        PartialSolution current = frontier.back ();
        frontier.pop_back ();
        assert (current.numUsed <= contest.probUsed);
        if (current.numUsed == contest.probUsed) {
            // This is a valid selection of problems.
            ++count;
            continue;
        }
        const Problem& nextProb = contest.problems.at (current.used.size ());
        PartialSolution without = current;
        without.used.push_back (false);
        if (contest.problems.size () - without.used.size () >= contest.probUsed - without.numUsed) {
            // There are still enough problems to select without this one.
            frontier.push_back (without);
        }
        PartialSolution with = current;
        with.used.push_back (true);
        ++with.numUsed;
        bool allGood = true;
        for (const std::string& s : nextProb.topics) {
            if (with.topicCounts.count (s) == 0) { with.topicCounts[s] = 0; }
            ++with.topicCounts[s];
            if (with.topicCounts[s] > contest.probUsed / 2) {
                // Too many problems based on this topic, so this won't work.
                allGood = false;
            }
        }
        if (allGood) {
            frontier.push_back (with);
        }
    }
    return count;
}

int
main()
{
    Contest contest = readInput ();
    // not needed (or helpful with current algorithm)
    //Contest minimized = removeIrrelevantTopics (contest);
    std::cout << bruteForce (contest) << "\n";
    return 0;
}