#include "studenttree.h"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "misc.h"
#include <math.h>

StudentTree::StudentTree(QAbstractTableModel* model)
{
    bstId = (TREE*)malloc(sizeof(TREE));
    bstId->root = 0;

    bstName = (TREE*)malloc(sizeof(TREE));
    bstName->root = 0;

    treeSize = 0;

    viewList = new LinkedList();
    tempLList = new LinkedList();
    this->model = model;

    treeMode = SORT_BY_ID | VIEW_ORDER_PRE;

    fileBuffer = (char*)malloc(0);
    fileBufferSize = 0;
    fileObj = 0;
}

void StudentTree::Balance()
{
    tempLList->Free();
    GenerateInOrder(bstId->root, false, tempLList);
    DeleteTree(&(bstId->root));
    Balance(0, tempLList->Size()-1, tempLList, true);

    tempLList->Free();
    GenerateInOrder(bstName->root, false, tempLList);
    DeleteTree(&(bstName->root));
    Balance(0, tempLList->Size()-1, tempLList, false);
}

void StudentTree::Balance(int min, int max, LinkedList* list, bool bstId)
{
    if (min <= max)
    {
        list->IteratorReset();

        int middleNode = (int)ceil(((double)min + max) / 2);
        TREE_NODE* node = list->IteratorGoTo(middleNode);

        //*node->data->bstId = *node->data->bstName = 0;
        //node->data->bstId = node->data->bstName = 0;
        node->left = node->right = 0;

        if (bstId)
        {
            node->data->bstId = 0;
            InsertSortedId(&(this->bstId->root), node);
        }
        else
        {
            node->data->bstName = 0;
            InsertSortedName(&(this->bstName->root), node);
        }

        Balance(min, middleNode - 1, list, bstId);
        Balance(middleNode + 1, max, list, bstId);
    }
}

void StudentTree::DeleteTree(TREE_NODE** root, bool nodes, bool data)
{
    if (!root || !*root) return;
    DeleteTree(&(*root)->left, nodes, data);
    DeleteTree(&(*root)->right, nodes, data);

    if (nodes)
    {
        if (data)
            free((*root)->data);
        free(*root);
    }
    else
    {
        if (data)
            free((*root)->data);
        (*root) = 0;
    }

}


TREE_NODE** StudentTree::Search(unsigned int id, bool reflect)
{
    pTempNode = Search(&(bstId->root), id);
    if (pTempNode)
    {
        if (reflect)
        {
            viewList->Free();
            viewList->InsertLast(*pTempNode);
        }
    }
    else
    {
        if (reflect)
            viewList->Free();
    }

    return pTempNode;
}

TREE_NODE** StudentTree::Search(char* fullname, bool reflect)
{
    pTempNode = Search(&(bstName->root), fullname);
    if (pTempNode)
    {
        if (reflect)
        {
            viewList->Free();
            viewList->InsertLast(*pTempNode);
        }
        else
        {
            if (reflect)
                viewList->Free();
        }
    }

    return pTempNode;
}

TREE_NODE** StudentTree::Search(TREE_NODE** root, unsigned int Id)
{
    if(!root || !*root)
        return 0;

    else if((*root)->data->id == Id)
        return root;

    else if ((*root)->data->id < Id)
        return Search(&((*root)->right),Id);

    else
        return Search(&((*root)->left),Id);
}

TREE_NODE** StudentTree::Search(TREE_NODE **root, char* fullname)
{
    if(!root || !*root)
        return 0;

    else if(strcasecmp((*root)->data->fullname, fullname) == 0)
        return root;

    else if (strcmp((*root)->data->fullname,fullname) < 0)
        return Search(&((*root)->right),fullname);

    else
        return Search(&((*root)->left),fullname);
}

bool StudentTree::LoadFromFile(char* filename, int mode)
{
    //UnloadFile();

    fileBufferSize = 0;

    tempUint = GetfileSize(filename);
    if (!tempUint)
        return false;

    fileBuffer = (char*)realloc(fileBuffer, tempUint * sizeof(char));

    if (!fileBuffer)
        return false;

    fileObj = fopen(filename, "rw");
    if (!fileObj)
        return false;

    fileBufferSize = fread(fileBuffer, sizeof (char), tempUint, fileObj);

    if (fileBufferSize != tempUint)
    {
        fileBuffer = (char*)realloc(fileBuffer, 0);
        return false;
    }

    tempUint = fclose(fileObj);
    if (tempUint)
        return false;

    ParseFile();
    Balance();
    SetMode(mode);

    return true;
}

