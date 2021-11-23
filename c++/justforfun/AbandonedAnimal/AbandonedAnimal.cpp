/// \file AbandonedAnimal.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/abandonedanimal
///
/// My strategy here relies on an assumption that I can always buy everything at the earliest possible time without loss of generality,
///   and can also do so at the latest possible time.  If there is more than one solution, these will be different.
///
/// \note This is currently a work in progress.

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

using Store = unsigned int;
using Product = std::string;

/// \brief Reads some data from standard input.
/// \tparam T The type to be read.
/// \return The value that was read.
template<typename T>
T read() {
    T value;
    std::cin >> value;
    return value;
}

/// \brief Assigns each product to the earliest store it could possibly have been purchased at.
/// \param[in] availabilities A map of products to the (ordered) set of stores they are available in.
/// \param[in] shoppingList A vector of products in the order they were purchased.
/// \return A pointer to a vector containing the earliest stores each product could be purchased in, or NULL if they could not be purchased
///   in that order.
std::unique_ptr<std::vector<Store>> findEarliestStores (std::unordered_map<Product, std::set<Store>> const& availabilities, std::vector<Product> const& shoppingList) {
    std::vector<Store>* earliest = new std::vector<Store> {};
    earliest->reserve (shoppingList.size ());
    Store current {0U};
    for (unsigned int productIndex {0U}; productIndex < shoppingList.size (); ++productIndex) {
        bool found = false;
        for (Store possible : availabilities.at (shoppingList.at (productIndex))) {
            if (possible >= current) {
                earliest->push_back (possible);
                current = possible;
                found = true;
                break;
            }
        }
        if (!found) {
            delete earliest;
            return std::unique_ptr<std::vector<Store>> ();
        }
    }

    return std::unique_ptr<std::vector<Store>> (earliest);
}

/// \brief Assigns each product to the latest store it could possibly have been purchased at.
/// \param[in] numStores The number of stores.
/// \param[in] availabilities A map of products to the (ordered) set of stores they are available in.
/// \param[in] shoppingList A vector of products in the order they were purchased.
/// \return A pointer to a vector containing the latest stores each product could be purchased in, or NULL if they could not be purchased
///   in that order.
std::unique_ptr<std::vector<Store>> findLatestStores (unsigned int numStores, std::unordered_map<Product, std::set<Store>> const& availabilities, std::vector<Product> const& shoppingList) {
    std::vector<Store>* latest = new std::vector<Store> {};
    latest->reserve (shoppingList.size ());
    Store current {numStores - 1};
    for (int productIndex = shoppingList.size () - 1; productIndex >= 0; --productIndex) {
        bool found = false;
        Product product {shoppingList.at (productIndex)};
        std::set<Store> stores {availabilities.at (product)};
        for (std::set<Store>::const_reverse_iterator it = stores.rbegin (); it != stores.rend (); ++it) {
            Store possible {*it};
            if (possible <= current) {
                latest->push_back (possible);
                current = possible;
                found = true;
                break;
            }
        }
        if (!found) {
            delete latest;
            return std::unique_ptr<std::vector<Store>> ();
        }
    }
    std::reverse (latest->begin (), latest->end ());
    return std::unique_ptr<std::vector<Store>> (latest);
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    unsigned int numSupermarkets = read<unsigned int> ();
    unsigned int numAvailabilities = read<unsigned int> ();
    std::unordered_map<Product, std::set<Store>> availabilities {};
    for (unsigned int i {0U}; i < numAvailabilities; ++i) {
        Store store = read<Store> ();
        Product product = read<Product> ();
        if (availabilities.count (product) == 0) {
            availabilities[product] = std::set<Store> {};
        }
        availabilities[product].insert (store);
    }

    unsigned int shoppingLength = read<unsigned int> ();
    std::vector<Product> shoppingList {};
    shoppingList.reserve (shoppingLength);
    for (unsigned int i {0U}; i < shoppingLength; ++i) {
        shoppingList.push_back (read<Product> ());
    }

    std::unique_ptr<std::vector<Store>> earliest = findEarliestStores (availabilities, shoppingList);
    if (!earliest) {
        std::cout << "impossible\n";
    }
    else {
        std::unique_ptr<std::vector<Store>> latest = findLatestStores (numSupermarkets, availabilities, shoppingList);
        if (*earliest == *latest) {
            std::cout << "unique\n";
        }
        else {
            std::cout << "ambiguous\n";
        }
    }
    return 0;
}