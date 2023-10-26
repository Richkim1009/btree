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
        const size_t index = findUpperIndex(key);
        std::unique_ptr<Node<T>> ptr = nullptr;
        if (mChildren.empty()) {
            mKeys.insert(mKeys.begin()+index, std::move(key));
            ptr = split(*this);
            return ptr;
        } else {
            ptr = mChildren[index]->Insert(std::move(key));
            if (ptr != nullptr) {
                index = findUpperIndex(ptr->GetKeys()[0]);
                mKeys.insert(mKeys.begin()+index, ptr->GetKeys()[0]);
                mChildren.insert(mChildren.begin()+index, ptr->GetChildren().begin(), ptr->GetChildren().end());
            }
            ptr = split(*this);
            return ptr;
        }
    }
    size_t Delete(T key)
    {
        if (isFindKey(key)) {
            const size_t idx = findLowerIndex(key);
            T tmp = mChildren[idx]->search(key);
            const size_t currentIdx = findKeyIndex(key)
            mKeys[currentIdx] = tmp;
        } else {
            const size_t idx = findUpperIndex(key);
            mChildren[idx]->Delete(key);
        }
    }
private:
    const size_t findUpperIndex(const T& key) const
    {
        typename std::vector::const_iterator it = std::upper_bound(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(it, mKeys.begin());
        return index;
    }

    const size_t findLowerIndex(const T& key) const
    {
        typename std::vector::const_iterator it = std::lower_bound(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(it, mKeys.begin());
        return index;
    }

    const size_t findKeyIndex(const T& key) const
    {
        typename std::vector::const_iterator it = std::find(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(it, mKeys.begin());
        return index;
    }

    bool isFindKey(const T& key) const
    {
        return std::binary_search(mKeys.begin(), mKeys.end(), key);
    }

    const size_t minimumKeySize() const noexcept
    {
        if (mDegree & 1) {
            return mDegree/2;
        }
        return mDegree/2 - 1;
    }

    T search(T& key)
    {
        if (mChildren.size() == 0) {
            T tmp = mKeys[mKeys.size()-1];
            mKeys.pop_back();
            return tmp;
        } else {
            const size_t idx = mChildren.size() - 1;
            if (mChildren[idx]->GetKeys().size() < minimumKeySize()) {
                
            }
            return mChildren[idx]->search(key);
        }
    }

    std::unique_ptr<Node<T>> split(Node& node)
    {
        std::unique_ptr<Node<T>> ptr = nullptr;
        if (node.GetKeys().size() == node.GetDegree()) {
            size_t mid = (mKeys.size() - 1)/2;
            std::unique_ptr<Node<T>> parent = std::make_unique<Node<T>>(mDegree);
            std::unique_ptr<Node<T>> leftChild = std::make_unique<Node<T>>(mDegree);
            std::unique_ptr<Node<T>> rightChild = std::make_unique<Node<T>>(mDegree);
            leftChild->GetKeys().insert(leftChild->GetKeys().end(), std::move_iterator(mKeys.begin(), mKeys.begin()+mid));
            leftChild->GetChildren().insert(leftChild->GetChildren().end(), std::move_iterator(mChildren.begin(), mChildren.begin()+mid));
            rightChild->GetKeys().insert(rightChild->GetKeys().end(), std::move_iterator(mKeys.begin()+mid+1, mKeys.end()));
            rightChild->GetChildren().insert(rightChild->GetChildren().end(), std::move_iterator(mChildren.begin()+mid+1, mChildren.end()));
            parent->GetKeys().emplace_back(std::move(mKeys[mid]));
            parent->GetChildren().emplace_back(std::move(leftChild));
            parent->GetChildren().emplace_back(std::move(rightChild));
            ptr = std::move(parent);
        }
        return ptr;
    }
private:
    size_t mDegree;
    std::vector<T> mKeys;
    std::vector<std::unique_ptr<Node<T>>> mChildren;
};