unsigned int StudentTree::GetfileSize(const char * filename)
{
    struct stat sb;
    if (stat (filename, & sb) != 0)
        return 0;

    return sb.st_size;
}

void StudentTree::ParseFile()
{
    if (!fileBufferSize) return;

    unsigned int i, strings_size=0;
    char** strings = (char**) malloc(strings_size * sizeof(char*));

    for (i=0; i<fileBufferSize; i++)
    {
        if (*(fileBuffer+i) == ',' || *(fileBuffer+i) == '\n')
        {
            *(fileBuffer+i) = '\0';
            if (strings_size == 0 && i != 0)
            {
                strings = (char**)realloc(strings, ++strings_size * sizeof(char*));
                *(strings+strings_size-1) = fileBuffer;

                if (i+1 < fileBufferSize)
                {
                    strings = (char**)realloc(strings, ++strings_size * sizeof(char*));
                    *(strings+strings_size-1) = fileBuffer+i+1;
                }
            }
            else if (i+1 < fileBufferSize)
            {
                strings = (char**)realloc(strings, ++strings_size * sizeof(char*));
                *(strings+strings_size-1) = fileBuffer+i+1;
            }
        }
    }

    for (i=0; i<strings_size/2; i++)
    {
        if (strlen(*(strings+i*2+1)) > 10)
            continue;

        if (strcmp(QString::number(atoi(*(strings+i*2+1))).toLocal8Bit().data(),
                   *(strings+i*2+1)) != 0)
            continue;

        if (ValidFullname(*(strings+i*2)))
            Insert(atoi(*(strings+i*2+1)), *(strings+i*2));
    }

    free(strings);
}

bool StudentTree::SaveToFile(char* filename, int mode)
{
    fileObj = fopen(filename, "w+");
    if (!fileObj) return false;

    SetMode(mode);

    for (tempUint=0; tempUint<Size(); tempUint++)
    {
        tempNode = Get(tempUint);
        fprintf(fileObj,
                "%s,%d\n",
                tempNode->data->fullname,
                tempNode->data->id);
    }

    fclose(fileObj);
    return true;
}

bool StudentTree::Exists(unsigned int id)
{
    pTempNode = Search(&(bstId->root),id);
    return pTempNode? true: false;
}

bool StudentTree::Set(unsigned int index, unsigned int id)
{
    if ((int)id <= 0 || Exists(id))
        return false;

    viewList->IteratorReset();
    tempNode = viewList->IteratorGoTo(index);
    if (tempNode)
    {
        tempNode->data->id = id;
        return true;
    }

    return false;
}

bool StudentTree::Set(unsigned int index, char* fullname)
{
    viewList->IteratorReset();
    tempNode = viewList->IteratorGoTo(index);

    if (tempNode)
    {
        if(!ValidFullname(fullname, &tempUint))
            return false;

        if (tempNode->data->flags & DATA_USER_ALLOC)
            tempNode->data->fullname = (char*)realloc(tempNode->data->fullname, tempUint + 1);
        else
            tempNode->data->fullname = (char*)malloc(tempUint + 1);

        strcpy(tempNode->data->fullname, fullname);

        tempNode->data->flags ^= (DATA_GLOCAL_ALLOC | DATA_NO_ALLOC);
        tempNode->data->flags |= DATA_USER_ALLOC;

        return true;
    }

    return false;
}

bool StudentTree::ValidFullname(char* fullname, unsigned int* len)
{
    tempUint = strlen(fullname);
    RemoveComma(fullname, 0, tempUint);
    RemoveExtraSpaces(fullname);
    Trim(fullname);
    Capitalize(fullname);
    tempUint = strlen(fullname);

    for(unsigned int i=0; i<tempUint; i++)
        if (isdigit(*(fullname + i)) || !isalpha(*(fullname + i)))
        {
            if (*(fullname + i) == ' ')
                continue;
            else
                return false;
        }

    return tempUint?(len?(*len = tempUint):true):false;
}

