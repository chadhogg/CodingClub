/// \file BuriedTreasure2.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/buriedtreasure2

// My own idea (which is too slow):
// 1: Start by dividing graph of (vertices=locations, edges=maps) into connected components.
// 2: If each connected component has a solution, the whole thing has a solution.
//    A: At each decision point, start by propagating any decisions that are forced (arc consistency).
//    B: Then make an arbitrary decision and backtrack to this point if it doesn't work out.
//       Bi: Make this decision on the variable that is most constraining.
//    C: Recurse steps A-B until a definitive answer has been found.

// Ultimate algorithm gleaned from Wikipedia:
// Aspvall, Plass & Tarjan's algorithm for solving 2-SAT.
//   Uses Kosaraju's algorithm finding strongly-connected components.
// Even with this very fast algorithm I still had to short-circuit as soon as evidence found
//   rather than just blindly creating all SCC and then checking them.

#define NDEBUG

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <list>

struct Map
{
    int first;
    int second;

    auto operator<=> (const Map& other) const = default;

    bool
    firstIsWrong (const std::unordered_map<int, bool>& decisions) const
    {
        return decisions.contains (abs (first)) && ((first > 0) != (decisions.at (abs (first))));
    }

    bool
    secondIsWrong (const std::unordered_map<int, bool>& decisions) const
    {
        return decisions.contains (abs (second)) && ((second > 0) != (decisions.at (abs (second))));
    }

    bool
    atLeastOneIsRight (const std::unordered_map<int, bool>& decisions) const
    {
        return (decisions.contains (abs (first)) && ((first > 0) == (decisions.at (abs (first)))))
            || (decisions.contains (abs (second)) && ((second > 0) == (decisions.at (abs (second)))));
    }

};

namespace std {
    template<>
    struct hash<Map> {
        inline size_t operator() (const Map& map) const
        {
            return map.first + 37 * map.second;
        }
    };
}


struct Problem
{
    std::vector<Map> maps;
    int numLocations;
};


Problem
readInput ()
{
    Problem prob;
    int numMaps;
    int tempA, tempB;
    std::cin >> numMaps >> prob.numLocations;
    assert (1 <= numMaps && numMaps <= 100000);
    assert (1 <= prob.numLocations && prob.numLocations <= 100000);
    prob.maps.reserve (prob.numLocations);
    while (prob.maps.size () < numMaps) {
        std::cin >> tempA >> tempB;
        assert (-prob.numLocations <= tempA && tempA <= prob.numLocations);
        assert (-prob.numLocations <= tempB && tempB <= prob.numLocations);
        prob.maps.push_back ({tempA, tempB});
    }
    return prob;
}


/// @brief Heuristically chooses a variable to search on next.
/// @param maps A set of maps that are not yet proven reasonable.
/// @param unassigned A set of variables that exist in maps and have not been decided.
/// @param assigned A map of locations to true (treasure) or false (trap).
/// @return The variable we should make a decision about next.
/// 
/// The idea behind this is that if we have already entered an impossible situation but have
///   not yet realized it, we want to discover it as soon as possible.
/// This idea is inspired by the Most Constrained Variable heuristic from CSP search.
int
chooseMostConstrainedVariable (const std::unordered_set<Map>& maps, const std::unordered_set<int>& unassigned, const std::unordered_map<int, bool> assigned)
{
    const int SCORE_MUST_HAVE_SPECIFIC_VALUE = 100;
    const int SCORE_WILL_FORCE_OTHER = 10;
    const int SCORE_USES = 1;
    int best = *unassigned.begin ();
    int bestScore = 0;
    for (int variable : unassigned) {
        int score = 0;
        for (const Map& map : maps) {
            if (abs (map.first) == variable) {
                if (map.secondIsWrong (assigned)) { score += SCORE_MUST_HAVE_SPECIFIC_VALUE; }
                else if (unassigned.contains (abs (map.second))) { score += SCORE_WILL_FORCE_OTHER; }
                else { score += SCORE_USES; }
            }
            else if (abs (map.second) == variable) {
                if (map.firstIsWrong (assigned)) { score += SCORE_MUST_HAVE_SPECIFIC_VALUE; }
                else if (unassigned.contains (abs (map.first))) { score += SCORE_WILL_FORCE_OTHER; }
                else { score += SCORE_USES; }
            }
        }
        if (score > bestScore) {
            bestScore = score;
            best = variable;
        }
    }
    return best;
}


