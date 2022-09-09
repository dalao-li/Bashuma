#include "path.h"

State::State(std::string now_node, std::string father_node, int g, int h)
{
    this->now_node = std::move(now_node);
    this->father_node = std::move(father_node);
    this->f = g + h;
    this->g = g;
    this->h = h;
}

bool State::operator<(const State &s) const
{
    return s.f < f;
}

bool State::operator==(const State &s) const
{
    return s.now_node == now_node;
}

// 更新父节点和g值
void State::update(const std::string &father_node, int g)
{
    this->father_node = father_node;
    this->f = g + this->h;
    this->g = g;
}

Game::Game(std::string start_str, std::string end_str)
{
    this->start_str = std::move(start_str);
    this->end_str = std::move(end_str);
}

// 判断两个字符的奇偶性
bool Game::is_odevity(std::string start_str, std::string end_str)
{
    int oss = 0, fss = 0;
    for (int i = 1; i < 9; ++i)
    {
        int a = 0, b = 0;
        for (int j = 0; j < i; j++)
        {
            if (start_str[j] > start_str[i] && start_str[i] != '0')
            {
                a++;
            }
            if (end_str[j] > end_str[i] && end_str[i] != '0')
            {
                b++;
            }
        }
        oss += a;
        fss += b;
    }
    return oss % 2 == fss % 2;
}

// 给定一个字符串返回它在容器的下标
int Game::get_node_index(const std::string &s, const std::vector<State> &v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (v[i].now_node == s)
        {
            return i;
        }
    }
    return -1;
}

// 更新状态
void Game::update_state(State state)
{
    std::stringstream stream;

    // 空白位置下标
    int black_index = state.now_node.find('0');
    for (int i = 0; i < 4; i++)
    {
        // 该方向可达
        if (move[black_index][i] != -1)
        {
            std::string next_str = state.now_node;
            // 交换空白位置与它可达位置的元素
            std::swap(next_str[move[black_index][i]], next_str[black_index]);
            // 若交换后的字符串未在close表中
            if (get_node_index(next_str, close) == -1)
            {
                int n = get_node_index(next_str, open);
                // 若该字符串也未在open表中
                if (n == -1)
                {
                    // 写入QT控件
                    stream << (open_table.size() + 1) << ":" << next_str << "不在open表中,加入";

                    open_table.push_back(QString::fromStdString(stream.str()));

                    stream.str("");
                    stream.clear();

                    open.emplace_back(next_str, state.now_node, state.g + 1, set_w_value(next_str));
                }

                // 否则,若经过当前状态可以使路径更优
                else if (state.g + 1 < open[n].g)
                {
                    // 将当前状态的节点设为交换后状态的父节点,并更新g值
                    open[n].update(state.now_node, state.g + 1);

                    stream << (open_table.size() + 1) << ":" << next_str << "在open表中,g值更新为" << (state.g + 1);
                    open_table.push_back(QString::fromStdString(stream.str()));
                    stream.str("");
                    stream.clear();
                }
            }

            if (next_str == end_str)
            {
                flag = true;
                return;
            }
        }
    }

    // 将当前状态移出open表
    open.erase(find(open.begin(), open.end(), state));
    close.push_back(state);

    // 写入QT控件
    stream << (open_table.size() + 1) << ":" << state.now_node << "被移除open表";
    open_table.push_back(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();

    stream << (close_table.size() + 1) << ":" << state.now_node << "被加入close表";
    close_table.push_back(QString::fromStdString(stream.str()));

    stream.str("");
    stream.clear();
}

// 生成h值
int Game::set_w_value(std::string now_node)
{
    // 即求出当前字符串str中的每个元素str[i]在str中的下标i与str[i]在strend中的下标之差
    int sum = 0;
    for (int i = 0; i < 9; i++)
    {
        if (i != int(now_node.find('0')))
        {
            sum += abs(i - int(end_str.find(now_node[i])));
        }
    }
    return sum;
}

// 生成整个转移过程的路径
void Game::find_path()
{
    if (!is_odevity(start_str, end_str))
    {
        return;
    }
    // 加入初始状态
    open.emplace_back(start_str, " ", 0, 0);

    update_state(open[0]);

    flag = false;

    while (!open.empty())
    {
        if (flag)
        {
            break;
        }
        // 按f值排序
        sort(open.begin(), open.end());
        update_state(open[open.size() - 1]);
    }

    // 生成路径
    std::vector<State> v;
    v.insert(v.end(), open.begin(), open.end());
    v.insert(v.end(), close.begin(), close.end());

    // 结束节点状态的下标
    int t = get_node_index(end_str, v);

    while (v[t].father_node != " ")
    {
        path.push_back(v[t].now_node);
        // 找寻下一个节点
        t = get_node_index(v[t].father_node, v);
    }

    // 加入起始节点
    path.push_back(start_str);
    reverse(path.begin(), path.end());
}
