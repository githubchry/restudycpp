
/*
[stack 官方中文文档](https://zh.cppreference.com/w/cpp/container/stack)

std::stack 类是容器适配器，是 LIFO （后进先出）数据结构。
stack 栈从被称作`栈顶`的容器尾部推(push)弹(pop)元素。
stack 缺省用 deque 来实现。但它的概念和 vector 更相似。它的接口跟 vector比，有如下改变：
    不能按下标访问元素
    没有 begin、end 成员函数
    back 成了 top，没有 front
    用 emplace 替代了 emplace_back，用 push 替代了 push_back，用 pop 替代了pop_back；
    没有其他的 push_…、pop_…、emplace…、insert、erase 函数

一般图形表示法会把 stack 表示成一个竖起的 vector：

                            ┌────────────────┐
                 ┌─────┐    │所有操作都从顶部完成│
                 │ top │    │    push/pop    │
                 └──┬──┘    └────────┬───────┘
                    │                │
                    └─────→──┬──←────┘
                             │
                          ┌──↓──┐
                          │  3  │
                          ├─────┤
                          │  2  │
                          ├─────┤
               ↖╮         │  1  │
┌───┬───┬───┬───┐         ├─────┤
│ 0 │ 1 │ 2 │ 3 │   =>    │  0  │
└───┴───┴───┴───┘         └─────┘
      vector               stack


这里有一个小细节需要注意。stack 跟内存管理时的栈有一个区别：
    这里的stack下面是低地址，向上则地址增大；而我们讨论内存管理时，高地址在下面，向上则地址减小，方向正好相反。
在使用 stack 时，这个区别通常无关紧要。
*/

#include <iostream>
#include <stack>

using namespace std;

int main() {
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    cout << "size:" << s.size() << endl;

    while (!s.empty()) {
        cout << s.top() << endl;
        s.pop();
    }

    cout << "size:" << s.size() << endl;
}