void StudentTree::Delete(unsigned int index)
{
    tempNode = Get(index);

    //tempUint = tempNode->data->id;
    //tempChar = tempNode->data->fullname;

    //pTempNode = Search(tempUint, false);
    //DeleteNode(pTempNode);

    //pTempNode = Search(tempChar, false);
    //DeleteNode(pTempNode);

    TREE_NODE_DATA* data = tempNode->data;

    DeleteNode(data->bstId, false, BST_ID);
    DeleteNode(data->bstName, false, BST_NAME);

    SetMode(treeMode);
}

void StudentTree::SearchSub(char* fullname)
{
    SetMode(treeMode);

    tempLList->Free();
    viewList->IteratorReset();

    for(tempUint=0; tempUint<viewList->Size(); tempUint++)
    {
        tempNode = viewList->IteratorGoTo(tempUint);
        if(strcasestr(tempNode->data->fullname, fullname))
            tempLList->InsertLast(tempNode);
    }

    viewList->Free();
    tempLList->IteratorReset();
    for(tempUint=0; tempUint<tempLList->Size(); tempUint++)
        viewList->InsertLast(tempLList->IteratorGoTo(tempUint));

    tempLList->Free();
}

unsigned int StudentTree::Size()
{
    return viewList->Size();
}

TREE_NODE* StudentTree::Get(unsigned int index)
{
    viewList->IteratorReset();
    return viewList->IteratorGoTo(index);
}

TREE_NODE * StudentTree::NewNode(TREE_NODE_DATA *data)
{
    tempNode =(TREE_NODE*)malloc(sizeof(TREE_NODE));
    tempNode->data = data;
    tempNode->left = tempNode->right = 0;
    return tempNode;
}

bool StudentTree::Insert(unsigned int id, char* fullname, unsigned int flags, bool reorder)
{
    if ((int)id <= 0 || Exists(id))
        return false;

    if(!ValidFullname(fullname))
        return false;

    tempData = (TREE_NODE_DATA*)malloc(sizeof(TREE_NODE_DATA));
    tempData->id = id;
    tempData->fullname = fullname;
    tempData->flags = flags;
    tempData->bstId = tempData->bstName = 0;

    tempNode = NewNode(tempData);
    InsertSortedId(&(bstId->root), tempNode);

    tempNode = NewNode(tempData);
    InsertSortedName(&(bstName->root), tempNode);

    if (reorder)
        SetMode(treeMode);

    return true;
}

TREE_NODE** StudentTree::SmallestNode(TREE_NODE** node)
{
    if (!(node) || !(*node)) return 0;
    if (!(*node)->left) return node;
    else return SmallestNode(&(*node)->left);
}

void StudentTree::DeleteNode(TREE_NODE** node, bool deleteData, bool bstName)
{
    if (!node || !(*node)) return;
    if (!(*node)->left && !(*node)->right)
    {
        if (deleteData)
            free((*node)->data);
        else
        {
            if (bstName)
                (*node)->data->bstName = 0;
            else
                (*node)->data->bstId = 0;
        }

        free(*node);
        *node = 0;
    }
    else if ((*node)->left && (*node)->right)
    {
        smallestNode = SmallestNode(&(*node)->right);
        smallestNodeData = (*smallestNode)->data;

        //free(*smallestNode);
        //*smallestNode = 0;

        DeleteNode(smallestNode, deleteData, bstName);

        if (deleteData)
        {
            if ((*node)->data->flags & DATA_USER_ALLOC)
                free((*node)->data->fullname);
            free((*node)->data);
        }
        else
        {
            if (bstName)
                (*node)->data->bstName = 0;
            else
                (*node)->data->bstId = 0;
        }

        (*node)->data = smallestNodeData;

        if (bstName)
            (*node)->data->bstName = node;
        else
            (*node)->data->bstId = node;

        /*TREE_NODE** smallestTree = SmallestNode(&(*node)->right);
        TREE_NODE* orgNode = *smallestTree;

        if (deleteData)
        {
            if ((*node)->data->flags & DATA_USER_ALLOC)
                free((*node)->data->fullname);
            free((*node)->data);
        }

        (*node)->data = (orgNode)->data;

        if (bstName)
            (*node)->data->bstName = node;
        else
            (*node)->data->bstId = node;

        if (orgNode->right)
        {
            if (bstName)
                orgNode->right->data->bstName = orgNode->data->bstName;
            else
                orgNode->right->data->bstId = orgNode->data->bstId;
            *smallestTree = (*smallestTree)->right;
        }
        else
            *smallestTree = 0;

        free(orgNode);*/
    }
    else
    {
        tempNode = (*node)->left?(*node)->left:(*node)->right;

        if (!bstName)
            tempNode->data->bstId = (*node)->data->bstId;
        else
            tempNode->data->bstName = (*node)->data->bstName;

        if (deleteData)
        {
            if ((*node)->data->flags & DATA_USER_ALLOC)
                free((*node)->data->fullname);
            free((*node)->data);
        }

        free(*node);
        *node = tempNode;
    }
}

