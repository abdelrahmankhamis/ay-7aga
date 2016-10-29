#include "adddialog.h"
#include "ui_adddialog.h"
#include <QDesktopWidget>
#include "mainwindow.h"
#include <QLayout>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    QWidget::setFixedSize(width(), height());

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    connect(ui->pushAdd, SIGNAL(clicked()), this, SLOT(Insert()));
    connect(ui->editFullname ,SIGNAL(textChanged(const QString &)), this, SLOT(Reset(const QString &)));
    connect(ui->editId ,SIGNAL(textChanged(const QString &)), this, SLOT(Reset(const QString &)));

    ui->pushAdd->setEnabled(false);
}

void AddDialog::Reset(const QString &)
{
    if (ui->editFullname->text().size() > 0 &&
            ui->editId->text().size() > 0)
    {
        tempUint = atoi(ui->editId->text().toLocal8Bit().data());

        if (strcmp(QString::number(tempUint).toLocal8Bit().data(),
                   ui->editId->text().toLocal8Bit().data()) == 0 &&
                tempUint > 0)
            ui->pushAdd->setEnabled(true);
        else
            ui->pushAdd->setEnabled(false);
    }
    else
        ui->pushAdd->setEnabled(false);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::SetModel(StudentModel *model)
{
    this->model=model;
}

void AddDialog::Insert()
{
    if (!model->Insert(
                ui->editId->text().toUInt(),
                ui->editFullname->text().toLocal8Bit().data()))
    {
        QMessageBox::critical(this, "Error", "Inserting new student failed!\nTry again.", QMessageBox::Ok);
        ui->pushAdd->setEnabled(false);
        ui->editFullname->setText("");
        ui->editFullname->setFocus();
        ui->editId->setText("");
    }
    else
    {
        ui->pushAdd->setEnabled(false);
        ui->editFullname->setText("");
        ui->editFullname->setFocus();
        ui->editId->setText("");
        close();
    }
}



