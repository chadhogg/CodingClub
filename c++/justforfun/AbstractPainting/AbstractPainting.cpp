/// \file AbstractPainting.cpp
/// \author Chad Hogg
/// \brief Solution to https://open.kattis.com/problems/abstractpainting

#include <iostream>

/// A way to avoid typing unsigned long a lot of times.
using Num = unsigned long;

/// The modular arithmetic limit by which we report our answers.
const Num MODULO {1000000007};

/// \brief Gets the number of choices one can make when coloring the top-left square.
/// \return 18.
static Num getTopLeftSquareChoices () {
    const Num choicesForNorthSide {3UL};
    const Num choicesForWhichSideMatchesNorth {3UL};
    const Num choicesForOtherColorToUse {2UL};
    return choicesForNorthSide * choicesForWhichSideMatchesNorth * choicesForOtherColorToUse;
}

/// \brief Gets the number of choices one can make when coloring squares directly below the top-left square.
/// \return 6.
static Num getNotTopLeftSquareChoices () {
    const Num choicesForNorthSide {1UL};
    const Num choicesForWhichSideMatchesNorth {3UL};
    const Num choicesForOtherColorToUse {2UL};
    return choicesForNorthSide * choicesForWhichSideMatchesNorth * choicesForOtherColorToUse;
}

/// \brief Gets the number of choices one can make when coloring squares directly right of the top-level square.
/// \return 6.
static Num getTopNotLeftSquareChoices () {
    const Num choicesForEastSide {1UL};
    const Num choicesForWhichSideMatchesEast {3UL};
    const Num choicesForOtherColorToUse {2UL};
    return choicesForEastSide * choicesForWhichSideMatchesEast * choicesForOtherColorToUse;
}

/// \brief Gets the number of choices one can make when coloring square with both a northern and western neighbor whose colors have already been decided.
/// \return 2.
static Num getNotTopNotLeftSquareChoices () {
    const Num choicesForNorthSide {1UL};
    const Num choicesForEastSide {1UL};
    const Num ifNEMatchChoicesForOtherColor {2UL};
    const Num ifNEDontMatchChoicesForWhichMatchesN {2UL};
    return choicesForNorthSide * choicesForEastSide * (ifNEMatchChoicesForOtherColor / 2UL + ifNEDontMatchChoicesForWhichMatchesN / 2UL);
}

/// \brief Multiplies two numbers, maintaining our modulo invariant.
/// \param x One number.
/// \param y Another number.
/// \param m The modular arithmetic limit.
/// \return The product of x and y, modulo m.
static inline Num multiplyModulo (Num x, Num y, Num m) {
    return (x % m) * (y % m) % m;
}

/// \brief Reads some data from standard input.
/// \tparam T The type to be read.
/// \return The value that was read.
template<typename T>
T read() {
    T value;
    std::cin >> value;
    return value;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    unsigned int numCases {read<unsigned int> ()};
    for (unsigned int c {0U}; c < numCases; ++c) {
        unsigned int rows {read<unsigned int> ()};
        unsigned int cols {read<unsigned int> ()};
        Num paintings = 1UL;
        for (unsigned int row {0UL}; row < rows; ++row) {
            for (unsigned int col {0UL}; col < cols; ++col) {
                if (row == 0UL) {
                    if (col == 0UL) {
                        paintings = multiplyModulo (paintings, getTopLeftSquareChoices (), MODULO);
                    }
                    else {
                        paintings = multiplyModulo (paintings, getTopNotLeftSquareChoices (), MODULO);
                    }
                }
                else {
                    if (col == 0UL) {
                        paintings = multiplyModulo (paintings, getNotTopLeftSquareChoices (), MODULO);
                    }
                    else {
                        paintings = multiplyModulo (paintings, getNotTopNotLeftSquareChoices (), MODULO);
                    }
                }
            }
        }
        std::cout << paintings << "\n";
    }
    return 0;
}