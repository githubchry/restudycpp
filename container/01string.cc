
/*

[cpp reference中文文档](https://zh.cppreference.com/w/cpp/string/basic_string)

string 是模板 basic_string 对于 char 类型的特化，可以认为是一个只存放字符char 类型数据的容器。
“真正”的容器类与 string 的最大不同点是里面可以存放任意类型的对象。

跟其他大部分容器一样， string 具有下列成员函数：
    begin 可以得到对象起始点
    end 可以得到对象的结束点
    empty 可以得到容器是否为空
    size 可以得到容器的大小
    swap 可以和另外一个容器交换其内容

对于不那么熟悉容器的人，需要知道 C++ 的 begin 和 end 是半开半闭区间：
    在容器非空时，begin 指向一个第一个元素，而 end 指向最后一个元素后面的位置； 在容器为空时，begin 等于 end。

在 string 的情况下，由于考虑到和 C 字符串的兼容，end 指向代表字符串结尾的 \0 字符。
string 的内存布局大致如下图所示：
┌───────┐ ┌───────┐    ┌───────┐ ┌───────┐
│ begin │ │ front │    │ back  │ │  end  │
└───┬───┘ └───┬───┘    └───┬───┘ └───┬───┘
    └────┬────┘            └───┐  ┌──┘
        ┌↓┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬↓┬─↓┐
        │h│e│l│l│o│ │w│o│r│l│d│!│\0│
        └─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴──┘

string 和简单的 C 字符串不同：
    string 负责自动维护字符串的生命周期
    string 支持字符串的拼接操作（+ 和 +=）
    string 支持字符串的查找操作（如 find 和 rfind）
    string 支持从 istream 安全地读入字符串（使用 getline）
    string 支持给期待 const char* 的接口传递字符串内容（使用 c_str）
    string 支持到数字的互转（stoi 系列函数和 to_string）
    等等······

推荐你在代码中尽量使用 string 来管理字符串。不过，对于对外暴露的接口，情况有一点复杂。

一般不建议在接口中使用 const string&，除非确知调用者已经持有 string：
如果函数里不对字符串做复杂处理的话，使用 const char* 可以避免在调用者只有 C 字符串时编译器自动构造 string，这种额外的构造和析构代价并不低。
反过来，如果实现较为复杂、希望使用 string 的成员函数的话，那就应该考虑下面的策略：
    1. 如果不修改字符串的内容，使用 const string& 或 C++17 的 string_view 作为参数类型。
       后者是最理想的情况，因为即使在只有 C 字符串的情况，也不会引发不必要的内存复制。
    2. 如果需要在函数内修改字符串内容、但不影响调用者的该字符串，使用 string 作为参数类型（自动拷贝）。
    3. 如果需要改变调用者的字符串内容，使用 string& 作为参数类型（通常不推荐）。
*/
#include <iostream>

using namespace std;

int main() {
    string name;
    cout << "What's your name? ";
    getline(cin, name);
    cout << "Nice to meet you, " << name << "!\n";


    // 从 const char* 创建 string
    std::string str1 = "hello";

    // 从字符串字面量创建 string
    auto str2 = "world"s;

    // 连接 string
    std::string str3 = str1 + " " + str2;

    // 打印结果
    std::cout << str3 << '\n';

    std::string::size_type pos = str3.find(" ");
    str1 = str3.substr(pos + 1); // the part after the space
    str2 = str3.substr(0, pos);  // the part till the space

    std::cout << str1 << ' ' << str2 << '\n';

    // 用下标运算符 operator[] 访问元素
    std::cout << str1[0] << '\n';
    str1[0] = 'W';
    std::cout << str1 << '\n';

}