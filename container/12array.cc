#include <map>
#include <array>    // std::array
#include <iostream> // std::cout/endl
#include "output_container.h"

using namespace std;

/*
 [array](https://zh.cppreference.com/w/cpp/container/array)

array容器是 C 数组的替代品。C 数组在 C++ 里继续存在，主要是为了保留和 C 的向后兼容性。C 数组本身和 C++ 的容器相差是非常大的：
    C 数组没有 begin 和 end 成员函数（虽然可以使用全局的 begin 和 end 函数）
    C 数组没有 size 成员函数（得用一些模板技巧来获取其长度）
    C 数组作为参数有退化行为，传递给另外一个函数后那个函数不再能获得 C 数组的长度和结束位置

*/
//在 C 的年代，大家有时候会定义这样一个宏来获得数组的长度：
#define ARRAY_LEN(arr)  (sizeof(arr) / sizeof((arr)[0]))

//如果在一个函数内部对数组参数使用这个宏，结果肯定是错的:
void test_size(int arr[]) {
    cout << ARRAY_LEN(arr) << endl;
}

/*
现在 GCC 会友好地发出警告：
``` shell
warning: sizeof on array function parameter will return size of ‘int *’ instead of ‘int [8]’ [-Wsizeof-array-argument]
    34 | #define ARRAY_LEN(a)  (sizeof(a) / sizeof((a)[0]))
       |                                ^
```
*/

//C++17 直接提供了一个 size 方法，可以用于提供数组长度，并且在数组退化成指针的情况下会直接失败：
void test_std_size(int arr[]) {
    // cout << std::size(arr) << endl; // 编译失败
}

/*
此外，C 数组也没有良好的复制行为。你无法用 C 数组作为 map 或 unordered_map 的键类型。下面的代码演示了失败行为：
    #include <map>
    typedef char mykey_t[8];
    int main()
        std::map<mykey_t, int> mp;
        mykey_t mykey{"hello"};
        mp[mykey] = 5; // 轰，大段的编译错误
    }

如果不用 C 数组的话，我们该用什么来替代呢？
我们有三个可以考虑的选项：
    1. 如果数组较大的话，应该考虑 vector。vector 有最大的灵活性和不错的性能。
    2. 对于字符串数组，当然应该考虑 string。
    3. 如果数组大小固定（C 的数组在 C++ 里本来就是大小固定的）并且较小的话，应该考虑array。
       array 保留了 C 数组在栈上分配的特点，同时，提供了 begin、end、size 等通用成员函数。

array 可以避免 C 数组的种种怪异行径。上面的失败代码，如果使用 array 的话，稍作改动就可以通过编译：
*/
typedef std::array<char, 8> mykey_t;

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    cout << ARRAY_LEN(arr) << endl;
    test_size(arr);  // 打印2 明显是错误

    std::map<mykey_t, int> mp;
    mykey_t mykey{"hello"};
    mp[mykey] = 5; // OK
    std::cout << mp << std::endl;
}