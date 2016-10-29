#include "studentmodel.h"
#include <cstdlib>
#include <cctype>
#include <string>
#include <ostream>
#include <sstream>
#include <iostream>

using namespace std;

StudentModel::StudentModel(): QAbstractTableModel()
{
    dataTree = new StudentTree(this);

    /*for (int i=0; i<11; i++)
    {
        ostringstream convert;
        convert << i;
        string str = string("Anwar") + string(convert.str().c_str());
        char* strr = new char[str.size() + 1];
        strcpy(strr, str.c_str());
        dataTree->Insert(i, strr, 0);
    }*/



    //dataTree->Insert(1, "Anwar mohamed", 0);
    //dataTree->Insert(2, "Abdallah", 0);
    //dataTree->Insert(4, "Yasmine", 0);
    //dataTree->Insert(3, "Moataz", 0);
    //dataTree->Insert(5, "Moataz", 0);

    //for (int i=50; i<100; i++)
    //    dataTree->Insert(i, "Anwar");

    //for (int i=10; i<40; i++)
    //    dataTree->Insert(i, "Anwar");

    //dataTree->SetMode(SORT_BY_ID | VIEW_ORDER_IN);
}

bool StudentModel::Insert(unsigned int id, char *fullname)
{
    beginResetModel();
    tempBool = true;
    pTempChar = (char*)malloc(strlen(fullname) + 1);
    strcpy(pTempChar, fullname);

    if (!dataTree->Insert(id, pTempChar, DATA_USER_ALLOC, true))
    {
        free(pTempChar);
        tempBool = false;
    }

    endResetModel();
    return tempBool;
}

void StudentModel::Search(char* fullname)
{
    beginResetModel();
    dataTree->SearchSub(fullname);
    endResetModel();
}

void StudentModel::Search(int id)
{
    beginResetModel();
    dataTree->Search(id, true);
    endResetModel();
}


void StudentModel::SetMode(int mode)
{
    beginResetModel();
    dataTree->SetMode(mode);
    endResetModel();
}

unsigned int StudentModel::Size()
{
    return dataTree->Size();
}

int StudentModel::rowCount(const QModelIndex&) const
{
    return dataTree->Size();
}

int StudentModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid()? 0:2;
}

QVariant StudentModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if((unsigned int)index.row() >= dataTree->Size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case ID:
            return QString::number(dataTree->Get(index.row())->data->id);
        case FULLNAME:
            return tr(dataTree->Get(index.row())->data->fullname);
        }
    }
    return QVariant();
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return tr(HEADER_COLUMN_0);
            case 1:
                return tr(HEADER_COLUMN_1);
            }
        }
        else if(orientation == Qt::Vertical)
            return QString::number(section + 1);
    }

    return QVariant::Invalid;
}


bool StudentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && !((unsigned int)index.row() >= dataTree->Size() || index.row() < 0))
    {
        int row = index.row();

        switch(index.column())
        {
        case ID:
            if (value.toString().size() > 10)
                return false;

            tempUint = atoi(value.toString().toLocal8Bit().data());

            if (strcmp(QString::number(tempUint).toLocal8Bit().data(),
                       value.toString().toLocal8Bit().data()) != 0 ||
                    !dataTree->Set(row, tempUint))
                return false;
            break;
        case FULLNAME:
            if (!value.toString().size() || !dataTree->Set(row, value.toString().toLocal8Bit().data()))
                return false;
            break;
        default:
            return false;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags StudentModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool StudentModel::removeRow(int row, const QModelIndex&)
{
    if (row >= 0)
    {
        beginRemoveRows(QModelIndex(),row,row);
        dataTree->Delete(row);
        endRemoveRows();
    }

    return true;
}

bool StudentModel::OpenFile(char* filename, int mode)
{
    beginResetModel();
    tempBool = dataTree->LoadFromFile(filename, mode);
    endResetModel();
    return tempBool;
}

bool StudentModel::SaveFile(char* filename, int mode)
{
    return dataTree->SaveToFile(filename, mode);
}

StudentModel::~StudentModel()
{

}
