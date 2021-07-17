


#include <algorithm>    // std::copy
#include <iostream>     // std::cout
#include <fstream>
#include <sstream>

// 实现一个输入行迭代器：把一个输入流（istream）的内容一行行地读取
class istream_line_reader {
public:
    // 仿照一般的容器，把迭代器iterator定义为 istream_line_reader 的嵌套类
    class iterator {
        /*
         * 以下五个类型是必须定义的, 其他泛型 C++ 代码可能会用到这五个类型；
         * 之前标准库定义了一个可以`继承`的类模板 std::iterator 来产生这些类型定义，但这个类目前已经被废弃
         * 因为`继承`方式被废弃，所以通过`嵌套`类方式来实现
         */
    public:
        // difference_type 代表迭代器之间距离的类型，定义为 ptrdiff_t 只是种标准做法（指针间差值的类型），对这个类型没什么特别作用。
        typedef ptrdiff_t difference_type;
        // value_type 是迭代器指向的对象的值类型，我们使用 string，表示迭代器指向的是字符串。
        typedef std::string value_type;
        // pointer 是迭代器指向的对象的指针类型，这儿就平淡无奇地定义为 value_type 的常指针了（我们可不希望别人来更改指针指向的内容）。
        typedef const value_type *pointer;
        // 类似的，reference 是 value_type 的常引用。
        typedef const value_type &reference;
        // iterator_category 被定义为 input_iterator_tag，标识这个迭代器的类型是input iterator（输入迭代器）
        typedef std::input_iterator_tag iterator_category;

        /*
         * 作为一个真的只能读一次的输入迭代器，有个特殊的麻烦（前向迭代器或其衍生类型没有）：到底应该让 * 负责读取还是 ++ 负责读取。
         * 我们这儿采用常见、也较为简单的做法，让 ++ 负责读取，* 负责返回读取的内容（这个做法会有些副作用，但按我们目前的用法则没有问题）。
         * 这样的话，这个 iterator 类需要有一个数据成员指向输入流，一个数据成员来存放读取的结果。
         * 根据这个思路，我们定义这个类的基本成员函数和数据成员：
         * */
    private:
        std::istream *stream_;  // 输入流
        std::string line_;      // 存放读取的结果

    public:
        // 默认构造函数，将 stream_ 清空；
        iterator() noexcept: stream_(nullptr) {}

        // 带参数的构造函数，根据传入的输入流来设置 stream_。
        // 并通过++运算符先读取一行数据到 line_, 确保在构造后调用 * 运算符时可以读取内容，符合日常先使用 *、再使用 ++ 的习惯。
        explicit iterator(std::istream &is) : stream_(&is) { ++*this; }

        // 定义 * 运算符来取得迭代器指向的文本行的引用
        reference operator*() const noexcept { return line_; }

        // 定义 -> 运算符来取得迭代器指向的文本行的指针
        pointer operator->() const noexcept { return &line_; }

        // 定义前置 ++ 运算符来读取一行输入流的内容
        iterator &operator++() {
            std::getline(*stream_, line_);
            // 一旦文件读取到尾部（或出错），则 stream_ 被清空，回到默认构造的情况。
            if (!*stream_) { stream_ = nullptr; }
            return *this;
        }

        // 定义后置 ++ 运算符, 以惯常方式使用前置 ++ 和拷贝构造来实现
        iterator operator++(int) {
            iterator temp(*this);
            ++*this;
            return temp;
        }

        // 定义 == 运算符来实现迭代器之间的比较，主要考虑文件有没有读到尾部的情况
        bool operator==(const iterator &rhs) const noexcept {
            return stream_ == rhs.stream_;
        }

        // == 运算符取反即可实现 != 运算符
        bool operator!=(const iterator &rhs) const noexcept {
            return !operator==(rhs);
        }
    };

    //有了这个 iterator 的定义后，istream_line_reader 的定义就简单得很了：

    // istream_line_reader() noexcept: stream_(nullptr) {}  // 默认构造函数感觉没啥用

    // 构造函数只是简单地把输入流的指针赋给 stream_ 成员变量。
    explicit istream_line_reader(std::istream &is) noexcept: stream_(&is) {}

    // begin 成员函数则负责构造一个真正有意义的迭代器
    iterator begin() {
        return iterator(*stream_);
    }

    // end 成员函数则只是返回一个默认构造的迭代器而已
    iterator end() const noexcept {
        return iterator();
    }

private:
    std::istream *stream_;
};

int main() {
    std::ifstream ifs(__FILE__);
    // 逐行打印文件
#if 0
    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << line << std::endl;
    }
#else
    // 通过手写输入行迭代器通过for语法糖实现std::getline功能
    for (const std::string &line : istream_line_reader(ifs)) {
        std::cout << line << std::endl;
    }
#endif

    std::cout << "===========================================" << std::endl;
    // istream_line_reader的设计因为采用常见、也较为简单的做法，让 ++ 负责读取，* 负责返回读取的内容，导致以下用法时会出现问题

    std::string context("1\n 2\n 3\n");
    std::istringstream is(context);
    istream_line_reader reader(is);

    auto begin = reader.begin();    // begin 里面调用构造函数，然后构造函数调用了++;
    //  begin 每多调用了一次，输出就少了一行……
    for (auto it = reader.begin(); it != reader.end(); ++it) {
        std::cout << *it << std::endl;
    }
    std::cout << "===========================================" << std::endl;

    std::cout << "可以看到第1行没打印出来！" << std::endl;

    return 0;
}