
/*

[forward_list 官方中文文档](https://zh.cppreference.com/w/cpp/container/forward_list)

既然 list 是双向链表，那么 C++ 里有没有单向链表呢？答案是肯定的。
从 C++11 开始，前向列表 forward_list 成了标准的一部分。

它的内存布局一般是下图这个样子：

┌───────┐┌───────┐                          ┌───────┐
│ begin ││ front │                          │  end  │
└───┬───┘└───┬───┘                          └───┬───┘
    └─→──┬──←┘                                  │
       ┌─↓─┬┐   ┌───┬┐   ┌───┬┐   ┌───┬┐   ┌────↓────┬┐
       │ 0 │├──→┤ 1 │├──→┤ 2 │├──→┤ 3 │├──→┤ nullptr ││
       └───┴┘   └───┴┘   └───┴┘   └───┴┘   └─────────┴┘

大部分 C++ 容器都支持 insert 成员函数，语义是从指定的位置之前插入一个元素。对于forward_list，这不是一件容易做到的事情（想一想，为什么？）。
标准库提供了一个insert_after 作为替代。此外，它跟 list 相比还缺了下面这些成员函数：
    back
    size
    push_back
    emplace_back
    pop_back

在元素大小较小的情况下，forward_list 能节约的内存是非常可观的；在列表不长的情况下，不能反向查找也不是个大问题。
提高内存利用率，往往就能提高程序性能，更不用说在内存可能不足时的情况了。
*/

#include "output_container.h"
#include <iostream>
#include <forward_list>

using namespace std;

int main() {
    forward_list<int> fwlist{1, 7, 2, 8, 3};

    fwlist.sort();
    cout << fwlist << endl; // { 1, 2, 3, 7, 8 }

    fwlist.insert_after(fwlist.begin(), 100);   // 在第一个元素后面插入
    cout << fwlist << endl; // { 1, 100, 2, 3, 7, 8 }

    fwlist.emplace_front(0);
    // fwlist.insert_after(fwlist.before_begin(), 0);   // 等价
    cout << fwlist << endl; // { 0, 1, 100, 2, 3, 7, 8 }

    // fwlist.insert_after(fwlist.end(), 100); // 运行崩溃：访问空指针（最后一个元素是nullprt）

    // 想在插入元素到 fwlist 后面只能遍历再插入
    auto pre = fwlist.before_begin();
    auto cur = fwlist.begin();
    while (cur != fwlist.end()) {
        pre = cur;
        cur++;
    }
    fwlist.insert_after(pre, 999);
    cout << fwlist << endl; // { 0, 1, 100, 2, 3, 7, 8, 999 }

}