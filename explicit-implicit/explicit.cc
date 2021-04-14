#include <malloc.h>
#include <string.h>

// [C++ explicit关键字详解](https://blog.csdn.net/guoyunfei123/article/details/89003369)
// 使用关键字explicit的类声明, 显示转换

class CxString {
public:
    explicit CxString(int size)
    {
        _size = size; // string的预设大小
        _pstr = (char*)malloc(size + 1); // 分配string的内存
        memset(_pstr, 0, size + 1);
    }

    CxString(const char* p)
    {
        int size = strlen(p);
        _pstr = (char*)malloc(size + 1); // 分配string的内存
        strcpy(_pstr, p); // 复制字符串
        _size = strlen(_pstr);
    }

private:
    char* _pstr = nullptr;
    int _size;
    // 析构函数这里不讨论, 省略...
};

int main()
{
    CxString string1(24);       // 这样是OK的
    // CxString string2 = 10;   // 这样是不行的, 因为explicit关键字取消了隐式转换
    // CxString string3;        // 这样是不行的, 因为没有默认构造函数
    CxString string4("aaaa");   // 这样是OK的
    CxString string5 = "bbb";   // 这样也是OK的, 调用的是CxString(const char *p)
    // CxString string6 = 'c';  // 这样是不行的, 其实调用的是CxString(int size), 且size等于 'c'的ascii码,但explicit关键字取消了隐式转换
    // string1 = 2;             // 这样也是不行的, 因为取消了隐式转换
    // string2 = 3;             // 这样也是不行的, 因为取消了隐式转换
    string4 = string1;          // 这样也是OK的, 至少编译是没问题的, 但是如果析构函数里用free释放_pstr内存指针的时候可能会报错, 完整的代码必须重载运算符"=", 并在其中处理内存释放
}