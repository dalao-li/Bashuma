#include <QSet>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bashuma.h"
#include <QDebug>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <QTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //窗口透明度
    setWindowOpacity(0.97);
    // 禁止最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    // 设置拖动窗口大小，这样的操作会使页面无法最大化;x,y 宽，长
    setFixedSize(960, 825);

    //将组件替换为数组以bian一会操作
    originLine[0] = ui->origin_lineEdit_0;
    originLine[1] = ui->origin_lineEdit_1;
    originLine[2] = ui->origin_lineEdit_2;
    originLine[3] = ui->origin_lineEdit_3;
    originLine[4] = ui->origin_lineEdit_4;
    originLine[5] = ui->origin_lineEdit_5;
    originLine[6] = ui->origin_lineEdit_6;
    originLine[7] = ui->origin_lineEdit_7;
    originLine[8] = ui->origin_lineEdit_8;
    endLine[0] = ui->end_lineEdit_0;
    endLine[1] = ui->end_lineEdit_1;
    endLine[2] = ui->end_lineEdit_2;
    endLine[3] = ui->end_lineEdit_3;
    endLine[4] = ui->end_lineEdit_4;
    endLine[5] = ui->end_lineEdit_5;
    endLine[6] = ui->end_lineEdit_6;
    endLine[7] = ui->end_lineEdit_7;
    endLine[8] = ui->end_lineEdit_8;

    //默认速度
    ui->label_7->setText(QString::number(ui->horizontalSlider->value()));
    //禁用计算路径,自动执行，单步执行，清空按钮
    ui->findPathBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    ui->manuOuputBtn->setDisabled(true);
    //ui->clearBtn->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//判断用户输入的合法性
bool MainWindow::judgeInputValue(QString s)
{
    if (s.size() < 9)
    {
        return false;
    }
    int a[10] = {-1};
    string v = s.toStdString();
    for (int i = 0; i < 9; i++)
    {
        if (v[i] < 48 || v[i] > 57)
        {
            return false;
        }
        a[v[i] - '0'] = 1;
    }
    for (int i = 0; i < 9; i++)
    {
        if (a[i] == -1)
        {
            return false;
        }
    }
    return true;
}

//设置LineEdit的状态
void MainWindow::setLineStatus(QLineEdit *a[9], bool flag)
{
    for (int i = 0; i < 9; i++)
    {
        a[i]->setReadOnly(flag);
        a[i]->setMaxLength(1);
    }
}

//设置九宫格输入
void MainWindow::setLineValue(QString str, QLineEdit *a[9])
{
    for (int i = 0; i < 9; i++)
    {
        //截取一个字符
        a[i]->setText(str.mid(i, 1));
    }
}

