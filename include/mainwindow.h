/*
 * @Description:
 * @Version: 1.0
 * @Author: DaLao
 * @Email: dalao_li@163.com
 * @Date: 2022-09-08 23:03:10
 * @LastEditors: DaLao
 * @LastEditTime: 2022-09-10 14:49:44
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/Game.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QTextBrowser>
#include <QMessageBox>
#include <QDebug>
#include <QTime>

#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    // 清除输入
    void clearLine(QLineEdit *a[9]);

    // 设置默认输入
    void setLine(QString str, QLineEdit *a[9]);

    // 设置输入框状态
    void setLineStatus(QLineEdit *a[9], bool flag);

    // 将九宫格输入转化为字符串
    QString getLineValue(QLineEdit *a[9]);

    // 产生随机字符串
    std::string getRandomStr();

    // 延时函数
    void wait(int times);

    // 输出路径
    void ouputPath(int num);

    // 输出open与close表
    void ouputTable(QTextBrowser text, std::vector<std::string> v);

    // 判断LineEdit的合法性
    bool judgeInput(QString s);

private slots:

    void on_autoOuputBtn_clicked();

    void on_autoInputBtn_clicked();

    void on_clearBtn_clicked();

    void on_find_pathBtn_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pathTextBrowser_sourceChanged(const QUrl &arg1);

    void on_manuInputBtn_clicked();

    void on_afterPathBtn_clicked();

    void on_nextPathBtn_clicked();

    void on_closeBtn_clicked();

protected:
    Ui::MainWindow *ui;

    QLineEdit *originLine[9], *endLine[9];

    Game m_game;

    QString m_start_input;

    QString m_end_input;

    // 记录当前时第几步
    int m_step = 0;
};

#endif
