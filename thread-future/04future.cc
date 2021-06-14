#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std;

int work() {
    // 假装我们计算了很久
    this_thread::sleep_for(2s);
    return 42;
}

int main() {
    /*
     * 调用 async 可以获得一个未来量，launch::async 是运行策略，告诉函数模板 async 应当在新线程里异步调用目标函数。
     * 在一些老版本的 GCC 里，不指定运行策略，默认不会起新线程。
     * async 函数模板可以根据参数来推导出返回类型，在我们的例子里，返回类型是 future<int>。
     *
     */
    auto fut = async(launch::async, work);

    // TODO:干一些其他事
    cout << "I am waiting now\n";

    /*
     * 在未来量上调用 get 成员函数可以获得其结果。这个结果可以是返回值，也可以是异常，
     * 即，如果 work 抛出了异常，那 main 里在执行 fut.get() 时也会得到同样的异常，需要有相应的异常处理代码程序才能正常工作。
     *
     * 一个 future 上只能调用一次 get 函数，第二次调用为未定义行为，通常导致程序崩溃。这是设计特性必须遵循。
     *
     * 因此，同一个 future 是不能直接在多个线程里用的。 => 解决多线程使用同一个future的两种办法:
     *      1. 直接拿 future 来移动构造一个 shared_future
     *      2. 调用 future 的 share 方法来生成一个 shared_future
     * 如此就可以在多个线程里使用同一个future了......
     * 当然，每个 shared_future 上仍然还是只能调用一次 get 函数。
     */
    cout << "Answer: " << fut.get() << '\n';

    // 对比 02condition_variable, 完全同样的结果，代码大大简化，变量减到了只剩一个未来量
}