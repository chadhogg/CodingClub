/// \file Ceiling.cpp
/// \author Chad Hogg
/// My solution to https://open.kattis.com/problems/ceiling.
/// The idea behind this is that a tree's shape can be represented by a set of indices in which its
///   nodes would be stored if we were to store them in level-order in an array, as we do with heaps.
/// The idea was inspired by Evan MaGill and Marshall Feng, who used the same idea first.

#include <iostream>
#include <set>

struct Node
{
    int value;
    Node* left;
    Node* right;
};

class BST
{
public:
    BST()
    : overallRoot (nullptr)
    {
    }

    ~BST()
    {
        clear (overallRoot);
    }

    void
    insert (int val)
    {
        overallRoot = insert (val, overallRoot);
    }

    void
    clear ()
    {
        clear (overallRoot);
    }

    std::set<int>
    getLevelOrderIndices()
    {
        std::set<int> indices;
        getLevelOrderIndices (overallRoot, 0, indices);
        return indices;
    }

private:
    Node* overallRoot;

    static Node*
    insert (int val, Node* sTR)
    {
        if (sTR == nullptr) { sTR = new Node {val, nullptr, nullptr}; }
        else if (val < sTR->value) { sTR->left = insert (val, sTR->left); }
        else if (val > sTR->value) { sTR->right = insert (val, sTR->right); }
        return sTR;
    }

    static void
    clear (Node* sTR)
    {
        if (sTR != nullptr) {
            clear (sTR->left);
            clear (sTR->right);
            delete sTR;
        }
    }

    static void
    getLevelOrderIndices (Node* sTR, int index, std::set<int>& indices)
    {
        if (sTR != nullptr) {
            indices.insert (index);
            getLevelOrderIndices (sTR->left, index * 2 + 1, indices);
            getLevelOrderIndices (sTR->right, index * 2 + 2, indices);
        }
    }
};

int
main ()
{
    std::set<std::set<int>> shapes;
    int n, k;
    std::cin >> n >> k;
    for (int i = 0; i < n; ++i) {
        BST tree;
        int val;
        for (int j = 0; j < k; ++j) {
            std::cin >> val;
            tree.insert (val);
        }
        shapes.insert (tree.getLevelOrderIndices ());
    }
    std::cout << shapes.size () << "\n";
    return 0;
}
