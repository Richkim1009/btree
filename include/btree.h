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
        if (!mRoot) {
            mRoot = std::make_unique<Node<T>>(mDegree);
        }
        std::unique_ptr<Node<T>> newRoot = mRoot->Insert(std::move(key));
        if (newRoot != nullptr) {
            mRoot = std::move(newRoot);
        }
        return *this;
    }

    BTree& Delete(const T key)
    {
        mRoot->Delete(key);
        if (mRoot->GetKeys().size() == 0) {
            mRoot = std::move(mRoot->GetChildren()[0]);
        }
        return *this;
    }

    void Print() const
    {
        if (mRoot) {
            mRoot->print(0);
        }
    }


private:
    size_t mDegree;
    std::unique_ptr<Node<T>> mRoot;
};