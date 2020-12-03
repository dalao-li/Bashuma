//
// Created by _casper on 2019/11/10.
//
#ifndef BASHUMA_BASHUMA_H
#define BASHUMA_BASHUMA_H

#include <iostream>
#include <vector>
#include <QMainWindow>
#include <QLineEdit>
#include <QDebug>
using namespace std;

//状态类
typedef class State
{
public:
    //当前节点
    string str;
    //父节点
    string fatherStr;
    int f, g, h;
    State(string str, string fatherStr, int g, int h);

    //更新父节点与g值
    void setInitValue(const string &fatherStrs, int gs);

    bool operator<(const State &s1) const;

    bool operator==(const State &s1) const;

} State;

//游戏类
typedef class Game
{
public:
    //八数码最初状态
    string originStr;
    //八数码最终状态
    string finalStr;
    vector<State> open;
    vector<State> close;
    vector<State> path;

    vector<QString> openTable;
    vector<QString> closeTable;
    //路径长度
    int pathLength;
    //可移动路径
    // 0,1,2
    // 3,4,5
    // 6,7,8
    //moves[i][j] 代表i能移动到的四个位置(-1代表不可达)
    int moves[9][4] = {{-1, -1, 1, 3},
                       {-1, 0, 2, 4},
                       {-1, -1, 1, 5},
                       {-1, 0, 4, 6},
                       {1, 3, 5, 7},
                       {-1, 2, 4, 8},
                       {-1, -1, 3, 7},
                       {-1, 4, 6, 8},
                       {-1, -1, 5, 7}};
    Game() {}
    Game(string originStr, string finalStr);

    void clear()
    {
        qDebug() << "open表的原始尺寸" << open.size() << endl;
        qDebug() << "open表的原始内存" << open.capacity() << endl;
        qDebug() << "openTable表的原始内存" << openTable.capacity() << endl;
        qDebug() << "openTable表的原始内存" << openTable.capacity() << endl;
        vector<State>().swap(open);
        vector<State>().swap(close);
        vector<State>().swap(path);
        vector<QString>().swap(openTable);
        vector<QString>().swap(closeTable);
        qDebug() << "open表的当前尺寸" << open.size() << endl;
        qDebug() << "open表的当前内存" << open.capacity() << endl;
        qDebug() << "openTable表的原始内存" << openTable.capacity() << endl;
        qDebug() << "openTable表的原始内存" << openTable.capacity() << endl;
    }

    bool isOdevity();

    //更新当前状态
    void updateState(State St);

    //寻找节点的坐标
    static int getStrIndex(const string &str, const vector<State> &v);

    //设置h值
    int disWeight(string str);

    //生成路径
    void creatProcessPath();

    //主循环
    void start();

} Game;

#endif //static BASHUMA_BASHstatic UMA_H
