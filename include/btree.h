#pragma once
#include "node.h"
#include <memory>
#include <vector>

template <typename T>
class BTree {
public: 
    BTree(size_t degree): mDegree(degree), mRoot(std::make_unique<Node<T>>(degree)) {}

    void Insert(const T key)
    {
        // 트리가 빈 경우
        if (mRoot == nullptr) {
            // 트리를 만들고 키 삽입
            mRoot = std::make_unique<Node<T>>(mDegree);
            mRoot->GetKeys().emplace_back(std::move(key));
        } else {
            mRoot->Insert(std::move(key));
        }
    }


private:
    size_t mDegree;
    std::unique_ptr<Node<T>> mRoot;
};