bool
makeAtLeastOneDecision (const std::unordered_set<Map>& maps, const std::unordered_set<int>& unassigned, const std::unordered_map<int, bool> assigned);

/// @brief Continues a search after an arbitrary decision has been made.
/// @param maps A set of maps that are not yet proven reasonable.
/// @param unassigned A set of variables that exist in maps and have not been decided.
/// @param assigned A map of locations to true (treasure) or false (trap).
/// @param var An unassigned variable that we are going to assign.
/// @param value The value we are assigning it.
/// @return True if at least one extension of the current assigned makes all maps reasonable.
bool
trySomething (const std::unordered_set<Map>& maps, const std::unordered_set<int>& unassigned, const std::unordered_map<int, bool>& assigned, int var, bool value)
{
    assert (unassigned.contains (var));
    assert (!assigned.contains (var));
    std::unordered_set<int> newUnassigned = unassigned;
    newUnassigned.erase (var);
    std::unordered_map<int, bool> newAssigned = assigned;
    newAssigned[var] = value;
    std::unordered_set<Map> newMaps;
    for (const Map& map : maps) {
        // The decision we made is going to force a map to be unreasonable, so give up.
        if (map.firstIsWrong (newAssigned) && map.secondIsWrong (newAssigned)) { return false; }
        // Even after our decision, we still don't know whether or not this map is reasonable, so keep it.
        else if (!map.atLeastOneIsRight (newAssigned)) { newMaps.insert (map); }
        // This map will be reasonable no matter what future decisions are made, so skip it.
        else {}
    }

    // If every map has been proven reasonable, we are done.
    if (newMaps.empty ()) { return true; }
    // Otherwise, keep searching.
    else { return makeAtLeastOneDecision (newMaps, newUnassigned, newAssigned); }
}

/// @brief Makes any decisions that can be committed to with *no backtracking*.
/// @param maps A set of maps that are not yet proven reasonable.
/// @param unassigned A set of variables that exist in maps and have not been decided.
/// @param assigned A map of locations to true (treasure) or false (trap).
/// @return True if it is still possible to solve, or false if impossible.
/// @note All parameters are in/out, and the primary purpose of this function is to move
///   variables from unassigned to assigned while decreasing the number of maps about which
///   decisions still need to be made.
/// @note This is intended to be called before each search step.
/// @note This idea is inspired by arc consistency checking in CSP search.
bool
makeForcedDecisions (std::unordered_set<Map>& maps, std::unordered_set<int>& unassigned, std::unordered_map<int, bool>& assigned)
{
    // Every map might allow us to make a forced decision.
    std::unordered_set<Map> todo = maps;
    while (!todo.empty ()) {
        Map current = *todo.begin ();
        todo.erase (current);
        // If this map is already proven unreasonable, give up.
        if (current.firstIsWrong (assigned) && current.secondIsWrong (assigned)) { return false; }
        // If this map is already proven reasonable, no need to keep thinking about it.
        else if (current.atLeastOneIsRight (assigned)) { maps.erase (current); }
        // If this map is wrong about first location, we can force it to be right about second.
        else if (current.firstIsWrong (assigned)) {
            unassigned.erase (abs (current.second));
            assigned[abs (current.second)] = (current.second > 0);
            // Any other map involving the recently-assigned variable needs to be re-evaluated.
            for (const Map& map : maps) {
                if (abs (map.first) == abs (current.second)) { todo.insert (map); }
                else if (abs (map.second) == abs (current.second)) { todo.insert (map); }
            }
        }
        // If this map is wrong about second location, we can force it to be right about first.
        else if (current.secondIsWrong (assigned)) {
            unassigned.erase (abs (current.first));
            assigned[abs (current.first)] = (current.first > 0);
            // Any other map involving the recently-assigned variable needs to be re-evaluated.
            for (const Map& map : maps) {
                if (abs (map.first) == abs (current.first)) { todo.insert (map); }
                else if (abs (map.second) == abs (current.first)) { todo.insert (map); }
            }
        }
    }
    // We never encountered a proven-unreasonable map.
    return true;
}

