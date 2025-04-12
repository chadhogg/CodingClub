/// \file Enduro.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/enduro

#include <cctype>
#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>
#include <map>
#include <list>

struct MyLocalTime
{
    int hours;
    int minutes;
    int seconds;
};

bool operator< (const MyLocalTime& t1, const MyLocalTime& t2)
{
    return t1.hours * 3600 + t1.minutes * 60 + t1.seconds < t2.hours * 3600 + t2.minutes * 60 + t2.seconds;
}

MyLocalTime
timeFromString (const std::string& str)
{
    assert (str.length () == 11);
    assert (str[2] == ':' && str[5] == ':' && str[8] == ' ');
    std::string hour = str.substr (0, 2);
    std::string minute = str.substr (3, 2);
    std::string second = str.substr (6, 2);
    std::string half = str.substr (9);
    assert (isdigit (hour[0]) && isdigit (hour[1]) && isdigit (minute[0]) 
        && isdigit (minute[1]) && isdigit (second[0]) && isdigit (second[1]));
    assert (half == "AM" || half == "PM");
    int hr = atoi (hour.c_str ());
    if (half == "PM" && hr != 12) { hr += 12; }
    if (half == "AM" && hr == 12) { hr -= 12; }
    return {hr, atoi (minute.c_str ()), atoi (second.c_str ())};
}

std::istream& operator>> (std::istream& in, MyLocalTime& t)
{
    std::string part1, part2;
    in >> part1 >> part2;
    assert (part1.length () == 8 && part1[2] == ':' && part1[5] == ':');
    assert (part2 == "AM" || part2 == "PM");
    t.hours = atoi (part1.substr (0, 2).c_str ());
    t.minutes = atoi (part1.substr (3, 2).c_str ());
    t.seconds = atoi (part1.substr (6).c_str ());
    if (part2 == "PM") {
        t.hours += 12;
    }
    return in;
}

std::ostream& operator<< (std::ostream& out, MyLocalTime& t)
{
    out << std::setw (2) << t.hours << ":" << std::setw (2) << t.minutes << ":" << std::setw (2) << t.seconds;
    return out;
}

void
printNames (std::map<std::string, std::list<MyLocalTime>> map)
{
    while (!map.empty ()) {
        std::string mostName = map.begin ()->first;
        int mostLaps = map.begin ()->second.size ();
        MyLocalTime mostLastTime = map.begin ()->second.back ();
        for (auto iter = map.cbegin (); iter != map.cend (); ++iter) {
            if (iter->second.size () > mostLaps
            || (iter->second.size () == mostLaps && iter->second.back () < mostLastTime)) {
                mostName = iter->first;
                mostLaps = iter->second.size ();
                mostLastTime = iter->second.back ();
            }
        }
        std::cout << mostLaps << " " << mostName << "\n";
        map.erase (mostName);
    }
}

int
main()
{
    std::string line;
    std::getline (std::cin, line);
    MyLocalTime startTime = timeFromString (line);
    MyLocalTime endTime = {startTime.hours + 3, startTime.minutes, startTime.seconds};
    std::getline (std::cin, line);
    int numRunners = atoi (line.c_str ());

    std::map<std::string, std::list<MyLocalTime>> runnerTimes;
    for (int i = 0; i < numRunners; ++i) {
        std::getline (std::cin, line);
        MyLocalTime lapTime = timeFromString (line.substr (0, 11));
        std::string name = line.substr (12);
        if (runnerTimes.count (name) == 0) { runnerTimes[name] = {}; }
        if (lapTime < startTime) { lapTime.hours += 24; }
        if (runnerTimes[name].empty () || runnerTimes[name].back () < endTime) {
            // Only the first time they cross at / after the end time matters.
            runnerTimes[name].push_back (lapTime);
        }
    }

    for (auto iter = runnerTimes.begin (); iter != runnerTimes.end (); ) {
        if (iter->second.empty () || iter->second.back () < endTime) {
            iter = runnerTimes.erase (iter);
        }
        else {
            ++iter;
        }
    }

    printNames (runnerTimes);
    return 0;
}