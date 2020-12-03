#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bashuma.h"
#include <QLineEdit>
#include<QTextBrowser>
#include<vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Game game = *new Game("","");

    QString str1,str2;
    //判断输入合法性
    bool judegInput(QLineEdit *a[9]);

    //清除输入
    void clearLineEdit(QLineEdit *a[9]);

    //设置默认输入
    void setLineEdit(QString str,QLineEdit *a[9]);

    //将输入转化为字符串
    QString getString(QLineEdit *a[9]);

    //输出路径
    void ouputPath(QString signlePath);

    //输出open与close表
    void ouputTable(QTextBrowser text,vector <string> v);


private slots:
    void on_strart_pushButton_clicked();

    void on_random_pushButton_clicked();

    void on_clear_pushButton_clicked();

    void on_sigleStep_pushButton_clicked();

    void on_creatPath_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QLineEdit *orign_line[9],*end_line[9];
};

#endif // MAINWINDOW_H
