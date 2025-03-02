/// \file Siffrid.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/siffrid

#include <iostream>
#include <cassert>
#include <string>

int
digitSum (int num)
{
    std::string str = std::to_string (num);
    int total = 0;
    for (int i = 0; i < str.size (); ++i) {
        total += (str[i] - '0');
    }
    return total;
}

int smallest (int desiredSum, int desiredDigits)
{
    std::string answer;
    // Need to save one for putting in front of what might be some leading zeroes.
    int remainingSum = desiredSum - 1;
    while (answer.size () < desiredDigits) {
        int digit = remainingSum > 9 ? 9 : remainingSum;
        assert (digit >= 0 && digit <= 9 && digit <= remainingSum);
        answer = std::to_string (digit) + answer;
        remainingSum -= digit;
    }
    assert (answer[0] != '9');
    answer[0] = answer[0] + 1;
    return atoi (answer.c_str ());
}

int largest (int desiredSum, int desiredDigits)
{
    std::string answer;
    int remainingSum = desiredSum;
    while (answer.size () < desiredDigits) {
        int digit = remainingSum > 9 ? 9 : remainingSum;
        assert (digit >= 0 && digit <= 9 && digit <= remainingSum);
        answer = answer + std::to_string (digit);
        remainingSum -= digit;
    }
    return atoi (answer.c_str ());
}

int
main()
{
    int original;
    std::cin >> original;
    int desiredSum = digitSum (original);
    int desiredDigits = std::to_string (original).size ();
    std::cout << smallest (desiredSum, desiredDigits) << " " << largest (desiredSum, desiredDigits) << "\n";
    return 0;
}