#pragma once
#include <memory>
#include <vector>

template <typename T>
class Node {
public:
    Node(int size): max_key{size}
    {
        keys.reserve(N+1);
    }
    void insert(T elem)
    {
        size_t idx = 0;
    }
    size_t keys_size()
    {
        return keys.size();
    }
    size_t max_key_size()
    {
        return max_key;
    }
private:
    size_t max_key;
    std::vector<T> keys;
    std::vector<std::unique_ptr<Node<T>>> children;
};

