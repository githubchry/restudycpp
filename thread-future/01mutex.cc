
#include "scoped_thread.h"
#include <chrono>
#include <mutex>
#include <iostream>

using namespace std;    // std::this_thread::sleep_for(100ms)会报错，因为100ms中的ms依赖using namespace std;

mutex output_lock;
/*
 * mutex 互斥量的基本语义是，一个互斥量只能被一个线程锁定，用来保护某个代码块在同一时间只能被一个线程执行。
 * mutex 只可默认构造，不可拷贝（或移动），不可赋值，主要提供的方法是：
 *      lock：锁定，锁已经被其他线程获得时则阻塞执行
 *      try_lock：尝试锁定，获得锁返回 true，在锁被其他线程获得时返回 false
 *      unlock：解除锁定（只允许在已获得锁时调用）
 * 如果一个线程已经锁定了某个互斥量，再次锁定会发生什么？对于mutex，回答是危险的未定义行为。——可使用递归锁 recursive_mutex
 *
 * 除了最简单的 mutex，C++ 标准库还提供了：
 *      recursive_mutex：允许同一线程可以无阻塞地多次加锁
 *      timed_mutex：允许锁定超时的互斥量
 *      recursive_timed_mutex：允许锁定超时的递归互斥量
 *      shared_mutex：允许共享和独占两种获得方式的互斥量
 *      shared_timed_mutex：允许共享和独占两种获得方式的、允许锁定超时的互斥量
 * */

void func(const char *name) {
    this_thread::sleep_for(100ms);
    /*
     * 为了避免手动加锁、解锁的麻烦，以及在有异常或出错返回时发生漏解锁，我们一般应当使用 lock_guard，而不是手工调用互斥量的 lock 和 unlock 方法。
     * lock_guard 提供了一种方便的RAII（Resource acquisition is initialization ）风格的机制来在作用域块的持续时间内拥有一个互斥量。
     * 特点：
     *      创建即加锁，作用域结束自动析构并解锁，无需手工解锁
     *      不能中途解锁，必须等作用域结束才解锁
     *      不能复制
     * */
    lock_guard<mutex> guard{output_lock};   // 防止打印交错而加锁
    cout << "I am thread " << name << '\n';
}


int main() {
    scoped_thread t1{func, "A"};
    scoped_thread t2{func, "B"};
}
