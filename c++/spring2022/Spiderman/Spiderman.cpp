/// \file Spiderman.cpp
/// \brief A solution to https://open.kattis.com/problems/spiderman
/// \author Chad Hogg
/// \version 2022-02-03

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using Scenario = std::vector<int>;
using Problem = std::vector<Scenario>;

Problem readProblem() {
    Problem prob;
    int numScenarios;
    int numDistances;
    int distance;
    std::cin >> numScenarios;
    for (int i = 0; i < numScenarios; ++i) {
        Scenario scen;
        std::cin >> numDistances;
        for (int j = 0; j < numDistances; ++j) {
            std::cin >> distance;
            scen.push_back (distance);
        }
        prob.push_back (scen);
    }
    return prob;
}

struct PartialSolution {
    std::vector<bool> up;
    int maxHeight;
    int currentHeight;
};

std::ostream & operator<< (std::ostream & out, PartialSolution const& p) {
    for (bool b : p.up) {
        out << (b ? 'U' : 'D');
    }
    out << " " << p.currentHeight << " (" << p.maxHeight << ")";
    return out;
}

template<typename T>
std::ostream & operator<< (std::ostream & out, std::vector<T> const& v) {
    out << "[ ";
    for (T const& val : v) {
        out << val << " ";
    }
    out << "]";
    return out;
}

PartialSolution const* findLowestSolution (Scenario const& scen, PartialSolution const* start) {
    //std::cout << " Considering " << *start << "\n";
    if(scen.size () == start->up.size ()) {
        if (start->currentHeight == 0) { return start; }
        else { return NULL; }
    }
    else {
        PartialSolution * goingDown = new PartialSolution (*start);
        goingDown->up.push_back (false);
        goingDown->currentHeight -= scen[start->up.size ()];
        PartialSolution * goingUp = new PartialSolution (*start);
        goingUp->up.push_back (true);
        goingUp->currentHeight += scen[start->up.size ()];
        goingUp->maxHeight = std::max (goingUp->maxHeight, goingUp->currentHeight);
        PartialSolution const * solution = NULL;
        if (goingDown->currentHeight >= 0) {
            solution = findLowestSolution (scen, goingDown);
        }
        if (solution == NULL) {
            solution = findLowestSolution (scen, goingUp);
        }
        else {
            PartialSolution const * alt = findLowestSolution (scen, goingUp);
            if (alt != NULL && solution->maxHeight > alt->maxHeight) { solution = alt; }
        }
        return solution;
    }
}

void solveScenarioBruteForce (Scenario const& scen) {
    PartialSolution const* solution = findLowestSolution (scen, new PartialSolution {{}, 0, 0});
    if (solution == NULL) {
        std::cout << "IMPOSSIBLE";
    }
    else {
        for (bool b : solution->up) {
            if (b) { std::cout << "U"; }
            else { std::cout << "D"; }
        }
    }
    std::cout << "\n";
}

void solveScenarioSmart (Scenario const& scen) {
    std::vector<int> remainingTotal;
    remainingTotal.resize (scen.size ());
    for (int index = (int)scen.size () - 2; index >= 0; --index) {
        remainingTotal[index] = remainingTotal[index + 1] + scen[index + 1];
    }
    std::unordered_map<int, PartialSolution *> solutions;
    solutions.insert ({0, new PartialSolution {{}, 0, 0}});
    for (int index = 0; index < scen.size (); ++index) {
        std::unordered_map<int, PartialSolution *> nextSolutions;

        for (std::pair<int, PartialSolution *> pair : solutions) {
            PartialSolution* soFar = pair.second;
            {
                PartialSolution* afterUp = new PartialSolution (*soFar);
                afterUp->up.push_back (true);
                afterUp->currentHeight += scen[index];
                afterUp->maxHeight = std::max (afterUp->maxHeight, afterUp->currentHeight);
                if (afterUp->currentHeight <= remainingTotal[index]) {
                    // (otherwise, it wouldn't be possible to get down from this height)
                    if (nextSolutions.count (afterUp->currentHeight) == 0) {
                        nextSolutions.insert ({afterUp->currentHeight, afterUp});
                    }
                    else if (afterUp->maxHeight < nextSolutions[afterUp->currentHeight]->maxHeight) {
                        nextSolutions[afterUp->currentHeight] = afterUp;
                    }
                    else {
                        delete afterUp;
                    }
                }
                else {
                    delete afterUp;
                }
            }
            {
                PartialSolution * afterDown = new PartialSolution (*soFar);
                afterDown->up.push_back (false);
                afterDown->currentHeight -= scen[index];
                if (afterDown->currentHeight >= 0) {
                    // (otherwise, he would be in the ground)
                    if (nextSolutions.count (afterDown->currentHeight) == 0) {
                        nextSolutions.insert ({afterDown->currentHeight, afterDown});
                    }
                    else if (afterDown->maxHeight < nextSolutions[afterDown->currentHeight]->maxHeight) {
                        nextSolutions[afterDown->currentHeight] = afterDown;
                    }
                    else {
                        delete afterDown;
                    }
                }
                else {
                    delete afterDown;
                }
            }
        }


        for (std::pair<int, PartialSolution *> const& pair : solutions) {
            delete pair.second;
        }
        solutions = std::move (nextSolutions);
    }

    if(solutions.empty ()) {
        std::cout << "IMPOSSIBLE\n";
    }
    else {
         for (bool b : solutions[0]->up) {
            if (b) { std::cout << "U"; }
            else { std::cout << "D"; }
        }
        std::cout << "\n";   
    }
}

int main(int argc, char* argv[]) {
    Problem prob = readProblem ();
    for (Scenario const& scen : prob) {
        solveScenarioSmart (scen);
    }
    return 0;
}