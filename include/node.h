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
        mKeys.reserve(degree);
        mChildren.reserve(degree);
    }

    void Insert(T elem)
    {
        if (mKeys.size() == 0) {
            mKeys.emplace_back(move(elem));
            return;
        }
        auto it = BinarySearch(elem);
        size_t index = std::distance(mKeys.begin(), it);
        if (mChildren.size() > 0) {
            auto node = mChildren[index];
            node->Insert(elem);
        } else {
            mKeys.insert(it, elem);
            return;
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
