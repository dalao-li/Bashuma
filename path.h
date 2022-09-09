#ifndef BASHUMA_BASHUMA_H
#define BASHUMA_BASHUMA_H

#include <iostream>
#include <vector>
#include <sstream>
#include <QMainWindow>
#include <QLineEdit>
#include <QDebug>
#include <utility>
#include <cstdio>
#include <algorithm>

// 状态类
typedef class State
{
public:
    State() = default;

    State(std::string now_node, std::string father_node, int g, int h);

    // 更新父节点与g值
    void update(const std::string &father_node, int g);

    bool operator<(const State &s) const;

    bool operator==(const State &s) const;

    // 当前节点
    std::string now_node;

    // 父节点
    std::string father_node;

    int f;

    int g;

    int h;

} State;

// 游戏类
typedef class Game
{
public:
    Game() {}

    Game(std::string start_str, std::string end_str);

    ~Game() {}

    bool is_odevity(std::string start_str, std::string end_str);

    // 寻找节点的坐标
    int get_node_index(const std::string &s, const std::vector<State> &v);

    // 更新当前状态
    void update_state(State St);

    // 设置h值
    int set_w_value(std::string str);

    // 生成路径
    void find_path();

    std::vector<State> open;

    std::vector<State> close;

    std::vector<std::string> path;

    std::vector<QString> open_table;

    std::vector<QString> close_table;

    State state;

    // 八数码最初状态,最终状态字符串
    std::string start_str;

    std::string end_str;

    // 标志量
    bool flag;

    // 可移动路径
    //  0,1,2
    //  3,4,5
    //  6,7,8
    // move[i][j] 代表i能移动到的四个位置(-1代表不可达)
    int move[9][4] = {
        {-1, -1, 1, 3},
        {-1, 0, 2, 4},
        {-1, -1, 1, 5},
        {-1, 0, 4, 6},
        {1, 3, 5, 7},
        {-1, 2, 4, 8},
        {-1, -1, 3, 7},
        {-1, 4, 6, 8},
        {-1, -1, 5, 7}};

} Game;

#endif // static BASHUMA_BASHstatic UMA_H