void StudentTree::InsertSortedName(TREE_NODE** root, TREE_NODE* node)
{
    if (!*root)
    {
        *root = node;
        node->data->bstName = root;
    }
    else if (strcmp((*root)->data->fullname, node->data->fullname) < 0)
    {
        if (!(*root)->right)
        {
            (*root)->right = node;
            node->data->bstName = &((*root)->right);
        }
        else
            InsertSortedName(&((*root)->right), node);
    }
    else
    {
        if (!(*root)->left)
        {
            (*root)->left = node;
            node->data->bstName = &((*root)->left);
        }
        else
            InsertSortedName(&((*root)->left), node);
    }
}

void StudentTree::InsertSortedId(TREE_NODE** root, TREE_NODE* node)
{
    if (!*root)
    {
        *root = node;
        node->data->bstId = root;
    }
    else if ((*root)->data->id <= node->data->id)
    {
        if (!(*root)->right)
        {
            (*root)->right = node;
            node->data->bstId = &((*root)->right);
        }
        else
            InsertSortedId(&((*root)->right), node);
    }
    else
    {
        if (!(*root)->left)
        {
            (*root)->left = node;
            node->data->bstId = &((*root)->left);
        }
        else
            InsertSortedId(&((*root)->left), node);
    }
}

StudentTree::~StudentTree()
{
    free(bstId);
    free(bstName);
    delete viewList;
}

void StudentTree::SetMode(int mode)
{
    treeMode = mode;

    if (mode & SORT_BY_ID)
        viewTree = bstId;
    else if (mode & SORT_BY_NAME)
        viewTree = bstName;

    viewList->Free();

    if (mode & VIEW_ORDER_PRE)
        GeneratePreOrder(viewTree->root);
    else if (mode & VIEW_ORDER_IN)
        GenerateInOrder(viewTree->root);
    else if (mode & VIEW_ORDER_POST)
        GeneratePostOrder(viewTree->root);
}

void StudentTree::GeneratePreOrder(TREE_NODE* root, bool reflect, LinkedList* list)
{
    if (!root) return;

    if (reflect)
        viewList->InsertLast(root);
    else if (!reflect && list)
        list->InsertLast(root);

    GeneratePreOrder(root->left, reflect, list);
    GeneratePreOrder(root->right, reflect, list);
}

void StudentTree::GenerateInOrder(TREE_NODE* root, bool reflect, LinkedList* list)
{
    if (!root) return;
    GenerateInOrder(root->left, reflect, list);

    if (reflect)
        viewList->InsertLast(root);
    else if (!reflect && list)
        list->InsertLast(root);

    GenerateInOrder(root->right, reflect, list);
}

void StudentTree::GeneratePostOrder(TREE_NODE* root, bool reflect, LinkedList* list)
{
    if (!root) return;
    GeneratePostOrder(root->left, reflect, list);
    GeneratePostOrder(root->right, reflect, list);

    if (reflect)
        viewList->InsertLast(root);
    else if (!reflect && list)
        list->InsertLast(root);
}
