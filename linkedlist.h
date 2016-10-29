#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "structs.h"

class LinkedList
{
    LINKEDLIST* list;
    LINKEDLIST_NODE* NewNode(TREE_NODE* node);

    LINKEDLIST_NODE* tempNode;
    unsigned int tempInt;

    void    InitIterator();
    struct
    {
        unsigned int index;
        LINKEDLIST_NODE* node;
    } iterator;

public:
    LinkedList();
    ~LinkedList();

    void    InsertFirst(TREE_NODE* node);
    void    InsertLast(TREE_NODE* node);
    void    RemoveFirst();
    void    RemoveLast();
    void    RemoveIndex(unsigned int index);
    void    Free();
    bool    Empty();
    unsigned int Size();
    TREE_NODE* First();
    TREE_NODE* Last();


    TREE_NODE*  IteratorReset();
    unsigned int IteratorCurrentIndex();
    TREE_NODE*  IteratorCurrent();
    TREE_NODE*  IteratorInc();
    TREE_NODE*  IteratorDec();
    bool        IteratorBegin();
    bool        IteratorEnd();
    TREE_NODE*  IteratorGoTo(unsigned int index);
};

#endif // LINKEDLIST_H
