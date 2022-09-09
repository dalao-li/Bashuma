/*
 * @Description:
 * @Version: 1.0
 * @Author: DaLao
 * @Email: dalao_li@163.com
 * @Date: 2022-09-09 22:57:33
 * @LastEditors: DaLao
 * @LastEditTime: 2022-09-09 23:31:01
 */

#ifndef STATE_H
#define STATE_H

#include <iostream>

// 状态类
typedef class State
{
public:
    State() = default;

    State(std::string m_now_node, std::string m_father_node, int m_g, int m_h);

    const std::string get_now_node() const;

    const std::string get_father_node() const;

    const int get_f() const;

    const int get_g() const;

    const int get_h() const;

    void set_now_node(std::string m_now_node);

    void set_father_node(std::string m_father_node);

    void set_f(int m_f);

    void set_g(int m_g);

    void set_h(int m_h);

    // 更新父节点与g值
    void update(const std::string &m_father_node, int m_g);

    bool operator<(const State &s) const;

    bool operator==(const State &s) const;

protected:
    // 当前节点
    std::string m_now_node;

    // 父节点
    std::string m_father_node;

    int m_f;

    int m_g;

    int m_h;

} State;

#endif