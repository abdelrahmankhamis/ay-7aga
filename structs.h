#ifndef STRUCTS_H
#define STRUCTS_H

struct TREE_NODE;

struct TREE_NODE_DATA
{
    unsigned int id;
    char *fullname;
    unsigned int flags;
    TREE_NODE** bstId, **bstName;
};

struct TREE_NODE
{
    TREE_NODE_DATA *data;
    TREE_NODE *right;
    TREE_NODE * left;
};

struct TREE
{
    TREE_NODE *root;
};


struct LINKEDLIST_NODE
{
    TREE_NODE* data;
    LINKEDLIST_NODE *next, *prev;
};

struct LINKEDLIST
{
    LINKEDLIST_NODE* head, *tail;
};

#endif // STRUCTS_H