bool
hasCycle (const std::unordered_set<Map>& maps)
{
    std::unordered_set<int> vars;
    for (const Map& map : maps) {
        vars.insert (abs (map.first));
        vars.insert (abs (map.second));
    }
    while (!vars.empty ()) {
        int var = *vars.begin ();
        vars.erase (var);
        std::unordered_set<Map> usedMaps;
        std::unordered_set<int> visited;
        std::unordered_set<int> frontier;
        frontier.insert (var);
        while (!frontier.empty ()) {
            int current = *frontier.begin ();
            frontier.erase (current);
            for (const Map& map : maps) {
                if (!usedMaps.contains (map)) {
                    // We have discovered a second way to get from one variable to another -- cycle!
                    if ((abs (map.first) == current && visited.contains (abs (map.second)))
                        || ((abs (map.second) == current && visited.contains (abs (map.first))))) {
                        return true;
                    }
                    // We have found a way to another variable.
                    else if (abs (map.first) == current) {
                        frontier.insert (abs (map.second));
                        usedMaps.insert (map);
                    }
                    // We have found a way to another variable.
                    else if (abs (map.second) == current) {
                        frontier.insert (abs (map.first));
                        usedMaps.insert (map);
                    }
                }
            }
            visited.insert (current);
        }
        for (int var : visited) { vars.erase (var); }
    }
    return true;
}

/// @brief Searches for an assignment that satisfies the CSP.
/// @param maps A set of maps that are not yet proven reasonable.
/// @param unassigned A set of variables that exist in maps and have not been decided.
/// @param assigned A map of locations to true (treasure) or false (trap).
/// @return True if some extension of assigned makes all maps reasonable, false otherwise.
bool
makeAtLeastOneDecision (const std::unordered_set<Map>& maps, const std::unordered_set<int>& unassigned, const std::unordered_map<int, bool> assigned)
{
    std::unordered_set<Map> m = maps;
    std::unordered_set<int> u = unassigned;
    std::unordered_map<int, bool> a = assigned;

    // Start by making any decisions that do not need to be backtracked on.
    // If we already can't be solved, give up.
    if (!makeForcedDecisions (m, u, a)) { return false; }
    // If every map has been proven reasonable, we are done.    
    if (m.empty ()) { return true; }
    // If no cycles, satisfiability is proven.  See https://en.wikipedia.org/wiki/Local_consistency#Consistency_and_satisfiability
    // But checking this actually slowed it down!
    //if (!hasCycle (m)) { return true; }

    // Pick a location and try extending assignment either possible way.
    int variable = chooseMostConstrainedVariable (m, u, a);
    return trySomething (m, u, a, variable, true)
        || trySomething (m, u, a, variable, false);
}


/// @brief Splits the problem up into connected components.
/// @param allMaps All the maps that need to be verified.
/// @param allVars All the locations that are referred to by maps.
/// @return A vector of problems that can be solved independently.
std::vector<std::pair<std::unordered_set<Map>, std::unordered_set<int>>>
divideProblem (const std::unordered_set<Map>& allMaps, const std::unordered_set<int>& allVars)
{
    std::vector<std::pair<std::unordered_set<Map>, std::unordered_set<int>>> components;
    std::unordered_set<int> unusedVars = allVars;
    std::unordered_set<Map> unusedMaps = allMaps;
    while (!unusedVars.empty ()) {
        int start = *unusedVars.begin ();
        unusedVars.erase (start);
        std::unordered_set<int> usedVars;
        std::unordered_set<int> frontier;
        frontier.insert (start);
        while (!frontier.empty ()) {
            int current = *frontier.begin ();
            frontier.erase (current);
            for (const Map& map : unusedMaps) {
                if (abs (map.first) == current && !usedVars.contains (abs (map.second))) {
                    frontier.insert (abs (map.second));
                }
                if (abs (map.second) == current && !usedVars.contains (abs (map.first))) {
                    frontier.insert (abs (map.first));
                }
            }
            usedVars.insert (current);
        }
        std::unordered_set<Map> usedMaps;
        for (const Map& map : unusedMaps) {
            if (usedVars.contains (abs (map.first))) {
                assert (usedVars.contains (abs (map.second)));
                usedMaps.insert (map);
            }
            else {
                assert (!usedVars.contains (abs (map.second)));
            }
        }
        for (const Map& map : usedMaps) {
            unusedMaps.erase (map);
        }
        for (int var : usedVars) {
            unusedVars.erase (var);
        }
        components.push_back ({usedMaps, usedVars});
    }
    return components;
}


