/// \file RacingAlphabet.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/racingalphabet

#include <string>
#include <iostream>
#include <cmath>
#include <iomanip>

/// The letters that appear in the circle, in order.
const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ '";
/// The number of symbols that appear in the circle.
const int NUM_SYMBOLS = ALPHABET.length ();
/// The diameter of the circle.
const double DIAMETER = 60.0;
/// The player's running speed in feet/second.
const double RUN_SPEED = 15.0;
/// The player's picking up papers speed in papers/second.
const double PICKUP_SPEED = 1.0;
/// The circumference of the track.
const double CIRCUMFERENCE = M_PI * DIAMETER;
/// The distance that the player needs to travel for each move from one symbol to another.
const double DISTANCE_PER_STEP = CIRCUMFERENCE / NUM_SYMBOLS;

/// \brief Reads some data from standard input.
/// \tparam T The type to be read.
/// \return The value that was read.
template<typename T>
T read() {
    T value;
    std::cin >> value;
    return value;
}

/// \brief Computes the time a player will expend to run an aphorism, in seconds. 
/// \param[in] str The aphorism to run.
/// \return The time it will take to run that aphorism, in seconds.
double computeTime (const std::string& str) {
    unsigned int stepsTaken {0U};
    int prevPos = ALPHABET.find (str[0]);

    for (unsigned int index {1U}; index < str.length(); ++index) {
        int curPos = ALPHABET.find (str[index]);
        int forwardDist {(NUM_SYMBOLS + curPos - prevPos) % NUM_SYMBOLS};
        int reverseDist {(NUM_SYMBOLS + prevPos - curPos) % NUM_SYMBOLS};
        stepsTaken += std::min (forwardDist, reverseDist);
        prevPos = curPos;
    }
    return DISTANCE_PER_STEP * stepsTaken / RUN_SPEED + str.length() / PICKUP_SPEED;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    const unsigned int numStrings = read<unsigned int> ();
    std::ws (std::cin);
    for (unsigned int i {0U}; i < numStrings; ++i) {
        std::string aphorism;
        std::getline (std::cin, aphorism);
        std::cout << std::fixed << std::setprecision(10) << computeTime(aphorism) << "\n";
    }
    return 0;
}