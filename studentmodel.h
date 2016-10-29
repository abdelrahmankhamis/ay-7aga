#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H

#include <QAbstractTableModel>
#include <QToolBar>
#include "studenttree.h"

#define HEADER_COLUMN_0 "ID"
#define HEADER_COLUMN_1 "Full Name"

class StudentModel: public QAbstractTableModel
{
    Q_OBJECT

    StudentTree* dataTree;
    bool tempBool;
    unsigned int tempUint;
    char tempChar[10];
    char* pTempChar;

public:
    StudentModel();
    ~StudentModel();

    enum
    {
        ID = 0,
        FULLNAME = 1
    };

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRow(int row, const QModelIndex & parent = QModelIndex());

    bool Insert(unsigned int id, char* fullname);

    void SetMode(int mode);
    unsigned int Size();

    void Search(int id);
    void Search(char* fullname);

    bool OpenFile(char* filename, int mode);
    bool SaveFile(char* filename, int mode);
};

#endif // STUDENTMODEL_H
