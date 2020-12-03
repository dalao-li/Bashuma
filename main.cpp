#include "mainwindow.h"

#include <QApplication>
bool flag;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("八数码");
    w.show();
    return a.exec();
}
