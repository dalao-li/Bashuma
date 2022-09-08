#include "mainwindow.h"

#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.setWindowTitle("八数码");
    //设置窗口左上角图标
    w.setWindowIcon(QIcon(":eight.ico"));
    w.show();

    return a.exec();
}
