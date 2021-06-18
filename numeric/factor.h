#include <cmath>
#include <cstdint>

//两个数求最大公约数 (since C++17) 辗转相除法（也叫欧几里得算法）
uint64_t gcd(uint64_t __m, uint64_t __n)
{
    while (__n != 0) {
        uint64_t __t = __m % __n;
        __m = __n;
        __n = __t;
    }
    return __m;
}

//求最小公倍数 (since C++17)
uint64_t lcm(uint64_t __m, uint64_t __n) {
    return (__m != 0 && __n != 0) ? (abs(__m) / gcd(__m, __n)) * abs(__n) : 0;
}


//n个数求最大公约数
uint64_t gcdn(uint64_t *arr, int len) {
    if (len == 0) {
        return 0;
    } else if (len == 1)
        return (*arr);
    return gcd(arr[len - 1], gcdn(arr, len - 1));
}

//求n个数的最小公倍数
uint64_t lcmn(uint64_t *arr, int len) {
    if (len == 0) {
        return 0;
    } else if (len == 1)
        return *arr;
    else
        return lcm(arr[len - 1], lcmn(arr, len - 1));
}
