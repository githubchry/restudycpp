/*
 * 很常见的一个输出迭代器是 back_inserter 返回的类型 back_inserter_iterator 了；用它我们可以很方便地在容器的尾部进行插入操作。
 * 另外一个常见的输出迭代器是ostream_iterator，方便我们把容器内容“拷贝”到一个输出流。示例如下：
 */
#include <algorithm>    // std::copy
#include <iterator>     // std::back_inserter
#include <vector>       // std::vector
#include <iostream>     // std::cout

int main() {

    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2;

    // 把v1的内容逐个“拷贝”到v2的尾部进行插入操作
    copy(v1.begin(), v1.end(), std::back_inserter(v2));

    // 把v2的内容逐个“拷贝”到一个输出流, 这里std::cout是标准输出流
    copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));

    return 0;
}