/// \file Traffic.cpp
/// \author Chad Hogg
/// \brief My (attempt at a) solution to https://open.kattis.com/problems/traffic

#include <iostream>
#include <string>
#include <vector>

struct Vehicle {
    int pos;
    std::vector<int> times;
    int index;
    bool moving;
};

struct State {
    Vehicle one;
    Vehicle two;
    int timeStep;

    bool
    hasMoreChanges () const {
        return one.index < one.times.size () || two.index < two.times.size ();
    }

    bool
    oneChangesNext () const {
        if (one.index == one.times.size ()) {
            return false;
        }
        else if (two.index == two.times.size ()) {
            return true;
        }
        else {
            return one.times.at (one.index) < two.times.at (two.index);
        }
    }

    void
    solve () {
        while (hasMoreChanges ()) {
            if (oneChangesNext ()) {
                if (one.moving) {
                    // TODO stop one, deal with it
                }
                else {
                    // TODO start one, deal with it
                }
                ++one.index;
            }
            else {
                if (one.moving) {
                    // TODO stop two, deal with it
                }
                else {
                    // TODO start two, deal with it
                }
                ++two.index;
            }
        }
        // TODO check whether any car is still moving / if so when they might collide
    }

};




int
main (int argc, char* argv[])
{
    int X1, X2, N1, N2, temp;
    std::vector<int> L1, L2;
    std::cin >> X1 >> X2;
    std::cin >> N1;
    L1.reserve (N1);
    for (int i = 0; i < N1; ++i) {
        std::cin >> temp;
        L1.push_back (temp);
    }
    std::cin >> N2;
    L2.reserve (N2);
    for (int i = 0; i < N2; ++i) {
        std::cin >> temp;
        L2.push_back (temp);
    }

    State state {{X1, L1, 0, false}, {X2, L2, 0, false}, 0};
    state.solve ();
    return 0;
}