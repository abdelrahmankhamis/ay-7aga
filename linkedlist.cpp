#include "linkedlist.h"
#include <cstdlib>

LinkedList::LinkedList()
{
    list = (LINKEDLIST*)malloc(sizeof(LINKEDLIST));
    list->head = 0;
    list->tail = 0;

    InitIterator();
}

void LinkedList::InsertFirst(TREE_NODE* treeNode)
{
    tempNode = NewNode(treeNode);

    if (list->head)
        list->head->prev = tempNode;

    tempNode->prev = 0;
    tempNode->next = list->head;
    list->head = tempNode;

    if (!list->tail)
        list->tail = tempNode;

    IteratorReset();
}

void LinkedList::InsertLast(TREE_NODE* treeNode)
{
    tempNode = NewNode(treeNode);

    if (list->tail)
        list->tail->next = tempNode;

    tempNode->prev = list->tail;
    tempNode->next = 0;
    list->tail = tempNode;

    if (!list->head)
        list->head = tempNode;

    IteratorReset();
}

void LinkedList::RemoveFirst()
{
    if (!list->head)
        return;

    tempNode = list->head;
    list->head = list->head->next;

    if (list->head)
        list->head->prev = 0;

    free(tempNode);

    if (!list->head)
        list->tail = 0;

    IteratorReset();
}

void LinkedList::RemoveLast()
{
    if (!list->tail)
        return;

    tempNode = list->tail;
    list->tail = list->tail->prev;

    if (list->tail)
        list->tail->next = 0;

    free(tempNode);

    if (!list->tail)
        list->head = 0;

    IteratorReset();
}

unsigned int LinkedList::Size()
{
    //IteratorReset();
    while(!IteratorEnd() && IteratorInc());
    return IteratorCurrentIndex() +
            ((!IteratorReset() && !IteratorCurrentIndex())?0:1);
}

TREE_NODE* LinkedList::First()
{
    return list->head?list->head->data:0;
}

TREE_NODE* LinkedList::Last()
{
    return list->tail?list->tail->data:0;
}

LINKEDLIST_NODE* LinkedList::NewNode(TREE_NODE* node)
{
    tempNode = (LINKEDLIST_NODE*)malloc(sizeof(LINKEDLIST_NODE));
    tempNode->next = tempNode->prev = 0;
    tempNode -> data = node;
    return tempNode;
}

bool LinkedList::Empty()
{
    return !list->head;
}

void LinkedList::Free()
{
    while(!Empty())
        RemoveLast();

    IteratorReset();
}

TREE_NODE* LinkedList::IteratorGoTo(unsigned int index)
{
    if (index == IteratorCurrentIndex())
        return IteratorCurrent();
    else if (index < IteratorCurrentIndex())
    {
        while(!IteratorBegin() && IteratorDec() && index != IteratorCurrentIndex());
        return (IteratorCurrent() &&
                index == IteratorCurrentIndex())?
                    IteratorCurrent():0;
    }
    else
    {
        while(!IteratorEnd() && IteratorInc() && index != IteratorCurrentIndex());
        return (IteratorCurrent() &&
                index == IteratorCurrentIndex())?
                    IteratorCurrent():0;
    }
}

void LinkedList::InitIterator()
{
    iterator.index = 0;
    iterator.node = list->head;
}

TREE_NODE* LinkedList::IteratorReset()
{
    iterator.index = 0;
    iterator.node = list->head;
    return IteratorCurrent();
}

unsigned int LinkedList::IteratorCurrentIndex()
{
    return iterator.index;
}

TREE_NODE* LinkedList::IteratorCurrent()
{
    return iterator.node? iterator.node->data:0;
}

TREE_NODE* LinkedList::IteratorInc()
{
    if (iterator.node && iterator.node->next)
    {
        iterator.index++;
        iterator.node = iterator.node->next;
        return IteratorCurrent();
    }

    return 0;
}

TREE_NODE* LinkedList::IteratorDec()
{
    if (iterator.node->prev)
    {
        iterator.index--;
        iterator.node = iterator.node->prev;
        return IteratorCurrent();
    }

    return 0;
}

bool LinkedList::IteratorEnd()
{
    return IteratorCurrent() == (list->tail?list->tail->data:0);
}

bool LinkedList::IteratorBegin()
{
    return IteratorCurrent() == (list->head?list->head->data:0);
}

LinkedList::~LinkedList()
{
    Free();
    free(list);
}
