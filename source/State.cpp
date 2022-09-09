/*
 * @Description:
 * @Version: 1.0
 * @Author: DaLao
 * @Email: dalao_li@163.com
 * @Date: 2022-09-09 22:58:24
 * @LastEditors: DaLao
 * @LastEditTime: 2022-09-09 23:31:30
 */

#include "State.h"

State::State(std::string m_now_node, std::string m_father_node, int m_g, int m_h)
{
    this->m_now_node = std::move(m_now_node);
    this->m_father_node = std::move(m_father_node);
    this->m_f = m_g + m_h;
    this->m_g = m_g;
    this->m_h = m_h;
}

const std::string State::get_now_node() const
{
    return this->m_now_node;
}

void State::set_now_node(std::string m_now_node)
{
    this->m_now_node = m_now_node;
}

const std::string State::get_father_node() const
{
    return this->m_father_node;
}

void State::set_father_node(std::string m_father_node)
{
    this->m_father_node = m_father_node;
}

const int State::get_f() const
{
    return this->m_f;
}

void State::set_f(int m_f)
{
    this->m_f = m_f;
}

const int State::get_g() const
{
    return this->m_g;
}

void State::set_g(int m_g)
{
    this->m_g = m_g;
}

const int State::get_h() const
{
    return this->m_h;
}

void State::set_h(int m_h)
{
    this->m_h = m_h;
}

bool State::operator<(const State &s) const
{
    return s.m_f < m_f;
}

bool State::operator==(const State &s) const
{
    return s.m_now_node == m_now_node;
}

// 更新父节点和g值
void State::update(const std::string &m_father_node, int m_g)
{
    this->m_father_node = m_father_node;
    this->m_f = m_g + this->m_h;
    this->m_g = m_g;
}