#pragma once
#include <vector>
#include <memory>

template <typename T>
class Node {
public:
    Node(size_t degree): mDegree(degree) {}

    size_t GetDegree() const {return mDegree;}
    std::vector<T>& GetKeys() {return mKeys;}
    const std::vector<T>& GetKeys() const {return mKeys;}
    std::vector<std::unique_ptr<Node<T>>>& GetChildren() {return mChildren;}
    const std::vector<std::unique_ptr<Node<T>>>& GetChildren() const {return mChildren;}

    std::unique_ptr<Node<T>> Insert(T key)
    {
        std::unique_ptr<Node<T>> retPtr = nullptr;
        const size_t index = findIndex(key);
        if (mChildren.empty()) {
            if (mKeys.size() + 1 == mDegree) {
                retPtr = split(key, index);
            }
            mKeys.insert(mKeys.begin()+index, key);
        } else {
            mChildren[index]->Insert(std::move(key));
        }

        return retPtr;
    }
private:
    size_t findIndex(const T& key)
    {
        typename std::vector::const_iterator it = std::upper_bound(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(it, mKeys.begin());
        return index;
    }

    std::unique_ptr<Node<T>> split(T key, size_t index)
    {
        size_t mid = (mKeys.size() - 1)/2;
        std::unique_ptr<Node<T>> parentNode = std::make_unique<Node<T>>{mDegree};
        std::unique_ptr<Node<T>> rightChild = std::make_unique<Node<T>>{mDegree};
        std::unique_ptr<Node<T>> leftChild = std::make_unique<Node<T>>{mDegree};
        if (index == mid) {
            parentNode->GetKeys().emplace_back(std:move(key));
            leftChild->GetKeys().insert(leftChild->GetKeys().end(), 
                std::move_iterator(mKeys.begin(), std::move_iterator(mKeys.begin()+mid)));
            rightChild->GetKeys().insert(rightChild->Getkeys().end(),
                std::move_iterator(mKeys.begin()+mid), std::move_iterator(mKeys.end()));
        } else if (index < mid) {
            parentNode->GetKeys().emplace_back(std::move(mKeys[mid]));
            leftChild->GetKeys().insert(leftChild->GetKeys().end(),
                std::move_iterator(mKeys.begin(), mKeys.begin()+index));
            leftChild->GetKeys().insert(leftChild->GetKeys().end(),
                std::move_iterator(mKeys.begin()+index, mKeys.begin()+mid));
            rightChild->GetKey().insert(rightChild->GetKeys().end(),
                std::move_iterator(mKeys.begin()+mid+1, mKeys.end()));
        } else {
            parentNode->GetKeys().emplace_back(std::move(mKeys[mid]));
            leftChild->GetKeys().insert(leftChild->GetKeys().end(),
                std::move_iterator(mKeys.begin(), mKeys.begin()+index));
            rightChild->GetKey().insert(rightChild->GetKeys().end(),
                std::move_iterator(mKeys.begin()+mid+1, mKeys.begin()+index));
            rightChild->GetKeys().insert(rightChild->GetKeys().end(),
                std::move_iterator(mKeys.begin()+index, mKeys.end()));
        }
        parentNode->GetChildren().emplace_bacK(std::move(leftChild));
        parentNode->GetChildren().emplace_bacK(std::move(rightChild));

        return parentNode;
    }
private:
    size_t mDegree;
    std::vector<Node<T>> mKeys;
    std::vector<std::unique_ptr<Node<T>>> mChildren;
};