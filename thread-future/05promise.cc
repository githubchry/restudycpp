#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include "scoped_thread.h"

using namespace std;

void work(promise<int> prom) {
    // 假装我们计算了很久
    this_thread::sleep_for(2s);
    prom.set_value(42);
}

int main() {
    promise<int> prom;
    auto fut = prom.get_future();

    // 把 prom 移动给新线程，这样老线程就完全不需要管理它的生命周期了。
    scoped_thread th{work, move(prom)};

    // TODO:干一些其他事
    cout << "I am waiting now\n";
    /*
     * promise 和 future 在这里成对出现，可以看作是一个一次性管道：
     *      这边有人需要兑现承诺，往 promise 里放东西（set_value）；另一边有人就像收期货一样，到时间去 future里拿（get）就行了。
     *
     * 就这个例子而言，使用 promise 没有 async 方便，但可以看到，这是一种非常灵活的方式，你不需要在一个函数结束的时候才去设置 future 的值。
     * 仍然需要注意的是，一组promise 和 future 只能使用一次，既不能重复设，也不能重复取。
     */
    cout << "Answer: " << fut.get() << '\n';

    /*
     * promise 和 future 还有个有趣的用法是使用 void 类型模板参数。
     * 这种情况下，两个线程之间不是传递参数，而是进行同步：
     *      当一个线程在一个 future<void> 上等待时（使用get() 或 wait()），
     *      另外一个线程可以通过调用 promise<void> 上的 set_value()让其结束等待、继续往下执行。
     */

    return 0;
}