//随机产生状态
string MainWindow::randomStr()
{
    string s = "";
    while (s.size() < 9)
    {
        int c = rand() % 10;
        if (c != 9)
        {
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

//延时函数
void MainWindow::wait(int times)
{
    //等待时间流逝1秒钟
    QTime time;
    time.start();
    while (time.elapsed() < times)
    {
        //处理事件
        QCoreApplication::processEvents();
    }
}

//获取输入的字符串
QString MainWindow::getLinesValue(QLineEdit *a[9])
{
    QString s = "";
    for (int i = 0; i < 9; i++)
    {
        s += a[i]->text();
    }
    return s;
}

//显示单步路径，参数为当前的步数和当前状态
void MainWindow::displayPath(int num, QString nowPath)
{
    //清除当前路径
    clearLineValue(originLine);
    //重新设置当前路径
    setLineValue(nowPath, originLine);
    QString s = QString("第%0步,共%1步:%2\n").arg(QString::number(num)).arg(QString::number(game.pathLen)).arg(nowPath);
    ui->pathTextBrowser->insertPlainText(s);
}

//自动输出八数码求解路径
void MainWindow::on_autoOuputBtn_clicked()
{
    //异常判断
    if (game.pathLen == 0)
    {
        QMessageBox::warning(NULL, "警告", "请先生成路径");
        return;
    }
    ui->pathTextBrowser->clear();
    //禁用单步执行按钮
    ui->manuOuputBtn->setDisabled(true);
    //禁用清空按钮
    ui->clearBtn->setDisabled(true);
    //禁用生成状态和计算路径按钮
    ui->findPathBtn->setDisabled(true);
    ui->autoInputBtn->setDisabled(true);

    for (int i = 0; i < game.path.size(); i++)
    {
        displayPath(i + 1, QString::fromStdString(game.path[i]));
        //延时
        int time = 10 * (100 - ui->horizontalSlider->value());
        wait(time);
    }
    QMessageBox::warning(NULL, "警告", "已到达,共" + QString::number(game.pathLen) + "步");

    //恢复单步执行按钮
    ui->manuOuputBtn->setEnabled(true);
    //恢复清空按钮
    ui->clearBtn->setEnabled(true);
}

//单步执行输出八数码求解路径
void MainWindow::on_manuOuputBtn_clicked()
{
    //异常判断
    if (game.pathLen == 0)
    {
        QMessageBox::warning(NULL, "警告", "请先生成路径");
        return;
    }
    //禁用自动执行按钮
    ui->autoOuputBtn->setDisabled(true);

    int num = game.pathLen - game.path.size();
    displayPath(num, QString::fromStdString(game.path[0]));
    if (game.path.empty())
    {
        QMessageBox::warning(NULL, "警告", "已到达,共" + QString::number(game.pathLen) + "步");
        ui->pathTextBrowser->insertPlainText("共" + QString::number(game.pathLen) + "步");
        //解禁自动执行按钮
        //ui->autoOuputBtn->setDisabled(false);
        //禁用单步执行按钮
        ui->manuOuputBtn->setDisabled(true);
        return;
    }
    //删除路径中的首元素
    reverse(game.path.begin(), game.path.end());
    game.path.pop_back();
    reverse(game.path.begin(), game.path.end());
}

//自动随机生成两个状态
void MainWindow::on_autoInputBtn_clicked()
{
    //禁止lineEdit输入
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);
    //解禁计算路径按钮
    ui->findPathBtn->setDisabled(false);
    //禁用其他按钮
    ui->manuInputBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    ui->manuOuputBtn->setDisabled(true);

    srand((int)time(0));
    string s1 = "";
    string s2 = "";
    while (1)
    {
        s1 = randomStr();
        s2 = randomStr();
        if (game.isOdevity(s1, s2))
        {
            break;
        }
    }
    str1 = QString::fromStdString(s1);
    str2 = QString::fromStdString(s2);
    //先清除输入
    clearLineValue(originLine);
    clearLineValue(endLine);
    //设置
    setLineValue(str1, originLine);
    setLineValue(str2, endLine);
}

//手动输入
void MainWindow::on_manuInputBtn_clicked()
{
    QMessageBox::warning(NULL, "警告", "请手动输入初始与结束状态");
    //解禁LineEdit
    setLineStatus(originLine, false);
    setLineStatus(endLine, false);
    //解禁计算路径
    ui->findPathBtn->setDisabled(false);
    //禁用自动生成
    ui->autoInputBtn->setDisabled(true);
}

//计算路径
void MainWindow::on_findPathBtn_clicked()
{
    str1 = getLinesValue(originLine);
    str2 = getLinesValue(endLine);
    //判断合法性
    if (!judgeInputValue(str1) || !judgeInputValue(str2))
    {
        QMessageBox::warning(NULL, "警告", "请检查输入的合法性");
        return;
    }
    //如果不可达
    if (!game.isOdevity(str1.toStdString(), str2.toStdString()))
    {
        QMessageBox::warning(NULL, "警告", "不可达请重新输入");
        on_clearBtn_clicked();
        return;
    }
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);

    //给状态f赋值
    game.os = str1.toStdString();
    game.es = str2.toStdString();

    //禁用生成状态，计算路径按钮
    ui->findPathBtn->setDisabled(true);
    ui->autoInputBtn->setDisabled(true);
    ui->manuInputBtn->setDisabled(true);

    //开始计算八数码
    game.start();

    QMessageBox::warning(NULL, "警告", "路径已经生成,共" + QString::number(game.pathLen) + "步");

    //输出open与close表
    for (int i = 0, size = game.openTable.size(); i < size; i++)
    {
        ui->open_textBrowser->insertPlainText(game.openTable[i] + "\n\n");
    }
    for (int i = 0, size = game.closeTable.size(); i < size; i++)
    {
        ui->close_textBrowser->insertPlainText(game.closeTable[i] + "\n\n");
    }
    //解禁自动执行，单步执行，清空按钮
    ui->autoOuputBtn->setDisabled(false);
    ui->manuOuputBtn->setDisabled(false);
    ui->clearBtn->setDisabled(false);
    //解禁调速按钮
    ui->horizontalSlider->setDisabled(false);
}

//清除输入的值
void MainWindow::clearLineValue(QLineEdit *a[9])
{
    //先清除输入
    for (int i = 0; i < 9; i++)
    {
        a[i]->clear();
    }
}

//清空输入
void MainWindow::on_clearBtn_clicked()
{
    //禁用自动执行，单步执行，清空
    ui->findPathBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    ui->manuOuputBtn->setDisabled(true);
    ui->manuOuputBtn->setDisabled(true);
    //禁用调速按钮
    ui->horizontalSlider->setDisabled(true);
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);

    //恢复生成状态按钮
    ui->autoInputBtn->setDisabled(false);
    ui->manuInputBtn->setDisabled(false);
    //恢复默认速度
    ui->horizontalSlider->setValue(50);

    //清楚初始与结束状态
    clearLineValue(originLine);
    clearLineValue(endLine);
    ui->pathTextBrowser->clear();
    ui->open_textBrowser->clear();
    ui->close_textBrowser->clear();

    //解除占用
    vector<State>().swap(game.open);
    vector<State>().swap(game.close);
    vector<string>().swap(game.path);
}

//显示速度
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_7->setText(QString::number(value));
}

//文本框滚动
void MainWindow::on_pathTextBrowser_sourceChanged(const QUrl &arg1)
{
    ui->pathTextBrowser->moveCursor(QTextCursor::End);
}
