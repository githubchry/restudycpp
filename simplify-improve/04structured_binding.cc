
#include <iostream>     // std::cout
#include <map>

using namespace std;

// g++ 增加编译参数 => -std=c++17
// cmake 指定为 C++17 版本 => set(CMAKE_CXX_STANDARD 17)

int main() {

    // 在讲关联容器的时候, container/10associative_container.cc 有过这样一个例子：
    multimap<string, int> mmp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };
    {
        // 如果需要在 multimap 里精确查找满足某个键的区间的话，建议使用 equal_range，可以一次性取得上下界（半开半闭）。如下所示：
        multimap<string, int>::iterator lower, upper;
        // 分别指向输入序列mmp中所有键等于 key 的元素所组成的子序列的起始及末尾（即最后一个元素之后的位置）位置。
        std::tie(lower, upper) = mmp.equal_range("four");
        cout << (lower != upper) << endl;       // 检测区间非空 true说明已找到有效元素
        cout << (upper == mmp.end()) << endl;   // true说明找到多个元素
    }

    // 这个例子里，equal_range 的返回值是个 pair，我们希望用两个变量来接收数值，就不得不声明了两个变量 lower, upper ，然后使用 tie 来接收结果。
    // 在 C++11/14 里，这里是没法使用 auto 的。好在C++17 引入了一个新语法，解决了这个问题。目前，我们可以把上面的代码简化为：
    auto[lower, upper] = mmp.equal_range("four");
    // 这个语法使得我们可以用 auto 声明变量来分别获取 pair 或 tuple 返回值里各个子项，可以让代码的可读性更好。

    cout << (lower != upper) << endl;       // 检测区间非空 true说明已找到有效元素
    cout << (upper == mmp.end()) << endl;  // true说明找到多个元素

    // 同样的， 该语法可以用在 for 遍历 map 结构,
    for (auto[key, val] : mmp) {
        cout << key << "-" << val << endl;
    }

    // 效果等同于：
    for (const auto& it : mmp) {
        cout << it.first << "-" << it.second << endl;
    }

    return 0;
}
