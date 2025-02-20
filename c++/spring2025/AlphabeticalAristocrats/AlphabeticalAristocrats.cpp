/// \file AlphabeticalAristocrats.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/alphabeticalaristocrats

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Name
{
    std::string full;
    std::string afterCapital;
};

bool
operator< (const Name& n1, const Name& n2)
{
    return n1.afterCapital < n2.afterCapital;
}

bool
operator== (const Name& n1, const Name& n2)
{
    return n1.afterCapital == n2.afterCapital;
}

bool
operator<= (const Name& n1, const Name& n2)
{
    return n1.afterCapital <= n2.afterCapital;
}

std::string
getFirstCapital(const std::string& str) {
    for (int i = 0; i < str.size (); ++i) {
        if (isupper (str[i])) { return str.substr (i); }
    }
    throw "Impossible";
}

std::vector<Name>
readInput ()
{
    std::string foo;
    int count;
    std::cin >> count;
    std::getline(std::cin, foo); // throw away blank after the number.
    std::vector<Name> names;
    names.reserve (count);
    for (int i = 0; i < count; ++i) {
        std::getline(std::cin, foo);
        names.push_back ({foo, getFirstCapital (foo)});
    }
    return names;
}

int
main()
{
    std::vector<Name> names = readInput ();
    std::sort (names.begin (), names.end ());
    for(const Name& name : names) {
        std::cout << name.full << "\n";
    }
    return 0;
}