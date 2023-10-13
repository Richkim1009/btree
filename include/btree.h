#pragma once
#include "node.h"
#include <memory>
#include <vector>

template <typename T>
class BTree {
public: 
    BTree(size_t degree): mDegree(degree), mRoot(std::make_unique<Node<T>>(degree)) {}

    void Insert(const T&& key)
    {
        if (mRoot == nullptr) {
            mRoot = std::make_unique<Node<T>>(mDegree);
            mRoot->getKeys.emplace_back(std::forward(key));
        } else {
            mRoot->getKeys().emplace_back(std::forward(key));
        }
    }
private:
    size_t mDegree;
    std::unique_ptr<Node<T>> mRoot;
};