bool
solve (const Problem& prob)
{
    std::unordered_set<Map> maps;
    for (const Map& map : prob.maps) {
        maps.insert (map);
    }
    std::unordered_set<int> unassigned;
    for (int i = 1; i <= prob.numLocations; ++i) {
        unassigned.insert (i);
    }
    std::vector<std::pair<std::unordered_set<Map>, std::unordered_set<int>>> components = divideProblem (maps, unassigned);
    for (std::size_t index = 0; index < components.size (); ++index) {
        std::unordered_map<int, bool> assigned;
        if (!makeAtLeastOneDecision (components[index].first, components[index].second, assigned)) {
            return false;
        }
    }
    return true;
}


struct ImplicationGraph
{
    std::unordered_set<int> vertices;
    std::unordered_map<int, std::unordered_set<int>> edges;
    std::unordered_map<int, std::unordered_set<int>> transposed;
};

ImplicationGraph
buildImplicationGraph (const Problem& prob)
{
    ImplicationGraph graph;
    for (int i = 1; i <= prob.numLocations; ++i) {
        graph.vertices.insert (i);
        graph.vertices.insert (-i);
        graph.edges[i] = {};
        graph.edges[-i] = {};
        graph.transposed[i] = {};
        graph.transposed[-i] = {};
    }
    for (const Map& map : prob.maps) {
        graph.edges[-map.first].insert (map.second);
        graph.edges[-map.second].insert (map.first);
        graph.transposed[map.second].insert (-map.first);
        graph.transposed[map.first].insert (-map.second);
    }
    return graph;
}

void
fCCVisit (const ImplicationGraph& graph, std::unordered_set<int>& visited, std::list<int>& orderedList, int u)
{
    if (!visited.contains (u)) {
        visited.insert (u);
        for (int v : graph.edges.at (u)) {
            fCCVisit (graph, visited, orderedList, v);
        }
        orderedList.push_front (u);
    }
}

bool
fCCAssign (const ImplicationGraph& graph, std::unordered_map<int, int>& assignments, int u, int root)
{
    if (!assignments.contains (u)) {
        if (assignments.contains (-u) && assignments[-u] == root) { return false; }
        assignments[u] = root;
        for (int v : graph.transposed.at (u)) {
            if (!fCCAssign (graph, assignments, v, root)) { return false; }
        }
    }
    return true;
}

bool
sCCsNotContradictory (const ImplicationGraph& graph)
{
    std::vector<std::unordered_set<int>> components;
    std::unordered_set<int> visited;
    std::list<int> orderedList;
    std::unordered_map<int, int> assignments;
    for (int u : graph.vertices) {
        fCCVisit (graph, visited, orderedList, u);
    }
    for (int u : orderedList) {
        if (!fCCAssign (graph, assignments, u, u)) { return false; }
    }
    return true;
}

bool
aPVSatisfiable (const Problem& prob)
{
    ImplicationGraph graph = buildImplicationGraph (prob);
    return sCCsNotContradictory (graph);
}

int
main()
{
    Problem prob = readInput ();
    if (aPVSatisfiable (prob)) {
        std::cout << "YES\n";
    }
    else {
        std::cout << "NO\n";
    }
    return 0;
}