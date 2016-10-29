#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QDesktopWidget>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AboutDialog about;
    about.show();
    about.setAsSplash(3000);
    about.exec();

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
