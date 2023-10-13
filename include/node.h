#pragma once
#include <vector>
#include <memory>

template <typename T>
class Node {
public:
    Node(sizt_t degree): mDegree(degree) {}

    size_t getDegree() const {return mDegree;}
    std::vector<T>& getKeys() {return mkeys;}
    const std::vector<T>& getKeys() const {return mKeys;}
    std::vector<std::unique_ptr<Node<T>>>& getChildren() {return mChildren;}
    const std::vector<std::unique_ptr<Node<T>>>& getChildren() const {return mChildren;}

private:
    size_t mDegree;
    std::vector<Node<T>> mKeys;
    std::vector<std::unique_ptr<Node<T>>> mChildren;
};