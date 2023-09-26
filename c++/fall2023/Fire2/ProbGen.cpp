/// \file ProbGen.cpp
/// \author Chad Hogg
/// \brief A program that generates problems in the format of https://open.kattis.com/problems/fire2

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

const int NUM_TEST_CASES = 100;
const int MIN_SIZE = 1;
const int MAX_SIZE = 1000;

const char ROOM = '.';
const char WALL = '#';
const char START = '@';
const char FIRE = '*';

using Problem = std::vector<std::vector<char>>;

std::mt19937 generator (std::chrono::steady_clock::now ().time_since_epoch ().count ());

Problem
gen_problem ()
{
    std::uniform_int_distribution<int> sizesDist (MIN_SIZE, MAX_SIZE); 
    std::uniform_int_distribution<int> percentDist (0, 100);
    std::normal_distribution<double> wallDist (15.0, 10.0);
    std::normal_distribution<double> fireDist (15.0, 10.0);
    int w = sizesDist (generator);
    int h = sizesDist (generator);
    int wallProb = (int)wallDist (generator);
    int fireProb = (int)fireDist (generator);
    std::uniform_int_distribution<int> heightDist (0, h - 1);
    std::uniform_int_distribution<int> widthDist (0, w - 1);

    Problem problem;
    for (int i = 0; i < h; ++i) {
        problem.push_back ({});
        for (int j = 0; j < w; ++j) {
            if (percentDist (generator) < wallProb) {
                problem.back ().push_back (WALL);
            }
            else if (percentDist (generator) < fireProb) {
                problem.back ().push_back (FIRE);
            }
            else {
                problem.back ().push_back (ROOM);
            }
        }
    }

    problem[heightDist (generator)][widthDist (generator)] = START;
    return problem;
}

void
print_problem (const Problem& problem)
{
    std::cout << problem.at (0).size () << " " << problem.size () << "\n";
    for (int i = 0; i < problem.size (); ++i) {
        for (int j = 0; j < problem.at (0).size (); ++j) {
            std::cout << problem.at (i).at (j);
        }
        std::cout << "\n";
    }
}

int
main (int argc, char* argv[])
{
    std::cout << NUM_TEST_CASES << "\n";
    for (int i = 0; i < NUM_TEST_CASES; ++i) {
        print_problem (gen_problem ());
    }
    return 0;
}