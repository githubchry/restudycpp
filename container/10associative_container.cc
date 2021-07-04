
/*

 [set](https://zh.cppreference.com/w/cpp/container/set)
 [map](https://zh.cppreference.com/w/cpp/container/map)
 [multiset](https://zh.cppreference.com/w/cpp/container/multiset)
 [multimap](https://zh.cppreference.com/w/cpp/container/multimap)

关联容器
    set（集合）
    map（映射）
    multiset（多重集）
    multimap（多重映射）

跳出 C++ 的语境，map（映射）的更常见的名字是关联数组和字典，而在 JSON里直接被称为对象（object）。
在 C++ 外这些容器常常是无序的；在 C++ 里关联容器则被认为是有序的。

运行本程序可以看到，关联容器是一种有序的容器。名字带“multi”的允许键重复，不带的不允许键重复。
set 和 multiset 只能用来存放键，而 map 和 multimap 则存放一个个键值对。

与序列容器相比，关联容器没有前、后的概念及相关的成员函数，但同样提供 insert、emplace 等成员函数。
此外，关联容器都有 find、lower_bound、upper_bound 等查找函数，结果是一个迭代器：
    find(k) 可以找到任何一个等价于查找键 k 的元素（!(x < k || k < x)）
    lower_bound(k) 找到第一个不小于查找键 k 的元素（!(x < k)）
    upper_bound(k) 找到第一个大于查找键 k 的元素（k < x）

如果在声明关联容器时没有提供比较类型的参数，缺省使用 less 来进行排序。
如果键的类型提供了比较算符 < 的重载，我们不需要做任何额外的工作。
否则，我们就需要对键类型进行 less 的特化，或者提供一个其他的函数对象类型。

对于自定义类型，我推荐尽量使用标准的 less 实现，通过重载 <（及其他标准比较运算符）对该类型的对象进行排序。
存储在关联容器中的键一般应满足严格弱序关系（strict weak ordering），即：
    对于任何该类型的对象 x：!(x < x)（非自反）
    对于任何该类型的对象 x 和 y：如果 x < y，则 !(y < x)（非对称）
    对于任何该类型的对象 x、y 和 z：如果 x < y 并且 y < z，则 x < z（传递性）
    对于任何该类型的对象 x、y 和 z：如果 x 和 y 不可比（!(x < y) 并且 !(y < x)）并且 y 和 z 不可比，则 x 和 z 不可比（不可比的传递性）

大部分情况下，类型是可以满足这些条件的，不过：
    如果类型没有一般意义上的大小关系（如复数），我们一定要别扭地定义一个大小关系吗？
    通过比较来进行查找、插入和删除，复杂度为对数 O(log(n))，有没有达到更好的性能的方法？
        —— 请看下一章`无序关联容器`
*/

#include <functional>   // std::less/greater/hash
#include <string>       // std::string
#include <map>          // std::map
#include <set>          // std::set
#include <tuple>        // std::tie
#include <iostream>     // std::cout/endl
#include "output_container.h"   // 打印 pair<int, int>

using namespace std;

int main() {
    set<int> s{1, 1, 1, 2, 3, 4};
    cout << s << endl;

    multiset<int, greater<int>> ms{1, 1, 1, 2, 3, 4};
    cout << ms << endl;

    cout << endl << "**** map ****" << endl;
    map<string, int> mp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };
    mp.insert({"four", -4});    // 已存在，不会插入
    cout << mp << endl;

    cout << boolalpha << bool(mp.find("five") == mp.end()) << endl;
    mp["five"] = 5;
    cout << boolalpha << bool(mp.find("five") == mp.end()) << endl;
    cout << mp << endl;
    cout << mp.find("four")->second << endl;
    cout << mp.lower_bound("ffff")->second << endl;     // 第一个 不小于(大于等于)"ffff"的键 => "five"
    cout << mp.upper_bound("four")->second << endl;     // 第一个 大于"four"的键 => 排在"four"后面的是"one"
    cout << (--mp.upper_bound("four"))->second << endl; // "one"的迭代器--就是"four"


    cout << endl << "**** multimap ****" << endl;

    multimap<string, int> mmp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };
    mmp.insert({"four", -4});   // multimap支持一个key存在多个相同或不同的值
    mmp.insert({"four", -4});   // multimap支持一个key存在多个相同或不同的值
    cout << mmp << endl;

    cout << mmp.find("four")->second << endl;
    cout << mmp.lower_bound("ffff")->second << endl;        // 第一个 不小于(大于等于)"ffff"的键 => "four"
    cout << mmp.upper_bound("four")->second << endl;        // 第一个 大于"four"的键 => 排在"four"后面的是"one"
    cout << (--mmp.upper_bound("four"))->second << endl;    // "one"的迭代器--就是"four"(第二个"four")

    cout << endl << "**** multimap equal_range****" << endl;
    cout << mmp << endl;
    // 如果需要在 multimap 里精确查找满足某个键的区间的话，建议使用 equal_range，可以一次性取得上下界（半开半闭）。如下所示：
    multimap<string, int>::iterator lower, upper;
    // 分别指向输入序列mmp中所有键等于 key 的元素所组成的子序列的起始及末尾（即最后一个元素之后的位置）位置。
    std::tie(lower, upper) = mmp.equal_range("four");

    cout << (lower != upper) << endl;       // 检测区间非空 true说明已找到有效元素
    cout << (upper == mmp.end())  << endl;  // true说明找到多个元素

    // 遍历打印找到的元素
    for (auto it = lower; it != upper; ++it) {
        cout << it->second << endl;
    }
}