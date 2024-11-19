/// \file OptimizedCheating.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/optimizedcheating
/// \note Thanks to Marshall Dier Feng, who sped it up by improving the way I was remembering how we
///   reached a state, for printing the sequence of operations at the end.

#include <iostream>
#include <string>
#include <cassert>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using Currency = int;
using OpType = char;
using Operation = std::pair<OpType, int>;

void
printSolutionAndExit (std::list<int> indexes) {
    std::cout << indexes.size () << "\n";
    for (int x : indexes) {
        std::cout << x + 1 << "\n";
    }
    exit (EXIT_SUCCESS);
}

int
main (int argc, char* argv[])
{
    int numSlots, numOps, currencySlot;
    Currency currencyValue;
    std::cin >> numSlots >> numOps >> currencySlot;
    // The values stored in the other memory locations.
    std::unordered_set<int> badNumbers;
    std::vector<Operation> operations;
    int x;
    OpType s;
    for (int i = 1; i <= numSlots; ++i) {
        std::cin >> x;
        if (i == currencySlot) {
            currencyValue = x;
        }
        else {
            badNumbers.insert (x);
        }
    }
    for (int i = 1; i <= numOps; ++i) {
        std::cin >> s >> x;
        operations.push_back ({s, x});
    }

    if (badNumbers.count (currencyValue) == 0) { printSolutionAndExit ({}); }

    std::unordered_set<Currency> explored;
    std::unordered_map<Currency, Currency> last;
    std::unordered_map<Currency, int> opIndex;
    std::list<Currency> frontier;
    frontier.push_back (currencyValue);

    // Breadth-first graph search.
    // Branching factor can be very high, but there are at most 10^4 reachable non-goal states.
    while (!frontier.empty ()) {
        Currency current = frontier.front ();
        frontier.pop_front ();
        // New state for each valid operation that leads to a not-yet-seen state.
        for (int o = 0; o < operations.size (); ++o) {
            const Operation& op = operations.at (o);
            Currency newValue = current;
            if (op.first == '+') {
                newValue += op.second;
            }
            else if (op.first == '-') {
                newValue -= op.second;
            }
            else if (op.first == '*') {
                newValue *= op.second;
            }
            else {
                assert (op.first == '/');
                newValue /= op.second;
            }
            // Only attempt moves that result in non-negative states.
            if (newValue >= 0) {
                // If we reached a goal state, stop.
                if (badNumbers.count (newValue) == 0) {
                    std::list<int> answer;
                    answer.push_front(o);
                    while (current != currencyValue)
                    {
                        answer.push_front(opIndex[current]);
                        current = last[current];
                    }
                    printSolutionAndExit (answer);
                }
                // If this is the first time finding this state, add it to frontier.
                if (explored.count (newValue) == 0) {
                    explored.insert (newValue);
                    frontier.push_back (newValue);
                    last[newValue] = current;
                    opIndex[newValue] = o;
                }
            }
        }
    }
    std::cout << "-1\n";
    return 0;
}