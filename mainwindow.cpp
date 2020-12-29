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
    // 设置拖动窗口大小,这样的操作会使页面无法最大化;x,y 宽,长
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
    //禁用计算路径,自动执行,单步执行,清空按钮
    ui->findPathBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);
    //禁用上一步下一步按钮
    ui->afterPathBtn->setDisabled(true);
    ui->nextPathBtn->setDisabled(true);
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);
    ui->pathLabel->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//判断用户输入的合法性
bool MainWindow::judgeInput(QString s)
{
    if (s.size() < 9)
    {
        return false;
    }
    int a[10] = {-1};
    string v = s.toStdString();
    for (int i = 0; i < 9; i++)
    {
        //若包含非数字
        if (v[i] < 48 || v[i] > 57)
        {
            return false;
        }
        a[v[i] - '0'] = 1;
    }
    //判断0-9数字是否都出现了
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

//将字符串赋与LineEdit
void MainWindow::setLine(QString s, QLineEdit *a[9])
{
    for (int i = 0; i < 9; i++)
    {
        //截取一个字符
        if (s[i] != '0')
        {
            a[i]->setText(s.mid(i, 1));
        }
        //若是0则写入空
        else
        {
            a[i]->setText(" ");
        }
    }
}

//随机产生状态字符串
string MainWindow::getRandomStr()
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
    //等待时间流逝秒钟
    QTime time;
    time.start();
    while (time.elapsed() < times)
    {
        //处理事件
        QCoreApplication::processEvents();
    }
}

//将LineEdit中的值转换为字符串
QString MainWindow::getLineValue(QLineEdit *a[9])
{
    QString s = "";
    for (int i = 0; i < 9; i++)
    {
        if (a[i]->text() == " ")
        {
            s += '0';
        }
        else
        {
            s += a[i]->text();
        }
    }
    return s;
}

//显示单步路径,参数为当前的步数和当前状态
void MainWindow::ouputPath(int num)
{
    //num范围
    if (num < 0)
    {
        pathNum = game.path.size();
        //QMessageBox::warning(NULL, "警告", "已到达初始状态");
        return;
    }
    //到达终点
    if (num >= game.path.size())
    {
        pathNum = 0;
        //恢复自动执行按钮
        ui->autoOuputBtn->setDisabled(false);
        return;
    }
    //清除当前路径
    clearLine(originLine);
    QString nowPath = QString::fromStdString(game.path[num]);
    //重新设置当前路径
    setLine(nowPath, originLine);
    //pathTextBrowser输出
    QString s = QString("第%0步,共%1步").arg(QString::number(num + 1)).arg(QString::number(game.path.size()));
    ui->pathTextBrowser->insertPlainText(s + ": " + nowPath + "\n");
    //更新pathLable
    ui->pathLabel->setText(s);
}

//自动输出八数码求解路径
void MainWindow::on_autoOuputBtn_clicked()
{
    //异常判断
    if (game.path.size() == 0)
    {
        QMessageBox::warning(NULL, "警告", "请先生成路径");
        return;
    }
    ui->pathTextBrowser->clear();
    //禁用清空按钮,生成状态和计算路径,上一步下一步按钮
    ui->clearBtn->setDisabled(true);
    ui->findPathBtn->setDisabled(true);
    ui->autoInputBtn->setDisabled(true);
    ui->afterPathBtn->setDisabled(true);
    ui->nextPathBtn->setDisabled(true);
    for (int i = 0; i < game.path.size(); i++)
    {
        ouputPath(i);
        //延时
        int time = 10 * (100 - ui->horizontalSlider->value());
        wait(time);
    }
    QMessageBox::warning(NULL, "警告", "已到达,共" + QString::number(game.path.size()) + "步");
    //恢复清空按钮
    ui->clearBtn->setDisabled(false);
    ui->afterPathBtn->setDisabled(false);
    ui->nextPathBtn->setDisabled(false);
}

