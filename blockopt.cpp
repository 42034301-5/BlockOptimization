#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <vector>
#include <utility>
#include <algorithm>

using std::cin, std::cout, std::endl;
using std::istream, std::ostream;

template<typename T, typename F>
bool contain(const T& ls, const F& x)
{
    for(auto&& i : ls)
        if(i.compare(x) == 0)
            return true;
    return false;
}

struct quad_exp
{
    std::string op, a1, a2, a3;
};

istream &operator>>(istream &in, quad_exp &E)
{
    in >> E.op >> E.a1 >> E.a2 >> E.a3;
    return in;
};
ostream &operator<<(ostream &out, quad_exp &E)
{
    out << E.op << "\t" << E.a1 << "\t" << E.a2 << "\t" << E.a3 << " \n";
    return out;
};

int quad_type(const quad_exp& E)
{
    if(E.a3 != "-")
        return 3;
    if(E.a2 != "-")
        return 2;
    return 1;
}

struct node
{
    node* left, * right;
    std::string op;
    std::list<std::string> sym_list;
    int val;
    bool is_constant;
    
    node() : 
    left{nullptr}, right{nullptr}, op{}, sym_list{}, val{0}, is_constant{false}
    {}

    bool operator==(const node& rhs)
    {
        return (op == rhs.op && left == rhs.left && right == rhs.right);
    }

    bool is_leaf()
    {
        return (left == nullptr && right == nullptr);
    }
};


std::map<int, node*> node_map;
std::list<std::string> active_var = {"A", "B"};

bool is_root(node* n)
{
    for(auto&& i : node_map)
        if(i.second->left == n || i.second->right == n)
            return false;
    return true;
}

bool contain_active(node* n)
{
    if(n->sym_list.empty())
        return false;
    for(auto&& i : n->sym_list)
        if(contain(active_var, i))
            return true;
    return false;
}

// node* ceate(std::string&& symbol)
node* create(const std::string& symbol)
{
    node* new_node = new node();

    if(std::isdigit(symbol[0]))
    {
        new_node->is_constant = true;
        new_node->val = std::stoi(symbol.c_str());
    }
    else
    {
        new_node->sym_list.push_front(symbol);
    }

    return new_node;
}


node* find(const std::string& symbol)
{
    for(auto&& i : node_map)
    {
        auto&& tmp = i.second;

        for(auto&& j : tmp->sym_list)
            if(j.compare(symbol) == 0)
                return tmp;
        
        if(tmp->is_constant && tmp->val == std::atoi(symbol.c_str()))
            return tmp;
    }

    return nullptr;
}


node* find(const std::string& op, node* n1, node* n2)
{
    for(auto&& i : node_map)
    {
        auto&& tmp = i.second;
        if(tmp->op == op && tmp->left == n1 && tmp->right == n2)
            return tmp;
    }

    return nullptr;
}

std::list<node*> find_all(const std::string& symbol)
{
    std::list<node*> result;
    for(auto&& i : node_map)
    {
        auto&& tmp = i.second;

        for(auto&& j : tmp->sym_list)
            if(j.compare(symbol) == 0)
                result.push_back(tmp);
    }

    return result;
}


