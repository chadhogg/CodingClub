/// \file Eszett.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/eszett

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

std::vector<std::string>
dealWithSs (const std::string allSs)
{
    // Would a more general solution be better?  Yes.
    // Would it be fun to think about right now?  No.
    if (allSs.size () == 1) {
        return {"s"};
    }
    else if (allSs.size () == 2) {
        return {"B", "ss"};
    }
    else {
        assert (allSs.size () == 3);
        return {"sss", "Bs", "sB"};
    }
}

int
main()
{
    std::string input;
    std::cin >> input;
    std::vector<std::string> oldPrefixes, newPrefixes;
    oldPrefixes.push_back ("");
    for (std::size_t index = 0; index < input.size (); ++index) {
        for (const std::string& old : oldPrefixes) {
            if (input[index] == 'S') {
                std::string esses;
                while (input[index] == 'S') { esses.push_back ('S'); ++index; }
                std::vector<std::string> parts = dealWithSs (esses);
                for (const std::string& part : parts) {
                    newPrefixes.push_back (old + part);
                }
                --index;
            }
            else {
                newPrefixes.push_back (old + (char)std::tolower (input[index]));
            }
        }
        oldPrefixes = newPrefixes;
        newPrefixes.clear ();
    }
    for (const std::string& str : oldPrefixes) {
        std::cout << str << "\n";
    }
    return 0;
}