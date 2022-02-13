/// \file Spiderman.cpp
/// \brief A solution to https://open.kattis.com/problems/stockprices
/// \author Chad Hogg
/// \version 2022-02-10

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

enum Type { BUY, SELL };

struct Order {
    Type type;
    int value;
    int quantity;
};

struct OrderComparator {
    bool operator() (Order const& a, Order const& b) {
        if (a.type == SELL) { return a.value > b.value; }
        else { return a.value < b.value; }
    }
};
OrderComparator comp;

struct Market {
    std::vector<Order> buyOrders;
    std::vector<Order> sellOrders;
    int lastSalePrice;
};

using TestCase = std::vector<Order>;

using Problem = std::vector<TestCase>;

constexpr int NO_VALUE = -1;

std::ostream & operator<< (std::ostream & out, Order const& order) {
    if (order.type == BUY) {
        out << "buy " << order.quantity << " shares at " << order.value;
    }
    else {
        out << "sell " << order.quantity << " shares at " << order.value;
    }
    return out;
}

std::ostream & operator<< (std::ostream & out, std::vector<Order> const& orders) {
    for (Order const& order : orders) {
        out << order << "\n";
    }
    return out;
}

std::ostream & operator<< (std::ostream & out, Market const& market) {
    out << "Outstanding buy orders:\n" << market.buyOrders << "Outstanding sell orders:\n" << market.sellOrders << "Last sale price: " << market.lastSalePrice;
    return out;
}

Problem readProblem () {
    int numCases, numLines;
    std::cin >> numCases;
    Problem problem;
    for (int i = 0; i < numCases; ++i) {
        TestCase tc;
        std::cin >> numLines;
        for (int line = 0; line < numLines; ++line) {
            int quantity, value;
            std::string word;
            std::cin >> word;
            Type type;
            if (word == "buy") { type = BUY; }
            else if (word == "sell" ) { type = SELL; }
            else { throw std::runtime_error ("Unexpected word: " + word); }
            std::cin >> quantity;
            std::cin >> word;
            if (word != "shares") { throw std::runtime_error ("Unexpected word: " + word); }
            std::cin >> word;
            if (word != "at") { throw std::runtime_error ("Unexpected word: " + word); }
            std::cin >> value;
            tc.push_back ({type, value, quantity});
        }
        problem.push_back (tc);
    }
    return problem;
}

void print (std::ostream & out, int ask, int bid, int price) {
    if (ask == NO_VALUE) { out << "-"; }
    else { out << ask; }
    out << " ";
    if (bid == NO_VALUE) { out << "-"; }
    else { out << bid; }
    out << " ";
    if (price == NO_VALUE) { out << "-"; }
    else { out << price; }
    out << "\n";

}

void solveTestCase (TestCase const& tc) {
    Market market {{}, {}, NO_VALUE};
    for (Order const& order : tc) {
        if (order.type == BUY) {
            market.buyOrders.push_back (order);
            std::push_heap (market.buyOrders.begin (), market.buyOrders.end (), comp);
        }
        else {
            market.sellOrders.push_back (order);
            std::push_heap (market.sellOrders.begin (), market.sellOrders.end (), comp);
        }
        int ask = market.sellOrders.empty () ? NO_VALUE : market.sellOrders.front ().value;
        int bid = market.buyOrders.empty () ? NO_VALUE : market.buyOrders.front ().value;

        while (ask != NO_VALUE && bid != NO_VALUE && ask <= bid) {
            market.lastSalePrice = ask;
            if (market.buyOrders.front ().quantity >= market.sellOrders.front ().quantity) {
                market.buyOrders.front ().quantity -= market.sellOrders.front ().quantity;
                market.sellOrders.front ().quantity = 0;
            }
            else {
                market.sellOrders.front ().quantity -= market.buyOrders.front ().quantity;
                market.buyOrders.front ().quantity = 0;
            }
            if (market.buyOrders.front ().quantity == 0) {
                std::pop_heap (market.buyOrders.begin (), market.buyOrders.end (), comp);
                market.buyOrders.pop_back ();
            }
            if (market.sellOrders.front ().quantity == 0) {
                std::pop_heap (market.sellOrders.begin (), market.sellOrders.end (), comp);
                market.sellOrders.pop_back ();
            }
            ask = market.sellOrders.empty () ? NO_VALUE : market.sellOrders.front ().value;
            bid = market.buyOrders.empty () ? NO_VALUE : market.buyOrders.front ().value;
        }
        print (std::cout, ask, bid, market.lastSalePrice);
        //std::cout << market << "\n\n";
    }
}

int main (int argc, char * argv[]) {
    Problem prob = readProblem ();
    for (TestCase const& tc : prob) {
        solveTestCase (tc);
    }
    return 0;
}