//(op, a1, a2, a3)
void read_tuple3(const quad_exp& tpl)
{
    node* n1 = nullptr, * n2 = nullptr, * n3 = nullptr;
    bool n23_const = true;  //n2和n3是否全为常量
    int val2, val3;
    n2 = ::find(tpl.a2), n3 = ::find(tpl.a3); 

    if(n2 != nullptr)
        if(!n2->is_constant)
            n23_const = false;
        else
            val2 = n2->val;
    else
        if(!std::isdigit(tpl.a2[0]))
            n23_const = false;
        else
            val2 = std::atoi(tpl.a2.c_str());

    if(n3 != nullptr)
        if(!n3->is_constant)
            n23_const = false;
        else
            val3 = n3->val;
    else
        if(!std::isdigit(tpl.a3[0]))
            n23_const = false;
        else
            val3 = std::atoi(tpl.a3.c_str());

    //n2n3均为常量 则直接计算n1值 n1无子结点
    if(n23_const)
    {
        n1 = ::create(tpl.a1);
        auto&& op = tpl.op;
        if(op == "ADD")
            n1->val = val2 + val3;
        else if(op == "SUB")
            n1->val = val2 - val3;
        else if(op == "MUL")
            n1->val = val2 * val3;
        else if(op == "DIV")
            n1->val = val2 / val3;

        
        n1->is_constant = true;
        auto tmp = find(std::to_string(n1->val));
        if(tmp != nullptr)
        {
            if(!::contain(tmp->sym_list, tpl.a1))
            {
                tmp->sym_list.push_back(tpl.a1);
            }
            delete n1;
        }
        else
            node_map.insert({node_map.size(), n1});
    }
    //n2n3不全为常量，则n1的子结点是n2和n3
    else
    {
        n1 = find(tpl.op, n2, n3);
        if(n1 != nullptr)
        {
            if(!::contain(n1->sym_list, tpl.a1))
            {
                n1->sym_list.push_back(tpl.a1);
            }
            return;
        }

        //缺少n2则创建
        if(n2 == nullptr)
        {
            n2 = create(tpl.a2);
            node_map.insert({node_map.size(), n2});
        }
        
        //缺少n3则创建
        if(n3 == nullptr)
        {
            n3 = create(tpl.a3);
            node_map.insert({node_map.size(), n3});
        }


        //已经存在标记为a1的结点且该结点不是叶子，则移除该结点上的a1标志

        auto all_node_with_a1 = find_all(tpl.a1);
        if(!all_node_with_a1.empty())
            for(auto&& n : all_node_with_a1)
            {
                if(n->is_leaf())
                    continue;
                for(auto i = n->sym_list.begin(); i != n->sym_list.end();)
                        if(i->compare(tpl.a1) == 0)
                            i = n->sym_list.erase(i);
                        else
                            ++i;
            }
        n1 = ::create(tpl.a1);
        n1->op = tpl.op;
        n1->left = n2;
        n1->right = n3;
        node_map.insert({node_map.size(), n1});
    }

}

//(op, x, y, -)
void read_tuple2(const quad_exp& tpl)
{
    node* n1 = nullptr, * n2 = nullptr;

    n2 = ::find(tpl.a2);
    if(n2 == nullptr)
    {
        n2 = create(tpl.a2);
        node_map.insert({node_map.size(), n2});
    }

    auto all_node_with_a1 = find_all(tpl.a1);
        if(!all_node_with_a1.empty())
            for(auto&& n : all_node_with_a1)
            {
                if(n->is_leaf())
                    continue;
                for(auto i = n->sym_list.begin(); i != n->sym_list.end();)
                        if(i->compare(tpl.a1) == 0)
                            i = n->sym_list.erase(i);
                        else
                            ++i;
            }

    n2->sym_list.push_back(tpl.a1);
}


//(READ, X, -, -)
void read_tuple1(const quad_exp& tpl)
{
    node* n1 = ::find(tpl.a1);
    if(n1 != nullptr && !contain(n1->sym_list, tpl.a1))
        n1->sym_list.push_back(tpl.a1);
    else
    {
        n1 = ::create(tpl.a1);
        node_map.insert({node_map.size(), n1});
    }
}


std::string node_symbol(node* n)
{
    std::string res;
    if(n == nullptr)
        return res;
    
    if(n->is_constant)
        res = std::to_string(n->val);
    else
        res = n->sym_list.front();

    return res;
}

std::string print_DAG()
{
    std::ostringstream os;

    for(auto&& i : node_map)
    {
        auto&& n = i.second;
        os << "Node: n" << i.first << "\t\t";
        os << "Mark: ";
        if(n->is_constant)
            os << n->val << "\t\t";
        else
            os << n->op << (n->op.empty() ? "\t\t" : "\t");

        os << "Is leaf: " << (n->left == nullptr && n->right == nullptr ? "Y" : "N") << "\t\t";

        os << "Symbols: " ;
        for(auto&& j : n->sym_list)
            os << j << ' ';
        os << "\t\t";

        if(n->left != nullptr)
            os << "Left node: n" << [=]()->int{
                for(auto&& i : node_map)
                    if(i.second == n->left)
                        return i.first;
                return -1;
            }() << "\t\t";

        if(n->right != nullptr)
            os << "Right node: n" << [=]()->int{
                for(auto&& i : node_map)
                    if(i.second == n->right)
                        return i.first;
                return -1;
            }() << "\t\t";

        if(n->is_constant)
            os << "\tConst\t\t";
        os << endl;
        
    }

    return os.str();
}

