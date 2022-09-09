/*
 * @Description:
 * @Version: 1.0
 * @Author: DaLao
 * @Email: dalao_li@163.com
 * @Date: 2022-09-09 23:00:11
 * @LastEditors: DaLao
 * @LastEditTime: 2022-09-09 23:35:28
 */
#ifndef GAME_H
#define GAME_H

#include "State.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <QMainWindow>
#include <QLineEdit>
#include <QDebug>
#include <utility>
#include <cstdio>
#include <algorithm>

// 游戏类
typedef class Game
{
public:
    Game() {}

    Game(std::string start_str, std::string end_str);

    ~Game() {}

    bool is_odevity(std::string start_str, std::string end_str);

    // 更新当前状态
    void update_state(State St);

    // 寻找节点的坐标
    int get_node_index(const std::string &s, const std::vector<State> &v);

    // 设置h值
    int set_w(std::string str);

    // 生成路径
    void find_path();

protected:
    std::vector<State> m_open;

    std::vector<State> m_close;

    std::vector<std::string> m_path;

    std::vector<QString> m_open_table;

    std::vector<QString> m_close_table;

    State m_state;

    // 八数码最初状态,最终状态字符串
    std::string m_start_str;

    std::string m_end_str;

    // 标志量
    bool m_flag;

    // 可移动路径
    //  0,1,2
    //  3,4,5
    //  6,7,8
    // move[i][j] 代表i能移动到的四个位置(-1代表不可达)
    int m_position[9][4] = {
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

#endif