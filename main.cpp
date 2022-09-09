/*
 * @Description:
 * @Version: 1.0
 * @Author: DaLao
 * @Email: dalao_li@163.com
 * @Date: 2022-09-08 23:03:10
 * @LastEditors: DaLao
 * @LastEditTime: 2022-09-09 23:47:58
 */
#include "include/mainwindow.h"

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