quad_exp gen_code(node* n, node* L, node* R)
{
    quad_exp e;
    e.a1 = node_symbol(n);
    e.a2 = node_symbol(L);
    e.a3 = node_symbol(R);
    e.op = n->op;
    
    return e;
}

std::list<quad_exp> optimized_code()
{
    std::list<quad_exp> exps;
    std::map<node*, bool> visited;
    std::vector<node*> roots;
    for(auto&& i : node_map)
    {
        if(i.second->is_leaf())
            visited.insert({i.second, true});
        else
            visited.insert({i.second, false});
    }

    for(auto&& i : node_map)
        if(::is_root(i.second))
            roots.push_back(i.second);
        
    for(auto&& i : roots)
    {
        std::stack<node*> stk;
        stk.push(i);
        while(!stk.empty())
        {
            auto j = stk.top();
            stk.pop();
            if(j->is_leaf())
            continue;
            if(visited[j->left] && visited[j->right])
            {
                exps.push_back(gen_code(j, j->left, j->right));
                visited[j] = true;
                continue;
            }
            if(!visited[j->left])
            {
                stk.push(j);
                stk.push(j->left);
            }
            if(!visited[j->right])
            {
                if(visited[j->left])
                    stk.push(j);
                stk.push(j->right);
            }
        }
    }

    
    return exps;
}

void release()
{
    for(auto&& i : node_map)
        if(i.second != nullptr)
        {
            delete i.second;
            i.second = nullptr;
        }
    node_map.clear();
}

int main(int argc, char** argv)
{
    std::ifstream in("test_blockopt.txt");
    std::ofstream out_DAG("DAG.txt");
    std::ofstream out_tpl("out.txt");

    quad_exp e;
    while(in.peek() != EOF)
    {
        in >> e;
        if(quad_type(e) == 3)
            read_tuple3(e);
        else if(quad_type(e) == 2)
            read_tuple2(e);
        else
            read_tuple1(e);
    }

    std::string DAG = print_DAG();
    out_DAG << DAG;

    //删除所有不含活跃变量的根结点
    while(true)
    {
        bool futile_clear = true;
        for(auto i = node_map.begin(); i != node_map.end();)
        {
            if(is_root(i->second) && !contain_active(i->second))
            {
                delete i->second;
                i = node_map.erase(i);
                futile_clear = false;
            }
            else
                ++i;
        }
        if(futile_clear)
            break;
    }

    //从n0开始删除所有不活跃的附加标识符，并对无标识符的结点附加一个标识符
    size_t node_serial = 1;
    for(auto&& i : node_map)
    {
        auto&& syml = i.second->sym_list;
        if(!syml.empty())
        {
            for(auto j = syml.begin(); j != syml.end();)
            {
                if(!contain(active_var, *j) && (
                    (!i.second->is_leaf()) ||                                                    // 非叶子结点的不活跃标识符全部删除
                    (i.second->is_leaf() && i.second->is_constant) ||                            // 常数叶子结点的不活跃标识符全部删除
                    (i.second->is_leaf() && !i.second->is_constant && j != syml.begin())         // 非常数叶子结点的非第一个不活跃标识符全部删除
                ))
                {
                    j = syml.erase(j);
                }
                else
                    ++j;
            }
        }

        if(syml.empty() && !i.second->is_constant)
            syml.push_back("S" + std::to_string(node_serial++));
    }

    cout << print_DAG() << endl;
    for(auto&& e : optimized_code())
    {
        out_tpl << e << endl;
    }

    release();
    return 0;
}