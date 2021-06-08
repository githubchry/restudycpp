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
    auto fut = async(launch::async, work);
    // 干一些其他事
    cout << "I am waiting now\n";
    cout << "Answer: " << fut.get() << '\n';
}