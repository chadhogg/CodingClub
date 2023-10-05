/// \file Classy.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/classy

#include <iostream>
#include <cmath>
#include <set>
#include <list>
#include <string>
#include <cstdio>
#include <cassert>

const int MAX_CLASS_PARTS = 10;

struct Person
{
    std::string name;
    std::list<std::string> classiness;
};

bool
operator< (const Person& p1, const Person& p2)
{
    assert (p1.classiness.size () == p2.classiness.size () && p1.classiness.size () == MAX_CLASS_PARTS);
    auto iter1 = p1.classiness.begin ();
    auto iter2 = p2.classiness.begin ();
    while (iter1 != p1.classiness.end ()) {
        std::string s1 = *iter1;
        std::string s2 = *iter2;
        assert (s1 == "upper" || s1 == "middle" || s1 == "lower");
        assert (s2 == "upper" || s2 == "middle" || s2 == "lower");
        if (s1 == "upper") {
            if (s2 == "middle" || s2 == "lower") { return false; }
        }
        if (s1 == "middle") {
            if (s2 == "upper") { return true; }
            if (s2 == "lower") { return false; }
        }
        if (s1 == "lower") {
            if (s2 == "upper" || s2 == "middle") { return true; }
        }
        ++iter1;
        ++iter2;
    }
    return p1.name > p2.name;
}

void
doCase ()
{
    int numPeople;
    std::cin >> numPeople;
    std::set<Person> people;
    while (numPeople > 0) {
        std::string name, classystring, junk;
        std::cin >> name >> classystring >> junk;
        assert (name[name.size () - 1] == ':');
        assert (junk == "class");
        Person person;
        person.name = name.substr (0, name.size () - 1);
        while (classystring.find_last_of ('-') != std::string::npos) {
            person.classiness.push_back (classystring.substr (classystring.find_last_of ('-') + 1));
            classystring = classystring.substr (0, classystring.find_last_of ('-'));
        }
        person.classiness.push_back (classystring);
        while (person.classiness.size () < MAX_CLASS_PARTS) {
            person.classiness.push_back ("middle");
        }
        people.insert (person);
        --numPeople;
    }
    for (auto iter = people.crbegin (); iter != people.crend (); ++iter) {
        std::cout << (*iter).name << "\n";
    }
    for (int i = 0; i < 30; ++i) {
        std::cout << "=";
    }
    std::cout << "\n";
}

int
main (int argc, char* argv[])
{
    int numCases;
    std::cin >> numCases;
    while (numCases > 0) {
        doCase ();
        --numCases;
    }
    return 0;
}