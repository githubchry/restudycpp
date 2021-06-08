
#include "scoped_thread.h"
#include <chrono>
#include <mutex>
#include <iostream>
#include <condition_variable>

using namespace std;    // std::this_thread::sleep_for(100ms)会报错，因为100ms中的ms依赖using namespace std;

void work(condition_variable &cv, int &result) {
    // 假装我们计算了很久
    this_thread::sleep_for(2s);
    result = 42;
    cv.notify_one();
}


int main() {
    condition_variable cv;
    int result;

    scoped_thread t1{work, ref(cv), ref(result)};

    // 干一些其他事
    cout << "I am waiting now\n";
    /*
     * unique_lock中的unique表示独占所有权。
     * 用法：新建一个unique_lock时给传入一个对象作为参数 => unique_lock<对象类型> lock{对象};
     * 作用：加锁时新建一个对象lock，而这个对象生命周期结束后自动解锁。
     * */
    mutex cv_mut;
    unique_lock<mutex> lock{cv_mut};    // unique_lock独占的是mutex对象，即对mutex锁的独占。
    cv.wait(lock);
    cout << "Answer: " << result << '\n';

    /*
     * 可以看到，为了这个小小的“计算”，我们居然需要定义 5 个变量：线程、条件变量、互斥量、单一锁和结果变量。
     * 我们也需要用 ref 模板来告诉 thread 的构造函数，我们需要传递条件变量和结果变量的引用，
     * 因为 thread 默认复制或移动所有的参数作为线程函数的参数。
     * 这种复杂性并非逻辑上的复杂性，而只是实现导致的，不是我们希望的写代码的方式
     *      ——引入看更高层的抽象，未来量 future
     * */
}
