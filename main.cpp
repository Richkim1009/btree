#include "include/btree.h"

int main(int argc, char** argv)
{
    // example
    BTree<int> tree;
    if (!tree.Initialize(-2)) {
        std::cerr << "Invalid degree \n";
        return 1;
    }
    tree.Insert(5).Insert(3).Insert(4).Insert(2).Insert(1).Insert(7).Insert(8)
    .Delete(7).Delete(8).Delete(2)
    .Print();
 
    return 0;
}