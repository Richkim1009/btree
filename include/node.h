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
        const size_t index = findIndex(key);
        std::unique_ptr<Node<T>> ptr = nullptr;
        if (mChildren.empty()) {
            ptr = mKeys.insert(mKeys.begin()+index, std::move(key));
            if (mKeys.size() == mDegree) {
                size_t mid = (mKeys.size() - 1)/2;
                std::unique_ptr<Node<T>> parent = std::make_unique<Node<T>>(mDegree);
                std::unique_ptr<NOde<T>> leftChild = std::make_unique<Node<T>>(mDegree);
                std::unique_ptr<Node<T>> rightChild = std::make_unique<Node<T>>(mDegree);
                leftChild->GetKeys().insert(leftChild->GetKeys().end(), std::move_iterator(mKeys.begin(), mKeys.begin()+mid));
                rightChild->GetKeys().insert(rightChild->GetKeys().end(), std::move_iterator(mKeys.begin()+mid+1, mKeys.end()));
                parent->GetKeys().emplace_back(std::move(mKeys[mid]));
                parent->GetChildren().emplace_back(std::move(leftChild));
                parent->GetChildren().emplace_back(std::move(rightChild));
                return ptr;
            }
            return ptr;
        } else {
            ptr = mChildren[index]->Insert(std::move(key));
            index = findIndex(ptr->GetKeys()[0]);
            mKeys.insert(mKeys.begin()+index, ptr->GetKeys()[0]);
            if (mKeys.size() == mDegree) {
                size_t mid = (mKeys.size() - 1)/2;
                std::unique_ptr<Node<T>> parent = std::make_unique<Node<T>>(mDegree);
                std::unique_ptr<NOde<T>> leftChild = std::make_unique<Node<T>>(mDegree);
                std::unique_ptr<Node<T>> rightChild = std::make_unique<Node<T>>(mDegree);
                leftChild->GetKeys().insert(leftChild->GetKeys().end(), std::move_iterator(mKeys.begin(), mKeys.begin()+mid));
                rightChild->GetKeys().insert(rightChild->GetKeys().end(), std::move_iterator(mKeys.begin()+mid+1, mKeys.end()));
                parent->GetKeys().emplace_back(std::move(mKeys[mid]));
                parent->GetChildren().emplace_back(std::move(leftChild));
                parent->GetChildren().emplace_back(std::move(rightChild));
                return ptr;
            }
        }
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
        
    }
private:
    size_t mDegree;
    std::vector<Node<T>> mKeys;
    std::vector<std::unique_ptr<Node<T>>> mChildren;
};