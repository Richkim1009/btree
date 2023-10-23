#pragma once
#include "node.h"
#include <memory>
#include <vector>

template <typename T>
class BTree {
public: 
    BTree(size_t degree): mDegree(degree), mRoot(std::make_unique<Node<T>>(degree)) {}

    BTree& Insert(const T key)
    {
        mRoot = std::make_unique<Node<T>>(mDegree);  
        std::unique_ptr<Node<T>> newRoot = nullptr;
        newRoot = mRoot->Insert(std::move(key));
        if (mRoot != newRoot) {
            mRoot = std::move(newRoot);
        }
        return *this;
    }


private:
    size_t mDegree;
    std::unique_ptr<Node<T>> mRoot;
};