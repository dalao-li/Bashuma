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

using namespace std;
extern bool flag;

State::State(string str, string fatherStr, int g, int h)
{
    this->str = std::move(str);
    this->fatherStr = std::move(fatherStr);
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

void State::setInitValue(const string &fatherStrs, int gs)
{
    this->fatherStr = fatherStrs;
    this->f = gs + this->h;
    this->g = gs;
}

Game::Game(string originStr, string finalStr) : originStr(std::move(originStr)),
                                                finalStr(std::move(finalStr)) {}

//判断两个字符的奇偶性
bool Game::isOdevity()
{
    int os = 0, fs = 0;
    for (int i = 1; i < 9; ++i)
    {
        int a = 0, b = 0;
        for (int j = 0; j < i; j++)
        {
            if (originStr[j] > originStr[i] && originStr[i] != '0')
            {
                a++;
            }
            if (finalStr[j] > finalStr[i] && finalStr[i] != '0')
            {
                b++;
            }
        }
        os += a;
        fs += b;
    }
    return os % 2 == fs % 2;
}

//更新状态
void Game::updateState(State St)
{
    //空白位置下标
    int b = St.str.find('0');
    for (int i = 0; i < 4; i++)
    {
        //该方向可达
        if (moves[b][i] != -1)
        {
            string newStr = St.str;
            //交换空白位置与它可达位置的元素
            swap(newStr[moves[b][i]], newStr[b]);
            //若交换后的字符串未在close表中
            if (getStrIndex(newStr, close) == -1)
            {
                int n = getStrIndex(newStr, open);
                //若该字符串也未在open表中
                if (n == -1)
                {
                    //写入QT控件
                    openTable.push_back(QString::number(openTable.size() + 1) + ":" + QString::fromStdString(newStr) + "不在open表中，加入");
                    open.emplace_back(newStr, St.str, St.g + 1, disWeight(newStr));
                }
                //否则，若经过当前状态可以使路径更优
                else if (St.g + 1 < open[n].g)
                {
                    //将当前状态的节点设为交换后状态的父节点，并更新g值
                    open[n].setInitValue(St.str, St.g + 1);
                    //写入QT控件
                    openTable.push_back(QString::number(openTable.size() + 1) + ":" + QString::fromStdString(newStr + "在open表中，g值更新为") + QString::number(St.g + 1));
                }
            }
            if (newStr == finalStr)
            {
                flag = true;
                printf("ok\n");
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
int Game::getStrIndex(const string &str, const vector<State> &v)
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
int Game::disWeight(string str)
{
    //即求出当前字符串str中的每个元素str[i]在str中的下标i与str[i]在strend中的下标之差
    int sum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (i != str.find('0'))
        {
            sum += abs(i - int(finalStr.find(str[i])));
        }
    }
    return sum;
}

//生成整个转移过程的路径
void Game::creatProcessPath()
{
    vector<State> v;
    v.insert(v.end(), open.begin(), open.end());
    v.insert(v.end(), close.begin(), close.end());
    State St = v[getStrIndex(finalStr, v)];
    while (St.fatherStr != " ")
    {
        path.push_back(St);
        //找寻下一个节点
        St = v[getStrIndex(St.fatherStr, v)];
    }
    path.emplace_back(originStr, " ", 0, 0);
    reverse(path.begin(), path.end());
}

void Game::start()
{
    if (!isOdevity())
    {
        cout << "不可达" << endl;
        return;
    }
    //初始状态
    State St = State(originStr, " ", 0, 0);
    open.push_back(St);
    updateState(St);
    //当open表不为空时
    while (!open.empty())
    {
        if (flag)
        {
            break;
        }
        //按f值排序
        sort(open.begin(), open.end());
        updateState(open[open.size() - 1]);
    }
    creatProcessPath();
    pathLength = path.size();
}
