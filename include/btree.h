#pragma once
#include "node.h"
#include <memory>
#include <vector>

template <typename T, int N = 3>
class BTree {
public: 
    BTree()
    {
        root = std::make_unique<Node<T>>(N);
    }
    void Insert(T elem)
    {
        root->Insert(elem);
    }
private:
    std::unique_ptr<Node<T>> root;
};