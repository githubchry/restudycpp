
#include <iostream>     // std::cout
#include <vector>
#include <fstream>
#include <codecvt>
#include <locale>

// g++ 增加编译参数 => -std=c++11
// cmake 指定为 C++11 版本 => set(CMAKE_CXX_STANDARD 11)

// [列表初始化](https://zh.cppreference.com/w/cpp/utility/initializer_list)

int main() {
    //==================== 初始化列表语法 ====================
    // 在 C++98 里，标准容器比起 C 风格数组至少有一个明显的劣势：不能在代码里方便地初始化容器的内容。比如，对于数组你可以写：
    int a[] = {1, 2, 3, 4, 5};

    // 而对于 vector 你却得写：
    std::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);

    // C++11 引入了列表初始化，允许以更简单的方式来初始化对象。现在我们初始化容器也可以和初始化数组一样简单了：
    std::vector<int> v2{1, 2, 3, 4, 5};

    /*
     * 同样重要的是，这不是对标准库容器的特殊魔法，而是一个通用的、可以用于各种类的方法。
     * 从技术角度，编译器的魔法只是对 {1, 2, 3} 这样的表达式自动生成一个初始化列表，在这个例子里其类型是 initializer_list<int>。
     * 程序员只需要声明一个接受 initializer_list 的构造函数即可使用。
     * 从效率的角度，至少在动态对象的情况下，容器和数组也并无二致，都是通过拷贝（构造）进行初始化。
     *
     * ==================== 统一初始化语法 ====================
     *
     * 有时候可能注意到有人在代码里使用了大括号 {} 来进行对象的初始化。
     * 这当然也是 C++11 引入的新语法，能够代替很多小括号 () 在变量初始化时使用。
     * 这被称为统一初始化（uniform initialization）。
     *
     * 大括号对于构造一个对象而言，最大的好处是避免了 C++ 里“最令人恼火的语法分析”（the most vexing parse）。
     * 假设有一个类，原型如下：
     */
    class path_to_wsl {
    private:
        std::string str_;
    public:
        // D:\codes\main.cc => /mnt/d/codes/main.cc
        path_to_wsl(const char *win_str) : str_(win_str) {
            std::cout << str_ << " => ";
            // 盘符小写
            char disk_symbol = tolower(str_.at(0));
            str_.replace(0, 1, std::string(1, disk_symbol));
            // 删除盘符后面的冒号
            str_.replace(str_.find(":"), 1, "");
            // 替换反斜杠`\`为斜杠`/`
            std::string::size_type pos = 0;
            while ((pos = str_.find('\\')) != std::string::npos)   //替换所有指定子串
            {
                str_.replace(pos, 1, "/");
            }
            // 增加挂载路径前缀
            str_ = "/mnt/" + str_;
            std::cout << str_ << std::endl;
        };

        // 不要加 explicit，让编译器可隐式自动将 wstring_to_utf8 类型转换成 const wchar_t * 类型
        operator const char *() { return str_.c_str(); }
    };


    {
        // 正常情况下，std::ifstream 的用法如下
        std::ifstream ifs(__FILE__);
        std::cout << ifs.is_open() << std::endl;
    }

    // 一个 Windows 下的 路径
    const char *filename = "D:\\codes\\git\\restudycpp\\simplify-improve\\05initialization.cc";

    {
        // 模拟在 Windows 下想使用这个类来帮助转换文件名，打开文件：
        std::ifstream ifs(path_to_wsl(filename));

        // 你随后就会发现，ifs 的行为无论如何都不正常, 甚至连构造函数的打印都没有输出
        // std::cout<<ifs.is_open()<<std::endl; // 编译错误
    }

    {
        // 最后，要么你自己查到，要么有人告诉你，上面这个写法会被编译器认为是和下面的写法等价的：
        std::ifstream ifs(path_to_wsl filename);
        // 换句话说，编译器认为你是声明了一个叫 ifs 的函数，而不是对象！
    }

    // 如果你把任何一对小括号替换成大括号（或者都替换，如下），则可以避免此类问题：
    std::ifstream ifs(path_to_wsl{filename});
    std::cout << ifs.is_open() << std::endl; // 编译通过

    /*
     推而广之，你几乎可以在所有初始化对象的地方使用大括号而不是小括号。
     它还有一个附带的特点：当一个构造函数没有标成 explicit 时，你可以使用大括号不写类名来进行构造，如果调用上下文要求那类对象的话。比如：
        Obj getObj() {
            return {1.0};
        }

     如果 Obj 类可以使用浮点数进行构造的话，上面的写法就是合法的。
     如果有无参数、多参数的构造函数，也可以使用这个形式。
     除了形式上的区别，它跟 Obj(1.0) 的主要区别是：
     后者可以用来调用 Obj(int)，而使用大括号时编译器会拒绝“窄”转换，不接受以{1.0} 或 Obj{1.0} 的形式调用构造函数 Obj(int)。

     这个语法主要的限制是，如果一个构造函数既有使用初始化列表的构造函数，又有不使用初始化列表的构造函数，
     那编译器会`千方百计`地试图调用使用`初始化列表`的构造函数，导致各种意外。所以，如果给一个推荐的话，那就是：
        如果一个类没有使用初始化列表的构造函数时，初始化该类对象可全部使用统一初始化语法。
        如果一个类有使用初始化列表的构造函数时，则只应用在初始化列表构造的情况。
     */

    return 0;
}

