
/*
[queue 官方中文文档](https://zh.cppreference.com/w/cpp/container/queue)

`类容器`与`容器`不一样，类容器特别点在于它们都不是完整的实现，而是依赖于某个现有的容器，因而被称为容器适配器（container adaptor）。

std::queue 类是容器适配器，是 FIFO （先进先出）数据结构。
类模板表现为底层容器的包装器——只提供特定的函数集合。 queue 在底层容器尾端推入(push)元素，从首端弹出(pop)元素。
queue 缺省用 deque 来实现。它的接口跟 deque 比，有如下改变：
    不能按下标访问元素
    没有 begin、end 成员函数
    用 emplace 替代了 emplace_back，用 push 替代了 push_back，用 pop 替代了pop_front
    没有其他的 push_…、pop_…、emplace…、insert、erase 函数

它的实际内存布局当然是随底层的容器而定的。从概念上讲，它的结构可如下所示：

  ┌────────┐                                      ┌────────────────┐
  │只能从前面│ ┌───────┐               ┌───────┐    │    只能从前面    │
  │  pop   │ │ front │               │ back  │    │  push/emplace  │
  └───┬────┘ └───┬───┘               └───┬───┘    └────────┬───────┘
      └───→──┬──←┘                       │                 │
           ┌─↓─┬───┬───┬───┬───┬───┬───┬─↓─┐               ↓
           │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │
           └───┴───┴───┴───┴───┴───┴───┴───┘

鉴于 queue 不提供 begin 和 end 方法，无法无损遍历（只能取出再读不能只读），我们只能用下面的代码约略展示一下其接口：
*/

#include <iostream>
#include <queue>

using namespace std;

int main() {
    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    cout << "size:" << q.size() << endl;

    while (!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }

    cout << "size:" << q.size() << endl;
}