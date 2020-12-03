#include <QSet>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bashuma.h"
#include <QDebug>
#include <windows.h>
#include <cstdlib>
#include <ctime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 禁止最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    // 设置拖动窗口大小，这样的操作会使页面无法最大化
    setFixedSize(1200, 1000);

    //将组件替换为数组以bian一会操作
    orign_line[0] = ui->origin_lineEdit_0;
    orign_line[1] = ui->origin_lineEdit_1;
    orign_line[2] = ui->origin_lineEdit_2;
    orign_line[3] = ui->origin_lineEdit_3;
    orign_line[4] = ui->origin_lineEdit_4;
    orign_line[5] = ui->origin_lineEdit_5;
    orign_line[6] = ui->origin_lineEdit_6;
    orign_line[7] = ui->origin_lineEdit_7;
    orign_line[8] = ui->origin_lineEdit_8;
    //
    end_line[0] = ui->end_lineEdit_0;
    end_line[1] = ui->end_lineEdit_1;
    end_line[2] = ui->end_lineEdit_2;
    end_line[3] = ui->end_lineEdit_3;
    end_line[4] = ui->end_lineEdit_4;
    end_line[5] = ui->end_lineEdit_5;
    end_line[6] = ui->end_lineEdit_6;
    end_line[7] = ui->end_lineEdit_7;
    end_line[8] = ui->end_lineEdit_8;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//判断输入是否合法
bool MainWindow::judegInput(QLineEdit *a[9])
{
    //记录是否有空格
    bool isBlank = false;
    QSet<QString> num;
    //bool num[9] = {false};
    for (int i = 0; i < 9; i++)
    {
        if (a[i]->text() == "0")
        {
            isBlank = true;
        }
        num.insert(a[i]->text());
    }
    if (!isBlank)
    {
        QMessageBox::warning(this, tr("警告"), tr("没有空格输入"));
        return false;
    }
}

//设置默认输入
void MainWindow::setLineEdit(QString str, QLineEdit *a[9])
{
    for (int i = 0; i < 9; i++)
    {
        //截取一个字符
        a[i]->setText(str.mid(i, 1));
    }
}

//获取输入的字符串
QString MainWindow::getString(QLineEdit *a[9])
{
    QString str = "";
    for (int i = 0; i < 9; i++)
    {
        str += a[i]->text();
    }
    qDebug() << str << endl;
    return str;
}

//输出单步路径
void MainWindow::ouputPath(QString signlePath)
{
    //清除起始路径
    clearLineEdit(orign_line);
    //重新设置起始路径
    setLineEdit(signlePath, orign_line);
    ui->path_textBrowser->insertPlainText(signlePath + "\n");
}

//开始完全输出八数码
void MainWindow::on_strart_pushButton_clicked()
{
    ui->path_textBrowser->clear();
    for (int i = 0; i < game.path.size(); i++)
    {
        ouputPath(QString::fromStdString(game.path[i].str));
        //Sleep(1000);
    }
    ui->path_textBrowser->insertPlainText("共" + QString::number(game.pathLength) + "步");
}

string randomStr()
{
    string s = "";
    while (s.size() < 9)
    {
        int c = rand() % 10;
        if (c != 9)
        {
            //cout<<c<<endl;
            char a = c + '0';
            bool f = false;
            for (int i = 0; i < s.size(); i++)
            {
                if (s[i] == a)
                {
                    f = true;
                    break;
                }
            }
            if (f == false)
            {
                s += a;
            }
        }
    }
    return s;
}

//随机生成八数码
void MainWindow::on_random_pushButton_clicked()
{
    srand((int)time(0));
    string s1 = randomStr();
    string s2 = randomStr();

    QString str11 = QString::fromStdString(s1);
    QString str22 = QString::fromStdString(s2);
    //先清除输入
    clearLineEdit(orign_line);
    clearLineEdit(end_line);
    str1 = str11;
    str2 = str22;

    setLineEdit(str1, orign_line);
    setLineEdit(str2, end_line);
}

//清除输入的值
void MainWindow::clearLineEdit(QLineEdit *a[9])
{
    //先清除输入
    for (int i = 0; i < 9; i++)
    {
        a[i]->clear();
    }
}

//清空输入
void MainWindow::on_clear_pushButton_clicked()
{

    clearLineEdit(orign_line);
    clearLineEdit(end_line);
    ui->path_textBrowser->clear();
    ui->open_textBrowser->clear();
    ui->close_textBrowser->clear();
}

//单步执行
void MainWindow::on_sigleStep_pushButton_clicked()
{

    ouputPath(QString::fromStdString(game.path[0].str));
    if (game.path.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("已到达"));
        ui->path_textBrowser->insertPlainText("共" + QString::number(game.pathLength) + "步");
        return;
    }
    //删除路径中的首元素
    reverse(game.path.begin(), game.path.end());
    game.path.pop_back();
    reverse(game.path.begin(), game.path.end());
}

//生成路径
void MainWindow::on_creatPath_pushButton_clicked()
{
    if (!judegInput(orign_line) || !judegInput(end_line))
    {
        return;
    }
    str1 = getString(orign_line);
    str2 = getString(end_line);
    //给状态f赋值
    game = Game(str1.toStdString(), str2.toStdString());
    if (!game.isOdevity())
    {
        QMessageBox::warning(this, tr("警告"), tr("不可达请重新输入"));
        on_clear_pushButton_clicked();
        return;
    }
    qDebug() << "open表的尺寸" << game.open.size() << endl;
    //开始计算八数码
    game.start();
    QMessageBox::warning(NULL, "警告", "路径已经生成,共" + QString::number(game.pathLength) + "步");
    //输出open与close表
    for (int i = 0; i < game.openTable.size(); i++)
    {
        ui->open_textBrowser->insertPlainText(game.openTable[i] + "\n\n");
    }
    for (int i = 0; i < game.closeTable.size(); i++)
    {
        ui->close_textBrowser->insertPlainText(game.closeTable[i] + "\n\n");
    }
}
