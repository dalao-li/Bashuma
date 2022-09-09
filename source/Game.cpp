#include "include/Game.h"

Game::Game(std::string m_start_str, std::string m_end_str)
{
    this->m_start_str = std::move(m_start_str);
    this->m_end_str = std::move(m_end_str);
}

// 判断两个字符的奇偶性
bool Game::is_odevity(std::string m_start_str, std::string m_end_str)
{
    int oss = 0;
    int fss = 0;
    for (int i = 1; i < 9; ++i)
    {
        int a = 0, b = 0;
        for (int j = 0; j < i; j++)
        {
            if (m_start_str[j] > m_start_str[i] && m_start_str[i] != '0')
            {
                a++;
            }
            if (m_end_str[j] > m_end_str[i] && m_end_str[i] != '0')
            {
                b++;
            }
        }
        oss += a;
        fss += b;
    }
    return oss % 2 == fss % 2;
}

// 更新状态
void Game::update_state(State state)
{
    std::stringstream stream;

    // 空白位置下标
    int black_index = state.get_now_node().find('0');
    for (int i = 0; i < 4; i++)
    {
        // 该方向可达
        if (this->m_position[black_index][i] != -1)
        {
            std::string next_str = state.get_now_node();
            // 交换空白位置与它可达位置的元素
            std::swap(next_str[m_position[black_index][i]], next_str[black_index]);
            // 若交换后的字符串未在close表中
            if (get_node_index(next_str, this->m_close) == -1)
            {
                int n = get_node_index(next_str, this->m_open);
                // 若该字符串也未在open表中
                if (n == -1)
                {
                    // 写入QT控件
                    stream << (m_open_table.size() + 1) << ":" << next_str << "不在open表中,加入";

                    this->m_open_table.push_back(QString::fromStdString(stream.str()));

                    stream.str("");
                    stream.clear();

                    this->m_open.emplace_back(next_str, state.get_now_node(), state.get_g() + 1, this->set_w_value(next_str));
                }

                // 否则,若经过当前状态可以使路径更优
                else if (state.get_g() + 1 < m_open[n].get_g())
                {
                    // 将当前状态的节点设为交换后状态的父节点,并更新g值
                    this->m_open[n].update(state.get_now_node(), state.get_g() + 1);

                    stream << (m_open_table.size() + 1) << ":" << next_str << "在open表中,g值更新为" << (state.get_g() + 1);
                    this->m_open_table.push_back(QString::fromStdString(stream.str()));
                    stream.str("");
                    stream.clear();
                }
            }

            if (next_str == this->m_end_str)
            {
                this->m_flag = true;
                return;
            }
        }
    }

    // 将当前状态移出open表
    this->m_open.erase(find(m_open.begin(), m_open.end(), state));
    this->m_close.push_back(state);

    // 写入QT控件
    stream << (m_open_table.size() + 1) << ":" << state.get_now_node() << "被移除open表";
    this->m_open_table.push_back(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();

    stream << (m_close_table.size() + 1) << ":" << state.get_now_node() << "被加入close表";
    this->m_close_table.push_back(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();
}

// 给定一个字符串返回它在容器的下标
int Game::get_node_index(const std::string &s, const std::vector<State> &v)
{
    for (int i = 0, size = v.size(); i < size; ++i)
    {
        if (v[i].get_now_node() == s)
        {
            return i;
        }
    }
    return -1;
}

// 生成h值
int Game::set_w_value(std::string node)
{
    // 即求出当前字符串str中的每个元素str[i]在str中的下标i与str[i]在strend中的下标之差
    int sum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (i != int(node.find('0')))
        {
            sum += abs(i - int(this->m_end_str.find(node[i])));
        }
    }
    return sum;
}

// 生成整个转移过程的路径
void Game::find_path()
{
    if (!is_odevity(this->m_start_str, this->m_end_str))
    {
        return;
    }
    // 加入初始状态
    this->m_open.emplace_back(this->m_start_str, " ", 0, 0);

    update_state(this->m_open[0]);

    this->m_flag = false;

    while (!this->m_open.empty())
    {
        if (this->m_flag)
        {
            break;
        }
        // 按f值排序
        std::sort(this->m_open.begin(), this->m_open.end());
        update_state(this->m_open[this->m_open.size() - 1]);
    }

    // 生成路径
    std::vector<State> v;
    v.insert(v.end(), this->m_open.begin(), this->m_open.end());
    v.insert(v.end(), this->m_close.begin(), this->m_close.end());

    // 结束节点状态的下标
    int t = get_node_index(this->m_end_str, v);

    while (v[t].get_father_node() != " ")
    {
        this->m_path.push_back(v[t].get_now_node());
        // 找寻下一个节点
        t = get_node_index(v[t].get_father_node(), v);
    }

    // 加入起始节点
    this->m_path.push_back(this->m_start_str);
    reverse(this->m_path.begin(), this->m_path.end());
}
