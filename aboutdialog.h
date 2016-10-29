#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    void setAsSplash(int ms);
    
private:
    Ui::AboutDialog *ui;
    bool isTimer;

protected:
    void mouseReleaseEvent(QMouseEvent *);

};

#endif // ABOUTDIALOG_H
