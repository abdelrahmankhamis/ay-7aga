#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTableWidget>
#include <QActionGroup>
#include <QSignalMapper>
#include <QToolBar>
#include <QToolButton>
#include "aboutdialog.h"
#include "studentmodel.h"
#include <QRadioButton>
#include <QLineEdit>
#include "adddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void aboutDialog();
    void changeViewOrder(int orderType);
    void changeViewSort(int sortType);
    void editMenu();
    void deleteMenu();
    void addMenu();
    void searchMenu();
    void resetMenu(const QString &);
    void openMenu();
    void saveMenu();

protected slots:
    void keyPressEvent(QKeyEvent *k);

private:
    int orderMode;
    int sortMode;
    QModelIndexList indexList;
    QLineEdit* editLine;
    QRadioButton *radioId, *radioName;

    Ui::MainWindow *ui;
    void setupStatusbar();
    void setupToolbar();
    void setupTableRecords();
    void setupEnvironment();
    void setupMenubar();
    void checkEnability(bool);

    bool dataSaved;
    QTableView* table;
    StudentModel* tableModel;
    QToolBar* toolbar;
    QActionGroup* orderGroup, *sortGroup;
    QSignalMapper* orderSignalMapper, *sortSignalMapper;

    bool checkSaveMessage();

    AboutDialog* about;
    AddDialog* add;

protected:
    void resizeEvent(QResizeEvent * event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
