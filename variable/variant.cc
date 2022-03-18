
#include <variant>    // std::variant
#include <iostream> // std::cout/endl
#include <optional>

using namespace std;

/*
 * [variant](https://zh.cppreference.com/w/cpp/utility/variant)
 * [如何优雅的使用 std::variant 与 std::optional](https://zhuanlan.zhihu.com/p/366537214)
 * [编译器优先识别"abc"为bool而不是std::string](https://stackoverflow.com/questions/14770252/string-literal-matches-bool-overload-instead-of-stdstring)
 *
 * std::variant是c++17加入的新容器，主要是为了提供更安全的union
*/


int main() {
    struct point {
        int x;
        int y;
    };

    // 注意bool和std::string很容易被混淆
    std::variant<int, double, std::string, point> a, b;

    a = 50;
    b = 50.0;

    std::cout << a.index() << std::endl;
    std::cout << b.index() << std::endl;

    a = false;
    std::cout << a.index() << std::endl;

    a = "hello";
    std::cout << a.index() << std::endl;

    a = std::string("hello");   // 不能直接 a = "hello";
    std::cout << a.index() << std::endl;

    b = point{123, 456};
    std::cout << b.index() << std::endl;

    // 如果强行取非对应Type的值, 则会抛出std::bad_variant_access类型的异常:
    try {
        // 以下两行等价
        // std::string s = std::get<std::string>(b);
        std::string s = std::get<2>(b);
    }
    catch (std::bad_variant_access e) {
        std::cerr << e.what() << std::endl;
    }

    // 更安全的获取方法
    auto *s = std::get_if<std::string>(&b);
    if (s == nullptr) {
        std::cout << "wrong type" << std::endl;
    } else {
        std::cout << "value is " << *s << std::endl;
    }


    //========= std::visit
    struct {
        void operator()(int v) { std::cout << v << std::endl; }

        void operator()(bool v) { std::cout << (v ? "true" : "false") << std::endl; }

        void operator()(double v) { std::cout << v << std::endl; }

        void operator()(point v) { std::cout << "{" << v.x << "," << v.y << "}" << std::endl; }

        void operator()(std::string const &v) { std::cout << v << std::endl; }
    } print;

    std::visit(print, a);
    std::visit(print, b);
    b = false;
    std::visit(print, b);

    std::cout << "下面看一种需要警惕的情况, var = `asd`：" << std::endl;
    std::variant<bool, std::string> var;  // 目的是赋值为std::string，实际上却是赋值为bool

    var = "asd";
    std::cout << "var = \"asd\" => ";
    std::visit(print, var);

    var = std::string("asd");
    std::cout << "var = std::string(\"asd\") => ";
    std::visit(print, var);


}