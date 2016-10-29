#ifndef STUDENTTREE_H
#define STUDENTTREE_H

#define BST_MODE_ID     1
#define BST_MODE_NAME   2

#include "structs.h"
#include "linkedlist.h"
#include <QAbstractTableModel>


#define VIEW_ORDER_PRE  1
#define VIEW_ORDER_IN   2
#define VIEW_ORDER_POST 4
#define SORT_BY_ID      8
#define SORT_BY_NAME    16

#define DATA_GLOCAL_ALLOC   1
#define DATA_USER_ALLOC     2
#define DATA_NO_ALLOC       3

#define BST_ID  false
#define BST_NAME !BST_ID

class StudentTree
{

private:
    TREE *bstId, *bstName, *viewTree;
    TREE_NODE*      tempNode;
    TREE_NODE_DATA* tempData;
    char*           tempChar;
    TREE_NODE**     pTempNode;
    unsigned int    tempUint;
    LinkedList*     tempLList;
    unsigned int    treeSize;
    LinkedList*     viewList;
    QAbstractTableModel* model;
    int treeMode;

    TREE_NODE** smallestNode;
    TREE_NODE_DATA* smallestNodeData;

    char* fileBuffer;
    unsigned int fileBufferSize;
    FILE* fileObj;

    void        InsertSortedId(TREE_NODE** root, TREE_NODE* node);
    void        InsertSortedName(TREE_NODE** root, TREE_NODE* node);

    TREE_NODE*  NewNode(TREE_NODE_DATA* data);
    void        DeleteNode(TREE_NODE** node, bool deleteData=false, bool bstName=false);
    TREE_NODE** SmallestNode(TREE_NODE** node);

    void        GeneratePreOrder(TREE_NODE* root, bool reflect=true, LinkedList* list=0);
    void        GenerateInOrder(TREE_NODE* root, bool reflect=true, LinkedList* list=0);
    void        GeneratePostOrder(TREE_NODE* root, bool reflect=true, LinkedList* list=0);

    bool        ValidFullname(char* fullname, unsigned int* len=0);

    TREE_NODE** Search(TREE_NODE** root, unsigned int Id);
    TREE_NODE** Search(TREE_NODE **root, char* fullname);

    unsigned int GetfileSize(const char * filename);
    void        ParseFile();

    void        Balance(int min, int max, LinkedList* list, bool bstId);

    void        DeleteTree(TREE_NODE** root, bool nodes=false, bool data=false);
public:
    StudentTree(QAbstractTableModel* model);
    ~StudentTree();

    unsigned int Size();
    TREE_NODE*  Get(unsigned int index);
    bool        Insert(unsigned int id, char* fullname, unsigned int flags=0, bool reorder=false);
    void        SetMode(int mode);

    void        Delete(unsigned int index);
    void        Delete(TREE_NODE* node, unsigned int flags);

    TREE_NODE** Search(unsigned int id, bool reflect);
    TREE_NODE** Search(char* fullname, bool reflect);

    void        SearchSub(char* fullname);

    bool        Set(unsigned int index, unsigned int id);
    bool        Set(unsigned int index, char* fullname);

    bool        Exists(unsigned int id);

    bool        LoadFromFile(char* filename, int mode);
    bool        SaveToFile(char* filename, int mode);

    void        Balance();
};

#endif // STUDENTTREE_H
