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
    setWindowOpacity(0.99);
    // 禁止最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    // 设置拖动窗口大小，这样的操作会使页面无法最大化
    //x,y 宽，长
    setFixedSize(960, 825);

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

    //默认速度
    ui->label_7->setText(QString::number(ui->horizontalSlider->value()));
    //计算路径,自动执行，单步执行，清空按钮禁用
    ui->findPathBtn->setDisabled(true);
    ui->autoOuputBtn->setDisabled(true);
    ui->manuOuputBtn->setDisabled(true);
    ui->clearBtn->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//设置九宫格默认输入
void MainWindow::setLineEdit(QString str, QLineEdit *a[9])
{
    for (int i = 0; i < 9; i++)
    {
        //截取一个字符
        QString s = str.mid(i, 1);
        a[i]->setText(s);
    }
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

//延时函数
void wait(int times)
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
QString MainWindow::getString(QLineEdit *a[9])
{
    QString str = "";
    for (int i = 0; i < 9; i++)
    {
        str += a[i]->text();
    }
    //qDebug() << str << endl;
    return str;
}

//输出单步路径，参数为当前的步数和当前状态
void MainWindow::ouputPath(int num, QString nowPath)
{
    //清除起始路径
    clearLineEdit(orign_line);
    //重新设置起始路径
    setLineEdit(nowPath, orign_line);
    QString s = QString("第%0步,共%1步:%2\n").arg(QString::number(num)).arg(QString::number(game.pathLen)).arg(nowPath);
    ui->path_textBrowser->insertPlainText(s);
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

    ui->path_textBrowser->clear();
    //禁用单步执行按钮
    ui->manuOuputBtn->setDisabled(true);
    //禁用清空按钮
    ui->clearBtn->setDisabled(true);
    //禁用生成状态和计算路径按钮
    ui->findPathBtn->setDisabled(true);
    ui->autoInputBtn->setDisabled(true);

    for (int i = 0; i < game.path.size(); i++)
    {
        ouputPath(i + 1, QString::fromStdString(game.path[i]));
        //延时
        int time = 20 * (100 - ui->horizontalSlider->value());
        wait(time);
    }
    QMessageBox::warning(NULL, "警告", "已到达,共" + QString::number(game.pathLen) + "步");

    //恢复单步执行按钮
    ui->manuOuputBtn->setEnabled(true);
    //恢复清空按钮
    ui->clearBtn->setEnabled(true);
}

//单步执行
void MainWindow::on_manuOuputBtn_clicked()
{
    //异常判断
    if (game.pathLen == 0)
    {
        QMessageBox::warning(NULL, "警告", "请先生成路径");
        return;
    }
    int num = game.pathLen - game.path.size();
    ouputPath(num, QString::fromStdString(game.path[0]));
    if (game.path.empty())
    {
        QMessageBox::warning(this, tr("警告"), tr("已到达"));
        ui->path_textBrowser->insertPlainText("共" + QString::number(game.pathLen) + "步");
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
    //解禁计算路径按钮
    ui->findPathBtn->setDisabled(false);
    //禁用其他按钮
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
    clearLineEdit(orign_line);
    clearLineEdit(end_line);
    //设置
    setLineEdit(str1, orign_line);
    setLineEdit(str2, end_line);
}

//计算路径
void MainWindow::on_findPathBtn_clicked()
{
    //清空openTable和closeTable表
    str1 = getString(orign_line);
    str2 = getString(end_line);
    //给状态f赋值
    game.os = str1.toStdString();
    game.es = str2.toStdString();
    if (!game.isOdevity(str1.toStdString(), str2.toStdString()))
    {
        QMessageBox::warning(this, tr("警告"), tr("不可达请重新输入"));
        on_clearBtn_clicked();
        //禁用计算路径,自动执行，单步执行，清空按钮
        ui->findPathBtn->setDisabled(true);
        ui->autoOuputBtn->setDisabled(true);
        ui->manuOuputBtn->setDisabled(true);
        ui->clearBtn->setDisabled(true);
        return;
    }
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
void MainWindow::clearLineEdit(QLineEdit *a[9])
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
    ui->clearBtn->setDisabled(true);
    ui->manuOuputBtn->setDisabled(true);
    //禁用调速按钮
    ui->horizontalSlider->setDisabled(true);

    //恢复生成状态按钮
    ui->autoInputBtn->setDisabled(false);
    ui->manuInputBtn->setDisabled(false);
    //恢复默认速度
    ui->horizontalSlider->setValue(50);
    clearLineEdit(orign_line);
    clearLineEdit(end_line);
    ui->path_textBrowser->clear();
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
void MainWindow::on_path_textBrowser_sourceChanged(const QUrl &arg1)
{
    ui->path_textBrowser->moveCursor(QTextCursor::End);
}

//手动输入
void MainWindow::on_manuInputBtn_clicked()
{
}
