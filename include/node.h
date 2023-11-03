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
            const size_t currentIdx = findKeyIndex(key);
            if (mChildren.empty()) {
                mKeys.erase(mKeys.begin()+currentIdx);
            } else {
                T tmp = mChildren[currentIdx]->GetPredecessorKey();
                mKeys[currentIdx] = std::move(tmp);
                if (mChildren[currentIdx]->GetKeys().size() < minimumKeySize()) {
                    balaced(currentIdx);
                }
            }
        } else {
            const size_t uppderIdx = findUpperIndex(key);
            const size_t lowerIdx = findLowerIndex(key);
            mChildren[uppderIdx]->Delete(key);
            if (mChildren[uppderIdx]->GetKeys().size() < minimumKeySize()) {
                balaced(uppderIdx-lowerIdx);
            }
        }
    }

    T GetPredecessorKey()
    {
        return getPredecessorKey();
    }

    void Balanced(size_t idx)
    {
        balaced(idx);
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

    void balaced(size_t idx)
    {
        if (idx == 0) {
            if (mChildren[idx+1]->GetKeys().size() > minimumKeySize()) {
                borrowKeyFromRightSibling(idx);
            } else {
                borrowKeyFromParent(idx);
            }
        } else if (idx == mChildren.size() - 1) {
            if (mChildren[idx-1]->GetKeys().size() > minimumKeySize()) {
                borrowKeyFromLeftSibling(idx);
            } else {
                borrowKeyFromParent(idx);
            }
        } else {
            if (mChildren[idx+1]->GetKeys().size() > minimumKeySize()) {
                borrowKeyFromRightSibling(idx);
            } else if (mChildren[idx-1]->GetKeys().size() > minimumKeySize()) {
                borrowKeyFromLeftSibling(idx);
            } else {
                borrowKeyFromParent(idx);
            }
        }
    }

    T getPredecessorKey()
    {
        if (mChildren.empty()) {
            T tmp = std::move(mKeys.back());
            mKeys.pop_back();
            return tmp;
        }
        const size_t idx = mChildren.size() - 1;
        const size_t currentIdx = mKeys.size() - 1;
        T tmp = mChildren[idx]->GetPredecessorKey();
        balaced(idx);
        
        return tmp;
    }

    void borrowKeyFromRightSibling(size_t idx)
    {
        auto & childNode = mChildren[idx+1];
        auto & childKeys = childNode->GetKeys();
        auto & childChildren = childNode->GetChildren();
        mKeys.emplace_back(std::move(childKeys[0]));
        childKeys.erase(childKeys.begin());
        if (!childChildren.empty()) {
            mChildren.emplace_back(std::move(childChildren[0]));
            childChildren.erase(childChildren.begin());
        }
    }

    void borrowKeyFromLeftSibling(size_t idx)
    {
        auto & childNode = mChildren[idx-1];
        auto & childKeys = childNode->GetKeys();
        auto & childChildren = childNode->GetChildren();
        mKeys.insert(mKeys.begin(), std::move(childKeys[childKeys.size()-1]));
        childKeys.pop_back();
        if (!childChildren.empty()) {
            mChildren.insert(mChildren.begin(), std::move(childChildren[childChildren.size()-1]));  
            childChildren.pop_back();
        }
    }
    void borrowKeyFromParent(size_t idx)
    {
        auto currentNode = mKeys[idx];
        auto & leftChildKeys = mChildren[idx]->GetKeys();
        if (idx + 1 < mChildren.size()) {
            auto & leftChildChildren = mChildren[idx]->GetChildren();
            auto & rightChildKeys = mChildren[idx+1]->GetKeys();
            auto & rightChildChildren = mChildren[idx+1]->GetChildren();
            if (rightChildChildren.size() != 0) {
                leftChildChildren.insert(leftChildChildren.end(), 
                    std::make_move_iterator(rightChildChildren.begin()), 
                    std::make_move_iterator(rightChildChildren.end())
                );
            }
            leftChildKeys.emplace_back(currentNode);
            leftChildKeys.insert(leftChildKeys.end(), rightChildKeys.begin(), rightChildKeys.end());
            
        } else {
            leftChildKeys.emplace_back(currentNode);
        }
        
        mChildren.erase(mChildren.begin()+idx+1);
        mKeys.erase(mKeys.begin()+idx);
    }

    std::unique_ptr<Node<T>> split(Node& node)
    {
        std::unique_ptr<Node<T>> ptr = nullptr;
        if (node.GetKeys().size() == node.GetDegree()) {
            size_t mid = mKeys.size() == 0 ? 0 : (mKeys.size()-1)/2;
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