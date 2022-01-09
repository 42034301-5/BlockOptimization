#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <tuple>
#include <memory>
#include <regex>
#include <functional>
#include <assert.h>


// 四元式的定义
struct QuadExp
{
    std::string op, a1, a2, a3;

    QuadExp(const std::string& op = "", const std::string& a1 = "", const std::string& a2 = "", const std::string& a3 = "")
    {
        this->op = op;
        this->a1 = a1;
        this->a2 = a2;
        this->a3 = a3;
    }

    // 返回四元式的类型
    int type()  const
    {
        if (op == "SET")
            return 0;
        if (op == "TAR")
            return 3;

        // 1 型四元式暂未被使用
        if (a2 == "-" && a3 != "-")
            return 1;

        return 2;
    }

    std::string toString()
    {
        return (op + "\t" + a1 + "\t" + a2 + "\t" + a3 + "\t");
    }

    void clear()
    {
        op.clear();
        a1.clear();
        a2.clear();
        a3.clear();
    }
};

std::istream& operator>>(std::istream& in, QuadExp& E)
{
    in >> E.op >> E.a1 >> E.a2 >> E.a3;
    return in;
};
std::ostream& operator<<(std::ostream& out, const QuadExp& E)
{
    out << E.op << "\t" << E.a1 << "\t" << E.a2 << "\t" << E.a3 << " \n";
    return out;
};

// 判断 elem 是否是 ls 中的元素
template<typename T, typename F>
bool contain(const T& ls, const F& elem)
{
    for (auto&& e : ls)
    {
        if (e == elem)
            return true;
    }
    return false;
}

// 判断 arg 代表的字符串是否是常数
bool isLiteral(const std::string& arg)
{
    for (auto&& i : arg)
    {
        if (i > '9' || i < '0')
            return false;
    }
    return true;
}

// 求有序容器 A 与 B 的交集
template<typename T>
T intersection(const T& A, const T& B)
{
    T result;
    auto A1 = A;
    auto B1 = B;
    std::sort(A1.begin(), A1.end());
    std::sort(B1.begin(), B1.end());
    std::set_intersection(
        A1.begin(), A1.end(),
        B1.begin(), B1.end(),
        std::back_inserter(result)
    );

    return result;
}

// 求有序容器 A 与 B 的并集
template<typename T>
T setunion(const T& A, const T& B)
{
    T result;
    auto A1 = A;
    auto B1 = B;
    std::sort(A1.begin(), A1.end());
    std::sort(B1.begin(), B1.end());
    std::set_union(
        A1.begin(), A1.end(),
        B1.begin(), B1.end(),
        std::back_inserter(result)
    );

    return result;
}

// 求有序容器 A 与 B 的差集
template<typename T>
T difference(const T& A, const T& B)
{
    T result;
    auto A1 = A;
    auto B1 = B;
    std::sort(A1.begin(), A1.end());
    std::sort(B1.begin(), B1.end());
    std::set_difference(
        A1.begin(), A1.end(),
        B1.begin(), B1.end(),
        std::back_inserter(result)
    );

    return result;
}

// 判断序列 A 是否以序列 B 开头
template<typename T>
bool startWith(const T& A, const T& B)
{
    if (B.size() > A.size())
        return false;

    auto it1 = A.begin();
    auto it2 = B.begin();
    while (it2 != B.end())
    {
        if (*it1 != *it2)
            return false;
        ++it1;
        ++it2;
    }
    return true;
}

// 删除字符串 str 两端的所有字符 mark
std::string strip(const std::string& str, const char mark = ' ')
{
    auto s = str;
    while (!s.empty() && s.front() == mark)
    {
        s.erase(s.begin());
    }
    while (!s.empty() && s.back() == mark)
    {
        s.erase(s.end() - 1);
    }

    return s;
}

// 反转序列进行基于范围的逆序遍历
template<typename T>
struct reversion_wrapper
{
    T& iterable;
};
template<typename T>
auto begin(reversion_wrapper<T> w)
{
    return std::rbegin(w.iterable);
}
template<typename T>
auto end(reversion_wrapper<T> w)
{
    return std::rend(w.iterable);
}
template<typename T>
reversion_wrapper<T> reverse(T&& iterable)
{
    return { iterable };
}


#endif