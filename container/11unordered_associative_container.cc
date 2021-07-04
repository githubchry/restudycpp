
/*


 [unordered_set](https://zh.cppreference.com/w/cpp/container/unordered_set)
 [unordered_map](https://zh.cppreference.com/w/cpp/container/unordered_map)
 [unordered_multiset](https://zh.cppreference.com/w/cpp/container/unordered_multiset)
 [unordered_multimap](https://zh.cppreference.com/w/cpp/container/unordered_multimap)

从 C++11 开始，每一个关联容器都有一个对应的无序关联容器，它们是：
    unordered_set（集合）
    unordered_map（映射）
    unordered_multiset（多重集）
    unordered_multimap（多重映射）

这些容器和关联容器非常相似，主要的区别就在于它们是“无序”的。
这些容器不要求提供一个排序的函数对象，而要求一个可以计算哈希值的函数对象。
你当然可以在声明容器对象时手动提供这样一个函数对象类型，但更常见的情况是，我们使用标准的 hash 函数对象及其特化。

从实际的工程角度，无序关联容器的主要优点在于其性能。
关联容器和 priority_queue 的插入和删除操作，以及关联容器的查找操作，其复杂度都是 O(log(n))，而无序关联容器的实现使用哈希表，可以达到平均 O(1)！
但这取决于我们是否使用了一个好的哈希函数：
    在哈希函数选择不当的情况下，无序关联容器的插入、删除、查找性能可能成为最差情况的 O(n)，那就比关联容器糟糕得多了。
*/

#include <complex>          // std::complex 复数
#include <unordered_map>    // std::unordered_map
#include <unordered_set>    // std::unordered_set

#include <functional>       // std::less/greater/hash
#include <tuple>            // std::tie
#include <iostream>         // std::cout/endl
#include "output_container.h"   // 打印 pair<int, int>

using namespace std;

// 请注意我们在 std 名空间中添加了特化，这是少数用户可以向 std 名空间添加内容的情况之一。
// 正常情况下，向 std 名空间添加声明或定义是禁止的，属于未定义行为。
namespace std {
    template<typename T>
    struct hash<complex<T>> {
        size_t
        operator()(const complex<T> &v) const
        noexcept {
            hash<T> h;
            return h(v.real()) + h(v.imag());
        }
    };
} // namespace std

int main() {
    unordered_set<int> s{1, 1, 2, 3, 5, 8, 13, 21};
    cout << s << endl;

    unordered_multiset<int> ms{1, 1, 2, 3, 5, 8, 13, 21};
    cout << ms << endl;


    cout << endl << "**** unordered_map ****" << endl;
    unordered_map<complex<double>, double> umc{
            {{1.0, 1.0}, 1.4142},
            {{3.0, 4.0}, 5.0}
    };
    cout << umc << endl;


    cout << endl << "**** unordered_multimap ****" << endl;
    unordered_multimap<string, int> mmp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };
    mmp.insert({"four", -4});   // multimap支持一个key存在多个相同或不同的值
    mmp.insert({"four", -4});   // multimap支持一个key存在多个相同或不同的值
    cout << mmp << endl;

    cout << mmp.find("four")->second << endl;


    // 如果需要在 multimap 里精确查找满足某个键的区间的话，建议使用 equal_range，可以一次性取得上下界（半开半闭）。如下所示：
    cout << endl << "**** unordered_multimap equal_range****" << endl;
    cout << mmp << endl;

    unordered_multimap<string, int>::iterator lower, upper;
    // 分别指向输入序列mmp中所有键等于 key 的元素所组成的子序列的起始及末尾（即最后一个元素之后的位置）位置。
    std::tie(lower, upper) = mmp.equal_range("four");

    cout << boolalpha << bool(lower != upper) << endl;       // 检测区间非空 true说明已找到有效元素
    cout << boolalpha << (upper == mmp.end()) << endl;  // true说明找到多个元素

    // 遍历打印找到的元素
    for (auto it = lower; it != upper; ++it) {
        cout << it->second << endl;
    }
}