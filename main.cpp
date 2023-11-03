#include "include/btree.h"

int main(int argc, char** argv)
{
    // example
    BTree<int> tree(3);
    tree.Insert(5).Insert(3).Insert(4).Insert(2).Insert(1).Insert(7).Insert(8)
    .Delete(7).Delete(8).Delete(2)
    .Print();
    // BTree<int> tree(3);
    // tree.Insert(5).Insert(3).Insert(4).Delete(3).Print();

    return 0;
}