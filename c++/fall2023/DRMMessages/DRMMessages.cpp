/// \file DRMMessages.cpp
/// \author CHad Hogg
/// \brief My solution to https://open.kattis.com/problems/drmmessages

#include <iostream>
#include <string>
#include <cassert>

void
sanityCheck (std::string input)
{
    assert (input.length () > 0);
    assert (input.length () <= 15000);
    assert (input.length () % 2 == 0);
    for (char c : input)
    {
        assert (isupper (c));
    }
}

std::string
rotate (std::string original)
{
    int amount = 0;
    for (char c: original) {
        amount = (amount + (c - 'A')) % 26;
    }
    std::string result = original;
    for (int i = 0; i < result.size (); ++i) {
        result[i] = (result[i] - 'A' + amount) % 26 + 'A'; 
    }
    //std::cout << "Rotated " << original << " to " << result << "\n";
    return result;
}

std::string
merge (std::string first, std::string second)
{
    std::string result = first;
    for (int i = 0; i < result.size (); ++i) {
        result[i] = (result[i] - 'A' + second[i] - 'A') % 26 + 'A';
    }
    return result;
}

std::string
decrypt (std::string ciphertext)
{
    std::string firstHalf = ciphertext.substr (0, ciphertext.length () / 2);
    std::string secondHalf = ciphertext.substr (ciphertext.length () / 2);
    std::string rotatedFirst = rotate(firstHalf);
    std::string rotatedSecond = rotate(secondHalf);
    return merge(rotatedFirst, rotatedSecond);
}

int
main (int argc, char* argv[])
{
    std::string ciphertext;
    std::cin >> ciphertext;
    sanityCheck (ciphertext);
    std::cout << decrypt (ciphertext) << "\n";
    return 0;
}