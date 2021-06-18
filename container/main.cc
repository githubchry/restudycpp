
#include <map>
#include <iostream>

int main() {
    std::map<std::string, int> mp{
            {"one",   1},
            {"two",   2},
            {"three", 3},
            {"four",  4}
    };


    typedef union {
        char valChar;
        bool valBool;
        int16_t valShort;
        int32_t valInt;
        int64_t valInt64;
    } TagValue;

    TagValue i32;
    uint8_t *p = (uint8_t *) &i32;

    for (int i = 0; i < sizeof(i32); ++i) printf("0x%02x ", p[i]);printf("\n");

    i32.valInt = -1;
    for (int i = 0; i < sizeof(i32); ++i) printf("0x%02x ", p[i]);printf("\n");

    i32.valInt64 = -1;
    for (int i = 0; i < sizeof(i32); ++i) printf("0x%02x ", p[i]);printf("\n");


    TagValue tmp = i32;
    p = (uint8_t *) &tmp;
    for (int i = 0; i < sizeof(i32); ++i) printf("0x%02x ", p[i]);printf("\n");

    return 0;
}