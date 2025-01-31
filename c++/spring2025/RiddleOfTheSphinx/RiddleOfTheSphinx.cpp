/// \file RiddleOfTheSphinx.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/riddleofthesphinx


// We need to devise a set of 5 questions knowing exactly 4 will be answered truthfully.
// No matter which one is answered falsely, we need to be able to recognize it.

// How about this?
// A: 1 0 0 B: 0 1 0 C: 0 0 1 D: 1 2 3 E: 5 7 11
// If D agrees with A, B, C, we assume E is a lie and vice versa
// If neither D nor E agree with A, B, C then what?
// Then we try leaving each of A, B, C out once at a time, calculating its value based on D and E.
// Hopefully there's one case where D and E agree with each other.

// I didn't notice that the numbers must be all <= 10.
// Replace 11 with 9 thinking they only need to be relatively prime to each other.

#include <iostream>

int
main()
{
    int A, B, C, D, E;
    std::cout << 1 << " " << 0 << " " << 0 << "\n";
    std::cin >> A;
    std::cout << 0 << " " << 1 << " " << 0 << "\n";
    std::cin >> B;
    std::cout << 0 << " " << 0 << " " << 1 << "\n";
    std::cin >> C;
    std::cout << 1 << " " << 2 << " " << 3 << "\n";
    std::cin >> D;
    std::cout << 5 << " " << 7 << " " << 9 << "\n";
    std::cin >> E;

    if (1 * A + 2 * B + 3 * C == D) {
        // D is true, E is a lie
        std::cout << A << " " << B << " " << C << "\n";
    }
    else if (5 * A + 7 * B + 9 * C == E) {
        // E is true, D is a lie
        std::cout << A << " " << B << " " << C << "\n";
    }
    else {
        // Imagine that A was the lie
        int da = D - 2 * B - 3 * C;
        int eanum = E - 7 * B - 9 * C;
        if (eanum % 5 == 0 && da == eanum / 5) {
            std::cout << da << " " << B << " " << C << "\n";
            return 0;
        }
        // Imagine that B was the lie
        int dbnum = D - 3 * C - 1 * A;
        int ebnum = E - 9 * C - 5 * A;
        if (dbnum % 2 == 0 && ebnum % 7 == 0 && dbnum / 2 == ebnum / 7) {
            std::cout << A << " " << dbnum / 2 << " " << C << "\n";
            return 0;
        }
        // Hope and pray that C was the lie
        std::cout << A << " " << B << " " << (D - 1 * A - 2 * B) / 3 << "\n";
    }
    return 0;
}