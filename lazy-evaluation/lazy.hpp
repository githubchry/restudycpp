//
// Created by chenr on 2021/6/8.
//

#ifndef RESTUDYCPP_LAZY_H
#define RESTUDYCPP_LAZY_H

#include <functional>
/*
 * 所谓惰性求值(lazy evaluation)是指表达式不在它被绑定到变量之后就立即求值，而是在我们需要到时候才会对其求值。
 * 与之相对应的是及早求值(eager evaluation)，大多数语言采用的都是这种求值方式。
 *
 * 惰性求值（Lazy Evaluation）是相对常用的编程语言中的及早求值而言的另一种求值策略，也被称之为按需调用（call-by-need），或者叫延时求值。
 * 简单地讲，惰性求值是在谋求一种平衡，一种在节省开发与节约计算资源的一种平衡策略。
 *
 * 实现惰性求值需要有模板元编程基础
 *
 * 三部曲: 提出初始化方案，检查是否初始化，返回正确值
 * */
template <typename T> class lazy
{
private:
    T value_;
    std::function<T()> policy_;
    bool initialized_;
    static T default_policy()
    {
        throw std::runtime_error("No lazy initiation policy given.");
    }
public:
    lazy() : policy_(default_policy), initialized_(false) {}
    lazy(std::function<T()> _) : policy_(_), initialized_(false) {}
    lazy(const lazy<T>& _) : policy_(_.policy_), initialized_(false) {}

    lazy<T>& operator=(const lazy<T>& _)
    {
        policy_ = _.policy_;
        initialized_ = false;
        return *this;
    }

    T& get_value()
    {
        if (!initialized_)
        {
            value_ = policy_();
            initialized_ = true;
        }
        return value_;
    }

    // T()返回的是 value_地址 而不是 lambda 表达式，外部甚至还可以更新这个值: T() = 123;
    T& operator()() { return get_value(); }

    // T本身存储的是 lambda 表达式;
    operator T() { return get_value(); }
};

/*
#include "lazy.hpp"
#include <math.h>
#include <iostream>

int main() {
    auto pi = lazy<double>([]() {
        std::cout << "惰性求值，只打印一次" << std::endl;
        return acos(-1.0);
    });

    auto area = double(pi) * 5 * 5;
    auto perimeter = (double) pi * 2 * 5;
    std::cout << "pi= " << pi() << std::endl;

    return 0; // 结果：.....
}
*/
#endif //RESTUDYCPP_LAZY_H
