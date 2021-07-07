#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include "scoped_thread.h"

using namespace std;

int work() {
    // 假装我们计算了很久
    this_thread::sleep_for(2s);
    return 42;
}

int main() {
    packaged_task<int()> task{work};
    auto fut = task.get_future();

    // 把 prom 移动给新线程，这样老线程就完全不需要管理它的生命周期了。
    scoped_thread th{move(task)};

    // TODO:干一些其他事
    cout << "I am waiting now\n";
    /*
     * 打包任务里打包的是一个函数，模板参数就是一个函数类型。跟 thread、future、promise 一样，packaged_task 只能移动，不能复制。
     * 它是个函数对象，可以像正常函数一样被执行，也可以传递给 thread 在新线程中执行。
     *
     * 它的特别地方，自然也是你可以从它得到一个未来量了。
     * 通过这个未来量，你可以得到这个打包任务的返回值，或者，至少知道这个打包任务已经执行结束了。
     */
    cout << "Answer: " << fut.get() << '\n';

    return 0;
}