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
    void insert(T elem)
    {
        if (root->keys_size() == 0) {
            root->insert(elem);
        } else if (root->keys_size() < root->max_key_size()) {

        }
    }
private:
    std::unique_ptr<Node<T>> root;
};