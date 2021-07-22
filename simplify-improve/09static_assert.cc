

/*
C++98 的 assert 允许在运行时检查一个函数的前置条件是否成立。没有一种方法允许开发人员在编译的时候检查假设是否成立。
比如，如果模板有个参数 alignment，表示对齐，那我们最好在编译时就检查 alignment 是不是二的整数次幂。
之前人们用了一些模板技巧来达到这个目的，但输出的信息并不那么友善。

C++11 直接从语言层面提供了静态断言机制，不仅能输出更好的信息，而且适用性也更好，可以直接放在类的定义中，而不像之前用的特殊技巧只能放在函数体里。
静态断言语法上非常简单，就是：
    static_assert(编译期条件表达式,可选输出信息);
*/

int main() {
    // 静态断言针对const常量 non-constant condition for static assertion

    const int alignment = 16; // 2^4 通过
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be power of two");

    const int tmp = 15; // 不通过
    static_assert((tmp & (tmp - 1)) == 0, "tmp must be power of two");
    // static assertion failed: tmp must be power of two

    return 0;
}

