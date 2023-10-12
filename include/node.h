#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
class Node {
public:
    explicit Node(size_t degree): mDegree{degree}
    {
        mKeys.reserve(degree - 1);
        mChildren.reserve(degree);
    }

    std::unique_ptr<Node> Insert(T elem)
    {
        if (mKeys.size() == 0) {
            mKeys.emplace_back(move(elem));
            return nullptr;
        }
        auto it = BinarySearch(elem);
        size_t index = std::distance(mKeys.begin(), it);
        if (mChildren.size() > 0) {
            auto tmp = mChildren[index]->Insert(elem);
            if (tmp == nullptr) return nullptr;
            
        } else {
            if (mKeys.size() < mDegree - 1) {
                mKeys.insert(std::next(mKeys.begin(), index), mKeys.begin(), mKeys.end());
                return nullptr;
            } else {
                size_t mid = mDegree / 2;
                std::unique_ptr<Node<T>> parent, leftChild, rightChild;
                if (mid == index) {
                    parent = std::make_unique(mDegree);
                    rightChild = std::make_unique(mDegree);
                    leftChild = std::make_unique(mDegree);
                    rightChild->mKeys.insert(c->mKeys.end(), std::make_move_iterator(mKeys.begin()+index), std::make_move_iterator(c->mKeys.end()));
                    mKeys.erase(mKeys.begin()+index, mKeys.end());
                    leftChild->mKeys = move(mKeys);
                    parent->mChildren.emplace_back(leftChild);
                    parent->mChildren.emplace_back(rightChild);
                    return parent;
                }
            }
        }
    }

    auto BinarySearch(T elem)
    {
        auto it = std::upper_bound(mKeys.begin(), mKeys.end(), elem);
        return it;
    }

private:
    size_t mDegree;
    std::vector<T> mKeys;
    std::vector<std::unique_ptr<Node<T>>> mChildren;
};
