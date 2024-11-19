/// \file DuelOfCards.cpp
/// \author Chad Hogg
/// \brief My solution to https://open.kattis.com/problems/duelofcards

#include <iostream>
#include <string>
#include <cassert>
#include <list>
#include <vector>

// For each of Alice's cards she either wants to win, beating the highest possible opponent card,
//   or lose, losing to the highest possible opponent card
/*
int
maxScore (std::vector<int> alice, std::vector<int> bob, int score)
{
    int highestAliceCard = alice.back ();
    alice.pop_back ();
    int highestBobCard = bob.back ();
    if (highestAliceCard > highestBobCard) {
        // 
        bob.pop_back ();
        return maxScore (alice, bob, score + 1);
    }
}
*/

int
maxScore (std::list<int> alice, std::list<int> bob)
{
    int total = 0;
    while (!alice.empty ()) {
        if (alice.back () > bob.back ()) {
            // Alice has the highest card; the best scenario for her is that Bob loses with his highest card.
            alice.pop_back ();
            bob.pop_back ();
            total += 1;
        }
        else {
            // Bob has the highest card; the best scenario for her is that she loses with her lowest card.
            alice.pop_front ();
            bob.pop_back ();
            total += 0;
        }
    }
    return total;
}

int
minScore (std::list<int> alice, std::list<int> bob)
{
    int total = 0;
    while (!alice.empty ()) {
        if (alice.back () > bob.back ()) {
            // Alice has the highest card; the worst scenario for her is that Bob loses with his lowest card.
            alice.pop_back ();
            bob.pop_front ();
            total += 1;
        }
        else {
            // Bob has the highest card; the worst scenario for her is that she loses with her highest card.
            alice.pop_back ();
            bob.pop_back ();
            total += 0;
        }
    }
    return total;
}


int
main (int argc, char* argv[])
{
    int n;
    std::cin >> n;
    std::vector<bool> aliceCards (2 * n, false);
    for (int x = 0; x < n; ++x) {
        int card;
        std::cin >> card;
        --card;
        assert (card < 2 * n);
        assert (!aliceCards[card]);
        aliceCards[card] = true;
    }
    std::list<int> alice, bob;
    for (int x = 0; x < 2 * n; ++x) {
        if (aliceCards[x]) { alice.push_back (x); }
        else { bob.push_back (x); }
    }
    assert (alice.size () == bob.size ());
    std::cout << minScore (alice, bob) << " " << maxScore (alice, bob) << "\n";

/*
    std::vector<bool> playedCards (2 * n, false);
    int numPlayedCards = 0;
    for (int potentialAliceCard = 2 * n - 1; potentialAliceCard >= 0; --potentialAliceCard) {
        if (aliceCards[potentialAliceCard] && !playedCards[potentialAliceCard]) {
            // This is the highest card Alice can still play.
        }
    }
    */
    return 0;
}