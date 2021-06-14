
#include "scoped_thread.h"
#include <chrono>
#include <mutex>
#include <iostream>
#include <condition_variable>

using namespace std;    // std::this_thread::sleep_for(100ms)会报错，因为100ms中的ms依赖using namespace std;

void work(condition_variable &cv, int &result) {
    // 假装我们计算了很久
    this_thread::sleep_for(60s);
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

    /*
     * 在C++11中信号量的wait_for通过wait_util实现，前者直接使用system_clock，后者虽然可以在传入time point指定clock类型
     * 但是在实现时都会转换为系统时间，然后等待，这会导致等待过程中，如果系统时间发生变化，等待行为不正确。
     *
     * 例如如果系统事件向前（过去）变化，可能永远无法超时，如果向后（未来）变化，则可能立即超时。
     *
     * 这个问题会在设备上导致，设置设备时间时，所有信号量等待出现问题，其中最严重的是看门狗的信号量无法超时，导致无法喂狗，最终设备重启。
     *
     * 运行后修改时间：date -s 2011-11-11
     * 以下代码行为都不正确:
     */

    cv.wait_for(lock, 30s); // 最长等待30秒
    cv.wait_until(lck, std::chronos::system_clock::now() + 30s);
    // 即使这样也不行，因为内部会转换为system clock在等待，这在c++11参考note里面说的很明确
    cv.wait_until(lck, std::chronos::steady_clock::now() + 30s);

    cout << "Answer: " << result << '\n';

    /*
     * 在POSIX编程中可以通过如下方式来确保使用的递增时钟。
     *      pthread_condattr_setclock(attr, MONOTONIC_CLOCK);
     *
     * 令：在boost 1.71或是1.72版本中该问题已经解决了，只要所处系统time.h中支持MONOTONIC_CLOCK，
     * 则boost中初始化pthread cond时，会进行时钟属性设置
     *
     * C++11不知道怎么整，尽量避免使用wait_xxx....
     */
}