//自动随机生成两个状态
void MainWindow::on_autoInputBtn_clicked()
{
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);
    //禁用其他按钮
    ui->manuInputBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    //解禁计算路径按钮
    ui->findPathBtn->setDisabled(false);
    srand((int)time(0));
    string s1 = "";
    string s2 = "";
    while (1)
    {
        s1 = getRandomStr();
        s2 = getRandomStr();
        if (game.isOdevity(s1, s2))
        {
            break;
        }
    }
    originInput = QString::fromStdString(s1);
    endInput = QString::fromStdString(s2);
    //先清除输入
    clearLine(originLine);
    clearLine(endLine);
    //设置
    setLine(originInput, originLine);
    setLine(endInput, endLine);
}

//手动输入状态
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
    originInput = getLineValue(originLine);
    endInput = getLineValue(endLine);
    //判断合法性
    if (!judgeInput(originInput) || !judgeInput(endInput))
    {
        QMessageBox::warning(NULL, "警告", "请检查输入的合法性");
        return;
    }
    //如果不可达
    if (!game.isOdevity(originInput.toStdString(), endInput.toStdString()))
    {
        QMessageBox::warning(NULL, "警告", "不可达请重新输入");
        on_clearBtn_clicked();
        return;
    }
    //禁用生成状态,计算路径按钮
    ui->findPathBtn->setDisabled(true);
    ui->autoInputBtn->setDisabled(true);
    ui->manuInputBtn->setDisabled(true);
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);
    //给状态f赋值
    game.os = originInput.toStdString();
    game.es = endInput.toStdString();
    //开始计算八数码
    game.findPath();
    QMessageBox::warning(NULL, "警告", "路径已经生成,共" + QString::number(game.path.size()) + "步");
    //解禁自动执行,清空按钮,下一步按钮
    ui->autoOuputBtn->setDisabled(false);
    ui->clearBtn->setDisabled(false);
    ui->nextPathBtn->setDisabled(false);
    //解禁调速按钮
    ui->horizontalSlider->setDisabled(false);
    //输出open与close表
    for (int i = 0, size = game.openTable.size(); i < size; i++)
    {
        ui->open_textBrowser->insertPlainText(game.openTable[i] + "\n\n");
    }
    for (int i = 0, size = game.closeTable.size(); i < size; i++)
    {
        ui->close_textBrowser->insertPlainText(game.closeTable[i] + "\n\n");
    }
}

//清除输入的值
void MainWindow::clearLine(QLineEdit *a[9])
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
    //禁用自动执行,上一步下一步
    ui->findPathBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    ui->afterPathBtn->setDisabled(true);
    ui->nextPathBtn->setDisabled(true);
    //禁用调速按钮
    ui->horizontalSlider->setDisabled(true);
    //恢复生成状态按钮
    ui->autoInputBtn->setDisabled(false);
    ui->manuInputBtn->setDisabled(false);
    //恢复默认速度
    ui->horizontalSlider->setValue(50);

    //清除初始与结束状态
    clearLine(originLine);
    clearLine(endLine);
    //默认禁用所有LineEdit控件
    setLineStatus(originLine, true);
    setLineStatus(endLine, true);

    ui->pathTextBrowser->clear();
    ui->open_textBrowser->clear();
    ui->close_textBrowser->clear();
    //解除占用
    vector<State>().swap(game.open);
    vector<State>().swap(game.close);
    vector<string>().swap(game.path);
    //恢复pathLabel
    ui->pathLabel->setText("");
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

//上一步
void MainWindow::on_afterPathBtn_clicked()
{
    pathNum--;
    ouputPath(pathNum);
}

//下一步
void MainWindow::on_nextPathBtn_clicked()
{
    //解禁after按钮
    ui->afterPathBtn->setDisabled(false);
    if (pathNum == 0)
    {
        ouputPath(0);
    }
    pathNum++;
    ouputPath(pathNum);
}
