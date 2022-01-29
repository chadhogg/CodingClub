/// \file SmoothieStand.cpp
/// \brief A solution to https://open.kattis.com/problems/smoothiestand
/// \author Chad Hogg
/// \version 2022-01-27

#include <iostream>
#include <vector>
#include <climits>

struct Recipe {
    std::vector<int> ingredientsNeeded;
    int price;
};

struct Problem {
    int numIngredients;
    int numRecipes;
    std::vector<int> ingredientsOnHand;
    std::vector<Recipe> recipes;
};

Problem readProblem () {
    Problem prob;
    int temp;
    std::cin >> prob.numIngredients >> prob.numRecipes;
    for (int i = 0; i < prob.numIngredients; ++i) {
        std::cin >> temp;
        prob.ingredientsOnHand.push_back (temp);
    }
    for (int i = 0; i < prob.numRecipes; ++i) {
        Recipe r;
        for (int j = 0; j < prob.numIngredients; ++j) {
            std::cin >> temp;
            r.ingredientsNeeded.push_back (temp);
        }
        std::cin >> r.price;
        prob.recipes.push_back (r);
    }
    return prob;
}

long findMaxRevenue (Problem const& prob) {
    long best = 0;
    for (Recipe const& r : prob.recipes) {
        long quantity = INT_MAX;
        for (int i = 0; i < prob.numIngredients; ++i) {
            if (r.ingredientsNeeded[i] != 0) {
                quantity = std::min (quantity, (long)prob.ingredientsOnHand[i] / r.ingredientsNeeded[i]);
            }
        }
        best = std::max (best, quantity * r.price);
    }
    return best;
}

int main(int argc, char* argv[]) {
    Problem prob = readProblem ();
    std::cout << findMaxRevenue (prob) << "\n";
    return 0;
}