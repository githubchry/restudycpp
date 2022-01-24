
/*
学习了解`函数对象及其特化`概念(08func_obj.cc)之后，开始学习 priority_queue。

[priority_queue 官方中文文档](https://zh.cppreference.com/w/cpp/container/priority_queue)

priority_queue 也是一个容器适配器，它用到了比较函数对象（默认是 less）。
它和 stack 相似，支持 push、pop、top 等有限的操作，但容器内的顺序既不是后进先出，也不是先进先出，而是（部分）排序的结果。
在使用缺省的 less 作为其 Compare 模板参数时，最大的数值会出现在容器的“顶部”。
如果需要最小的数值出现在容器顶部，则可以传递 greater 作为其 Compare 模板参数。
*/

#include <algorithm>    // std::sort
#include <functional>   // std::less/greater/hash
#include <iostream>     // std::cout/endl
#include <memory>       // std::pair
#include <queue>        // std::priority_queue
#include <vector>       // std::vector
#include "output_container.h"   // 打印 pair<int, int>

using namespace std;

template<typename T>
void print_queue(T &q) {
    std::cout << "{ ";
    while (!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << "}\n";
}

int main() {
    std::priority_queue<int> int_q;
    // std::priority_queue<int, std::vector<int>, std::less<int>> int_q; // 等价

    for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
        int_q.push(n);
    print_queue(int_q);

    // 创建一个复杂一些的 priority_queue  元素类型是键值对(pair)  最小的数值出现在容器顶部(greater)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pair_q;
    pair_q.push({2, 1});
    pair_q.push({2, 0});
    pair_q.push({0, 3});
    pair_q.push({9, 4});
    print_queue(pair_q);

    // 用 lambda 比较元素。  异或运算符^  偶数^1 => 偶数+1  奇数^1 => 奇数-1
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
    for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2})
        q3.push(n);

    print_queue(q3);
    cout << int(8 ^ 1) << endl; // => 9
    cout << int(9 ^ 1) << endl; // => 8
}