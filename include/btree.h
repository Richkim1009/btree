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
        std::unique_ptr<Node<T>> newRoot = nullptr;
        // 트리가 빈 경우
        if (mRoot == nullptr) {
            // 트리를 만듬
            mRoot = std::make_unique<Node<T>>(mDegree);  
        }
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