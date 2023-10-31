#include "include/btree.h"

int main(int argc, char** argv)
{
    // example
    BTree<int> tree(3);
    tree.Insert(5).Insert(3)
        .Insert(7).Insert(4)
        .Insert(2).Insert(6)
        .Insert(8).Insert(1)
        .Insert(9).Insert(10)
        .Print();

    return 0;
}