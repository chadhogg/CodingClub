/// \file EchoEchoEcho.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/echoechoecho

#include <iostream>
#include <string>

int main()
{
    std::string str;
    std::cin >> str;
    std::cout << str << " " << str << " " << str << "\n";
    return 0;
}