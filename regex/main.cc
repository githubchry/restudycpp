#include <regex>
#include <iostream>

int main() {

    // 从"v1.2.3.20210525"里面捕获 1 2 3 20210525
    std::string input("v1.0.0.20210519");

    /* 如果输入字符串是从文件里面获取一行 需要注意！！！
     *
     * 各系统的文件换行方式有所不同，可能有以下多种格式
     * Win - CRLF，字符序列是'\r\n'
     * Linux/Unix - LF，字符序列是'\n'
     * Mac - CR, 字符序列是'\r'
     *
     * regex_match()的时候有可能会Win的CRLF输入进去导致失败
     * */

    std::regex express("^v([0-9]+).([0-9]+).([0-9]+).([0-9]+)*");

    std::match_results<std::string::iterator> results;

    bool ret = std::regex_match(input.begin(), input.end(), results, express);

    if (!ret) {
        std::cout << "解析版本失败:" << input << std::endl;
    }

    for (auto str : results) {
        std::cout << str << std::endl;
    }

    // auto version_str1 = results[1] + results[2] + results[3];
    auto version_str2 = std::string(results[1]) + std::string(results[2]) + std::string(results[3]);

    // std::cout << version_str1 << std::endl;
    std::cout << version_str2 << std::endl;


    // ==================================================

    input = "/sys/1377203920857726976/1442fc60a3a7/passthrough/serial/ttyS3-RS2321";
    express = "[\\S]+/passthrough/([\\S]+)/([\\S]+)";
    ret = std::regex_match(input.begin(), input.end(), results, express);
    if (!ret) {
        std::cout << "解析失败:" << input << std::endl;
    }
    for (auto str : results) {
        std::cout << str << std::endl;
    }
    return 0; // 结果：.....
}