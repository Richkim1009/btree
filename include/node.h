#pragma once
#include <vector>
#include <memory>
#include <iostream>

template <typename T>
class Node {
public:
    Node(size_t degree): mDegree(degree) {}

    size_t GetDegree() const {return mDegree;}
    std::vector<T>& GetKeys() {return mKeys;}
    const std::vector<T>& GetKeys() const {return mKeys;}
    std::vector<std::unique_ptr<Node<T>>>& GetChildren() {return mChildren;}
    const std::vector<std::unique_ptr<Node<T>>>& GetChildren() const {return mChildren;}
    void print(int depth) const
    {
        std::string indent = std::string(depth * 2, ' '); 

        std::cout << indent << "Keys: ";
        for (const T& key : mKeys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;

        for (const auto& child : mChildren) {
            if (child) {
                child->print(depth + 1);
            }
        }
    }

    std::unique_ptr<Node<T>> Insert(T key)
    {
        size_t index = findUpperIndex(key);
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
                mChildren.erase(mChildren.begin()+index);
                ptr->GetKeys().clear();
                mChildren.insert(mChildren.begin()+index, 
                std::make_move_iterator(ptr->GetChildren().begin()), std::make_move_iterator(ptr->GetChildren().end()));
                ptr->GetChildren().clear();
            }
            ptr = split(*this);
            return ptr;
        }
    }

    void Delete(T key)
    {
        if (isFoundKey(key)) {
            const size_t idx = findLowerIndex(key);
            T tmp = mChildren[idx]->search(key);
            const size_t currentIdx = findKeyIndex(key);
            mKeys[currentIdx] = tmp;
        } else {
            const size_t idx = findUpperIndex(key);
            mChildren[idx]->Delete(key);
        }
    }
private:
    const size_t findUpperIndex(const T& key) const
    {
        auto it = std::upper_bound(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(mKeys.begin(), it);
        return index;
    }

    const size_t findLowerIndex(const T& key) const
    {
        auto it = std::lower_bound(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(mKeys.begin(), it);
        return index;
    }

    const size_t findKeyIndex(const T& key) const
    {
        auto it = std::find(mKeys.begin(), mKeys.end(), key);
        const size_t index = std::distance(mKeys.begin(), it);
        return index;
    }

    bool isFoundKey(const T& key) const
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
            const size_t currentIdx = mKeys.size() - 1;
            T childItem = mChildren[idx]->search(key);
            if (mChildren[idx]->GetKeys().size() < minimumKeySize()) {
                if (idx == 0) {
                    if (mChildren[idx+1]->GetKeys().size() > minimumKeySize()) {
                        borrowKeyFromRightSibling(idx);
                    } else {
                        borrowKeyFromParent(idx, currentIdx);
                    } 
                } else if (idx == mChildren.size()-1) {
                    if (mChildren[idx-1]->GetKeys().size() > minimumKeySize()) {
                        borrowKeyFromLeftSibling(idx);
                    } else {
                        borrowKeyFromParent(idx, currentIdx);
                    }
                } else {
                    if (mChildren[idx+1]->GetKeys().size() > minimumKeySize()) {
                        borrowKeyFromRightSibling(idx);
                    } else if (mChildren[idx-1]->GetKeys().size() > minimumKeySize()) {
                        borrowKeyFromLeftSibling(idx);
                    } else {
                        borrowKeyFromParent(currentIdx);
                    }
                }
            }
            return childItem;
        }
    }

    void borrowKeyFromRightSibling(size_t idx)
    {
        auto & childItem = mChildren[idx+1];
        auto & childKeys = childItem->GetKeys();
        auto & childChildren = childItem->GetChildren();
        mKeys.emplace_back(std::move(childKeys[0]));
        mChildren.emplace_back(std::move(childChildren[0]));
        childKeys.erase(childKeys.begin());
        childChildren.erase(childChildren.begin());
    }

    void borrowKeyFromLeftSibling(size_t idx)
    {
        auto & childItem = mChildren[idx-1];
        auto & childKeys = childItem->GetKeys();
        auto & childChildren = childItem->GetChildren();
        mKeys.insert(mKeys.begin(), std::move(childKeys[childKeys.size()-1]));
        mChildren.insert(mChildren.begin(), std::move(childChildren[childChildren.size()-1]));  
        childKeys.pop_back();
        childChildren.pop_back();
    }
    void borrowKeyFromParent(size_t idx, size_t currentIdx)
    {
        auto currentItem = mKeys[currentIdx];
        auto & leftChildKeys = mChildren[idx].GetKeys();
        auto & leftChildChildren = mChildren[idx].GetChildren();
        auto & rightChildKeys = mChildren[idx+1].GetKeys();
        auto & rightChildChildren = mChildren[idx+1].GetChildren();
        leftChildKeys.emplace_back(currentItem);
        leftChildKeys.insert(leftChildKeys.end(), rightChildKeys.begin(), rightChildKeys.end());
        leftChildChildren.insert(leftChildChildren.end(), std::make_move_iterator(rightChildChildren.begin()), std::make_move_iterator(rightChildChildren.end()));
        mChildren.pop_back();
        mKeys.pop_back();
    }

    std::unique_ptr<Node<T>> split(Node& node)
    {
        std::unique_ptr<Node<T>> ptr = nullptr;
        if (node.GetKeys().size() == node.GetDegree()) {
            size_t mid = mKeys.size()/2;
            std::unique_ptr<Node<T>> parent = std::make_unique<Node<T>>(mDegree);
            std::unique_ptr<Node<T>> leftChild = std::make_unique<Node<T>>(mDegree);
            std::unique_ptr<Node<T>> rightChild = std::make_unique<Node<T>>(mDegree);

            leftChild->GetKeys().insert(
                leftChild->GetKeys().end(),
                std::make_move_iterator(node.GetKeys().begin()),
                std::make_move_iterator(node.GetKeys().begin() + mid)
            );

            rightChild->GetKeys().insert(
                rightChild->GetKeys().end(),
                std::make_move_iterator(node.GetKeys().begin() + mid + 1),
                std::make_move_iterator(node.GetKeys().end())
            );

            if (node.GetChildren().size() > 0) {
                leftChild->GetChildren().insert(
                    leftChild->GetChildren().end(),
                    std::make_move_iterator(node.GetChildren().begin()),
                    std::make_move_iterator(node.GetChildren().begin() + mid+1)
                );

                rightChild->GetChildren().insert(
                    rightChild->GetChildren().end(),
                    std::make_move_iterator(node.GetChildren().begin() + mid+1),
                    std::make_move_iterator(node.GetChildren().end())
                );
            }
            parent->GetKeys().emplace_back(std::move(node.GetKeys()[mid]));
            parent->GetChildren().emplace_back(std::move(leftChild));
            parent->GetChildren().emplace_back(std::move(rightChild));
            node.GetChildren().clear();
            node.GetKeys().clear();
            ptr = std::move(parent);
        }
        return ptr;
    }
private:
    size_t mDegree;
    std::vector<T> mKeys;
    std::vector<std::unique_ptr<Node<T>>> mChildren;
};