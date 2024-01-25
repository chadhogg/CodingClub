/// \file Ovissa.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/ovissa

#include <iostream>
#include <string>

int main()
{
    std::string line;
    std::cin >> line;
    unsigned count = 0;
    for(unsigned index = 0; index < line.size(); ++index) {
        if(line[index] == 'u') { ++count; }
    }
    std::cout << count << "\n";
    return 0;
}