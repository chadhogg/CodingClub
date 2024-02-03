/// \file KcudDlanod.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/kcuddlanod

#include <iostream>
#include <string>

std::string
correct (const std::string& orig) {
    std::string revised;
    for (int i = orig.length() - 1; i >= 0; --i) {
        if (orig[i] == '2') { revised.push_back ('5');}
        else if (orig[i] == '5') { revised.push_back ('2');}
        else { revised.push_back (orig[i]); }
    }
    return revised;
}

int main()
{
    std::string first, second;
    std::cin >> first >> second;
    if (atoi (correct (first).c_str ()) > atoi (correct (second).c_str ())) {
        std::cout << "1\n";
    }
    else {
        std::cout << "2\n";
    }
    return 0;
}