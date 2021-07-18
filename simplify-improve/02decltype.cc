
#include <iostream>     // std::cout

/*
## decltype
decltype 的用途是获得一个表达式的类型，结果可以跟类型一样使用。它有两个基本用法：
    decltype(变量名) 可以获得变量的精确类型。
    decltype(表达式) （表达式不是变量名，但包括 decltype((变量名)) 的情况）可以获得表达式的引用类型；除非表达式的结果是个纯右值（prvalue），此时结果仍然是值类型。

如果我们有 int a; 那么：
    decltype(a) 会获得 int（因为 a 是 int）。
    decltype((a)) 会获得 int&（因为 a 是 lvalue）。
    decltype(a + a) 会获得 int（因为 a + a 是 prvalue）。


## decltype(auto)
通常情况下，能写 auto 来声明变量肯定是件比较轻松的事。但这儿有个限制，你需要在写下 auto 时就决定你写下的是个引用类型还是值类型。
根据类型推导规则:
    auto 是值类型
    auto& 是左值引用类型
    auto&& 是转发引用（可以是左值引用，也可以是右值引用）

不过，decltype(expr) 既可以是值类型，也可以是引用类型。因此，我们可以这么写：
     decltype(expr) a = expr;

这种写法明显不能让人满意，特别是表达式很长的情况（而且，任何代码重复都是潜在的问题）。
为此，C++14 引入了 decltype(auto) 语法。对于上面的情况，我们只需要像下面这样写就行了。
    decltype(auto) a = expr;

*/

/*
## 函数返回值类型推断
从 C++14 开始，函数的返回值也可以用 auto 或 decltype(auto) 来声明了。
同样的，用 auto 可以得到值类型，用 auto& 或 auto&& 可以得到引用类型；
而用decltype(auto) 可以根据返回表达式通用地决定返回的是值类型还是引用类型。

和这个形式相关的有另外一个语法，`后置返回值类型声明`。严格来说，这不算“类型推断”，不过我们也放在一起讲吧。它的形式是这个样子：
    auto foo(参数) -> 返回值类型声明 {
        // 函数体
    }
通常，在返回类型比较复杂、特别是返回类型跟参数类型有某种推导关系时会使用这种语法。
*/

template<typename T, typename U>
auto add11(T x, U y) -> decltype(x + y) {
    return x + y;
}

// 令人欣慰的是从 C++14 开始是可以直接让普通函数具备返回值推导，因此下面的 写法变得合法：
template<typename T, typename U>
auto add14(T x, U y) {
    return x + y;
}

int main() {
    auto x = 1;
    auto y = 2.1;
    decltype(x + y) z = 15.5;
    std::cout << z << std::endl;

    std::cout << add11<>(x, y) << std::endl;
    std::cout << add14<>(x, y) << std::endl;

    return 0;
}