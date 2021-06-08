
#include <utility>
#include <thread>

class scoped_thread {
public:
    // 使用了可变模板和完美转发来构造 thread 对象
    template<typename... Arg>
    scoped_thread(Arg &&... arg):thread_(std::forward<Arg>(arg)...) {}

    // thread 不能拷贝，但可以移动；我们也类似地实现了移动构造函数。
    scoped_thread(scoped_thread &&other) : thread_(move(other.thread_)) {}

    scoped_thread(const scoped_thread &) = delete;

    ~scoped_thread() {
        // 只有 joinable 的 thread 才可以对其调用 join 成员函数，否则会引发异常。
        // 已经 join 的、已经 detach 的或者空的线程对象都不满足 joinable
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    std::thread thread_;
};
