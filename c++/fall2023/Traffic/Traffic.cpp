/// \file Traffic.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/traffic
/// \note Only the basic solver works for every test case, but it is fast enough.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

const int CLOSEST_ALLOWABLE = 5;
const bool MESSAGES = false;
const bool BASIC = true;

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
    bothChangeAtSameTime () const {
        return one.index < one.times.size () && two.index < two.times.size () && one.times[one.index] == two.times[two.index];
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

    /// @brief  Stops both vehicles.
    /// @param first The first vehicle.
    /// @param second The second vehicle.
    /// @pre Both vehicles have a next event at the same time.
    /// @pre Both vehicles are currently moving.
    /// @post Both vehicles are stopped.
    /// @post Both vehicle positions have been increased, as has the time step.
    /// @post The next event index has been incremented for both cars.
    /// @note No collisions during the last interval were possible, since vehicles were moving at the same speed.
    void
    stopBoth (Vehicle& first, Vehicle& second) {
        assert (first.index < first.times.size () && second.index < second.times.size ());
        assert (first.times[first.index] == second.times[second.index]);
        assert (first.moving && second.moving);
        int timeSinceLastUpdate = first.times[first.index] - timeStep;
        if (MESSAGES) {
            std::cout << "Stopping both vehicles at time " << first.times[first.index] << "\n";
            std::cout << "\tone vehicle had been at " << first.pos << " at time " << timeStep << " but is now at " << first.pos + timeSinceLastUpdate << "\n";
            std::cout << "\tanother     had been at " << second.pos << " at time " << timeStep << " but is now at " << second.pos + timeSinceLastUpdate << "\n";
        }
        first.moving = false;
        second.moving = false;
        first.pos += timeSinceLastUpdate;
        second.pos += timeSinceLastUpdate;
        timeStep += timeSinceLastUpdate;
        ++first.index;
        ++second.index;
    }

    /// @brief  Starts both vehicles.
    /// @param first The first vehicle.
    /// @param second The second vehicle.
    /// @pre Both vehicles have a next event at the same time.
    /// @pre Both vehicles are currently stopped.
    /// @post Both vehicles are moving.
    /// @post Both vehicle positions have stayed the same, but the time step has increased.
    /// @post The next event index has been incremented for both cars.
    /// @note No collisions during the last interval were possible, since vehicles were stopped.
    void
    startBoth (Vehicle& first, Vehicle& second) {
        assert (first.index < first.times.size () && second.index < second.times.size ());
        assert (first.times[first.index] == second.times[second.index]);
        assert (!first.moving && !second.moving);
        int timeSinceLastUpdate = first.times[first.index] - timeStep;
        if (MESSAGES) {
            std::cout << "Starting both vehicles at time " << first.times[first.index] << "\n";
            std::cout << "\tone vehicle had been at " << first.pos << " at time " << timeStep << " and still is\n";
            std::cout << "\tanother     had been at " << second.pos << " at time " << timeStep << " and still is\n";
        }
        first.moving = true;
        second.moving = true;
        timeStep += timeSinceLastUpdate;
        ++first.index;
        ++second.index;
    }

    /// @brief Starts one vehicle while stopping the other.
    /// @param starter The vehicle that is starting.
    /// @param stopper The vehicle that is stopping.
    /// @pre Both vehicles have a next event at the same time.
    /// @pre The first vehicle is currently stopped while the second is moving.
    /// @post The first vehicle will now be moving and the second now stopped.
    /// @post The first vehicle's position will stay the same, while the second's position and the time step will have increased.
    /// @post The next event index has been incremented for both cars.
    /// @note There could be a collision, if the one that had been moving moves through the fixed position of the one that was not.
    void
    startOneStopOne (Vehicle& starter, Vehicle& stopper) {
        assert (starter.index < starter.times.size () && stopper.index < stopper.times.size ());
        assert (starter.times[starter.index] == stopper.times[stopper.index]);
        assert (!starter.moving && stopper.moving);
        int timeSinceLastUpdate = starter.times[starter.index] - timeStep;
        int finalStopperPosition = stopper.pos + timeSinceLastUpdate;
        if (MESSAGES) {
            std::cout << "Starting a vehicle while stopping another at time " << starter.times[starter.index] << "\n";
            std::cout << "\tStarter was at " << starter.pos << " at time " << timeStep << " and still is";
            std::cout << "\tStopper was at " << stopper.pos << " at time " << timeStep << " and is now at " << stopper.pos + timeSinceLastUpdate << "\n";
        }
        if (stopper.pos < starter.pos && starter.pos < finalStopperPosition + CLOSEST_ALLOWABLE) {
                std::cout << "bumper tap at time " << timeStep + (stopper.pos - starter.pos + CLOSEST_ALLOWABLE + 1) << "\n";
            exit (0);
        }
        starter.moving = true;
        stopper.moving = false;
        stopper.pos = finalStopperPosition;
        timeStep += timeSinceLastUpdate;
        ++starter.index;
        ++stopper.index;
    }

    /// @brief Starts one of the vehicles, leaving the other in its current state.
    /// @param starter The vehicle that should start.
    /// @param other The vehicle whose state is not changing.
    /// @pre The starter vehicle has a next event, and is currently stopped.
    /// @post The starter vehicle begins moving.
    /// @post If the other vehicle was moving, its position is increased.
    /// @post The time step is increased.
    /// @post The next event index has been incremented for the starter.
    /// @note There can be a collision here, but only if other was moving and happens to have moved through starter's space.
    void
    startOne (Vehicle& starter, Vehicle& other) {
        assert (starter.index < starter.times.size () && !starter.moving);
        int timeSinceLastUpdate = starter.times[starter.index] - timeStep;
        if (MESSAGES) {
            std::cout << "Starting one vehicle, leaving other unchanged, at time " << starter.times[starter.index] << "\n";
            std::cout << "\tStarter was at " << starter.pos << " at time " << timeStep << " and still is\n";
            if(other.moving) {
                std::cout << "\tOther was at " << other.pos << " at time " << timeStep << " and is now at " << other.pos + timeSinceLastUpdate << "\n";
            }
            else {
                std::cout << "\tOther was at " << other.pos << " at time " << timeStep << " and still is\n";
            }
        }
        if (other.moving) {
            int endPosition = other.pos + timeSinceLastUpdate;
            if (other.pos < starter.pos && starter.pos < endPosition + CLOSEST_ALLOWABLE) {
                std::cout << "bumper tap at time " << timeStep + (other.pos - starter.pos + CLOSEST_ALLOWABLE + 1) << "\n";
                exit (0);
            }
            other.pos = endPosition;
        }
        timeStep += timeSinceLastUpdate;
        starter.moving = true;
        ++starter.index;
    }

    /// @brief Stops one of the vehicles, leaving the other in its current state.
    /// @param stopper The vehicle that should stop.
    /// @param other The vehicle whose state is not changing.
    /// @pre The stopper vehicle has a next event, and is currently moving.
    /// @post The stopper vehicle stops moving.
    /// @post If the other vehicle was moving, its position is increased.
    /// @post The stopper vehicle's position is increased.
    /// @post The time step is increased.
    /// @post The next event index has been incremented for the stopper.
    /// @note There can be a collision here, but only if other was stationary and stopper moved through its space.
    void
    stopOne (Vehicle& stopper, Vehicle& other) {
        assert (stopper.index < stopper.times.size () && stopper.moving);
        int timeSinceLastUpdate = stopper.times[stopper.index] - timeStep;
        if (MESSAGES) {
            std::cout << "Stopping one vehicle, leaving other unchanged, at time " << stopper.times[stopper.index] << "\n";
            std::cout << "\tStopper was at " << stopper.pos << " at time " << timeStep << " and is now at " << stopper.pos + timeSinceLastUpdate << "\n";
            if(other.moving) {
                std::cout << "\tOther was at " << other.pos << " at time " << timeStep << " and is now at " << other.pos + timeSinceLastUpdate << "\n";
            }
            else {
                std::cout << "\tOther was at " << other.pos << " at time " << timeStep << " and still is\n";
            }
        }
        if (other.moving) {
            other.pos += timeSinceLastUpdate;
        }
        else {
            if (stopper.pos < other.pos && other.pos < stopper.pos + timeSinceLastUpdate + CLOSEST_ALLOWABLE) {
                std::cout << "bumper tap at time " << timeStep + (stopper.pos - other.pos + CLOSEST_ALLOWABLE + 1) << "\n";
                exit (0);
            }
        }
        stopper.pos += timeSinceLastUpdate;
        timeStep += timeSinceLastUpdate;
        stopper.moving = false;
        ++stopper.index;
    }

    void
    solve () {
        while (hasMoreChanges ()) {
            if (MESSAGES) {
                std::cout << "At time " << timeStep << " one is at " << one.pos << " and two is at " << two.pos << "\n";
            }
            if (bothChangeAtSameTime ()) {
                if (one.moving && two.moving) { stopBoth (one, two); }
                else if (one.moving & !two.moving) { startOneStopOne (two, one); }
                else if (!one.moving & two.moving) { startOneStopOne (one, two); }
                else { startBoth (one, two); }
            }
            else if (oneChangesNext ()) {
                if (one.moving) { stopOne (one, two); }
                else { startOne (one, two); }
            }
            else {
                if (two.moving) { stopOne (two, one); }
                else { startOne (two, one); }
            }
        }
        if (one.moving && !two.moving && one.pos < two.pos) {
            std::cout << "bumper tap at time " << timeStep + (two.pos - one.pos - CLOSEST_ALLOWABLE + 1) << "\n";
        }
        else if (!one.moving && two.moving && two.pos < one.pos) {
            std::cout << "bumper tap at time " << timeStep + (one.pos - two.pos - CLOSEST_ALLOWABLE + 1) << "\n";
        }
        else {
            std::cout << "safe and sound\n";
        }
    }

    void
    basicSolve () {
        while (hasMoreChanges ()) {
            if (one.moving) {
                ++one.pos;
            }
            if (two.moving) {
                ++two.pos;
            }
            if (abs (one.pos - two.pos) < CLOSEST_ALLOWABLE) {
                std::cout << "bumper tap at time " << timeStep << "\n";
                exit (0);
            }
            if (one.index < one.times.size () && timeStep == one.times[one.index]) {
                one.moving = !one.moving;
                ++one.index;
            }
            if (two.index < two.times.size () && timeStep == two.times[two.index]) {
                two.moving = !two.moving;
                ++two.index;
            }
            ++timeStep;
        }
        if (one.moving && !two.moving && one.pos < two.pos) {
            std::cout << "bumper tap at time " << timeStep + (two.pos - one.pos - CLOSEST_ALLOWABLE) << "\n";
        }
        else if (!one.moving && two.moving && two.pos < one.pos) {
            std::cout << "bumper tap at time " << timeStep + (one.pos - two.pos - CLOSEST_ALLOWABLE) << "\n";
        }
        else {
            std::cout << "safe and sound\n";
        }
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
    if (BASIC) {
        state.basicSolve ();
    }
    else {
        state.solve ();
    }
    return 0;
}