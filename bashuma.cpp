//
// Created by _casper on 2019/11/10.
//
#include <iostream>
#include <utility>
#include <vector>
#include "bashuma.h"
#include <cstdio>
#include <algorithm>
#include "mainwindow.h"
#include <QDebug>
using namespace std;

State::State(string str, string fs, int g, int h)
{
    this->str = std::move(str);
    this->fs = std::move(fs);
    this->f = g + h;
    this->g = g;
    this->h = h;
}

bool State::operator<(const State &s1) const
{
    return s1.f < f;
}

bool State::operator==(const State &s1) const
{
    return s1.str == str;
}

void State::upState(const string &fss, int gs)
{
    this->fs = fss;
    this->f = gs + this->h;
    this->g = gs;
}

Game::Game(string os, string es) : os(std::move(os)),
                                   es(std::move(es)) {}

//判断两个字符的奇偶性
bool Game::isOdevity(string os, string es)
{
    int oss = 0, fss = 0;
    for (int i = 1; i < 9; ++i)
    {
        int a = 0, b = 0;
        for (int j = 0; j < i; j++)
        {
            if (os[j] > os[i] && os[i] != '0')
            {
                a++;
            }
            if (es[j] > es[i] && es[i] != '0')
            {
                b++;
            }
        }
        oss += a;
        fss += b;
    }
    return oss % 2 == fss % 2;
}

//更新状态
void Game::findState(State St)
{
    //空白位置下标
    int b = St.str.find('0');

    for (int i = 0; i < 4; i++)
    {
        //该方向可达
        if (move[b][i] != -1)
        {
            string ns = St.str;
            //交换空白位置与它可达位置的元素
            swap(ns[move[b][i]], ns[b]);
            //若交换后的字符串未在close表中
            if (findStr(ns, close) == -1)
            {
                int n = findStr(ns, open);
                //若该字符串也未在open表中
                if (n == -1)
                {
                    //写入QT控件
                    openTable.push_back(QString::number(openTable.size() + 1) + ":" + QString::fromStdString(ns) + "不在open表中,加入");
                    open.emplace_back(ns, St.str, St.g + 1, setWeight(ns));
                }
                //否则,若经过当前状态可以使路径更优
                else if (St.g + 1 < open[n].g)
                {
                    //将当前状态的节点设为交换后状态的父节点,并更新g值
                    open[n].upState(St.str, St.g + 1);

                    //写入QT控件
                    openTable.push_back(QString::number(openTable.size() + 1) + ":" + QString::fromStdString(ns + "在open表中,g值更新为") + QString::number(St.g + 1));
                }
            }
            if (ns == es)
            {
                flag = true;
                return;
            }
        }
    }
    //将当前状态移出open表
    open.erase(find(open.begin(), open.end(), St));
    close.push_back(St);

    //写入QT控件
    openTable.push_back(QString::number(openTable.size() + 1) + ":" + QString::fromStdString(St.str + "被移除open表"));
    closeTable.push_back(QString::number(closeTable.size() + 1) + ":" + QString::fromStdString(St.str + "被加入close表"));
}

//给定一个字符串返回它在容器的下标
int Game::findStr(const string &str, const vector<State> &v)
{
    for (int i = 0, size = v.size(); i < size; ++i)
    {
        if (v[i].str == str)
        {
            return i;
        }
    }
    return -1;
}

//生成h值
int Game::setWeight(string str)
{
    //即求出当前字符串str中的每个元素str[i]在str中的下标i与str[i]在strend中的下标之差
    int sum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (i != str.find('0'))
        {
            sum += abs(i - int(es.find(str[i])));
        }
    }
    return sum;
}

void Game::start()
{
    if (!isOdevity(os, es))
    {
        return;
    }
    //加入初始状态
    open.emplace_back(os, " ", 0, 0);
    findState(open[0]);
    flag = false;
    while (!open.empty())
    {
        if (flag)
        {
            break;
        }
        //按f值排序
        sort(open.begin(), open.end());
        findState(open[open.size() - 1]);
    }
    findPath();
}

//生成整个转移过程的路径
void Game::findPath()
{
    vector<State> v;
    v.insert(v.end(), open.begin(), open.end());
    v.insert(v.end(), close.begin(), close.end());
    //结束节点状态的下标
    int t = findStr(es, v);
    State St = v[t];
    while (St.fs != " ")
    {
        path.push_back(St.str);
        //找寻下一个节点
        St = v[findStr(St.fs, v)];
    }
    //加入起始节点
    path.emplace_back(os);
    reverse(path.begin(), path.end());
}
