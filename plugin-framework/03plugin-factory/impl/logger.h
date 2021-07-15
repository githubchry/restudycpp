
#pragma once

//#define ENABLE_LOG

#define COLOR_STR_NONE          "\033[0m"
#define COLOR_STR_BLACK         "\033[0;30m"
#define COLOR_STR_LIGHT_GRAY    "\033[0;37m"
#define COLOR_STR_DARK_GRAY     "\033[1;30m"
#define COLOR_STR_BLUE          "\033[0;34m"
#define COLOR_STR_LIGHT_BLUE    "\033[1;34m"
#define COLOR_STR_GREEN         "\033[0;32m"
#define COLOR_STR_LIGHT_GREEN   "\033[1;32m"
#define COLOR_STR_CYAN          "\033[0;36m"
#define COLOR_STR_LIGHT_CYAN    "\033[1;36m"
#define COLOR_STR_RED           "\033[0;31m"
#define COLOR_STR_LIGHT_RED     "\033[1;31m"
#define COLOR_STR_PURPLE        "\033[0;35m"
#define COLOR_STR_LIGHT_PURPLE  "\033[1;35m"
#define COLOR_STR_BROWN         "\033[0;33m"
#define COLOR_STR_YELLOW        "\033[1;33m"
#define COLOR_STR_WHITE         "\033[1;37m"

#ifdef ENABLE_LOG
#include <zlog.h>
#define log_debug(str, args...)     dzlog_debug (COLOR_STR_LIGHT_GREEN  str COLOR_STR_NONE, ##args);
#define log_info(str, args...)      dzlog_info  (COLOR_STR_LIGHT_GRAY   str COLOR_STR_NONE, ##args);
#define log_notice(str, args...)    dzlog_notice(COLOR_STR_LIGHT_CYAN   str COLOR_STR_NONE, ##args);
#define log_warn(str, args...)      dzlog_warn  (COLOR_STR_YELLOW       str COLOR_STR_NONE, ##args);
#define log_error(str, args...)     dzlog_error (COLOR_STR_LIGHT_RED    str COLOR_STR_NONE, ##args);
#define log_fatal(str, args...)     dzlog_fatal (COLOR_STR_DARK_GRAY    str COLOR_STR_NONE, ##args);

class scoped_zlog {
public:
    scoped_zlog(const char* conf_path, const char* conf_node) {
        if (dzlog_init(conf_path, conf_node)) {
            printf("dzlog_init fail: %s-%s\n", conf_path, conf_node);
            exit(-1);
        }
        printf("dzlog_init\n");
    }

    ~scoped_zlog() {
        zlog_fini();
        printf("zlog_fini\n");
    }
};

#else

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define TIME_STR        "[%04d-%02d-%02d %02d:%02d:%02d]"
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

#define log_debug(str, args...)     do {struct timeval tm_tmp = { 0 }; struct tm *ptm = NULL; gettimeofday(&tm_tmp, NULL); ptm = gmtime(&tm_tmp.tv_sec); \
    printf(COLOR_STR_LIGHT_GREEN " " TIME_STR " %s:%03d, %s> " COLOR_STR_NONE " " str, \
    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, filename(__FILE__), __LINE__, __FUNCTION__, ## args); } while (0)

#define log_info(str, args...)     do {struct timeval tm_tmp = { 0 }; struct tm *ptm = NULL; gettimeofday(&tm_tmp, NULL); ptm = gmtime(&tm_tmp.tv_sec); \
    printf(COLOR_STR_LIGHT_GRAY " " TIME_STR " %s:%03d, %s> " COLOR_STR_NONE " " str, \
    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, filename(__FILE__), __LINE__, __FUNCTION__, ## args); } while (0)

#define log_notice(str, args...)     do {struct timeval tm_tmp = { 0 }; struct tm *ptm = NULL; gettimeofday(&tm_tmp, NULL); ptm = gmtime(&tm_tmp.tv_sec); \
    printf(COLOR_STR_LIGHT_CYAN " " TIME_STR " %s:%03d, %s> " COLOR_STR_NONE " " str, \
    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, filename(__FILE__), __LINE__, __FUNCTION__, ## args); } while (0)

#define log_warn(str, args...)     do {struct timeval tm_tmp = { 0 }; struct tm *ptm = NULL; gettimeofday(&tm_tmp, NULL); ptm = gmtime(&tm_tmp.tv_sec); \
    printf(COLOR_STR_YELLOW " " TIME_STR " %s:%03d, %s> " COLOR_STR_NONE " " str, \
    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, filename(__FILE__), __LINE__, __FUNCTION__, ## args); } while (0)

#define log_error(str, args...)     do {struct timeval tm_tmp = { 0 }; struct tm *ptm = NULL; gettimeofday(&tm_tmp, NULL); ptm = gmtime(&tm_tmp.tv_sec); \
    printf(COLOR_STR_LIGHT_RED " " TIME_STR " %s:%03d, %s> " COLOR_STR_NONE " " str, \
    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, filename(__FILE__), __LINE__, __FUNCTION__, ## args); } while (0)

#define log_fatal(str, args...)     do {struct timeval tm_tmp = { 0 }; struct tm *ptm = NULL; gettimeofday(&tm_tmp, NULL); ptm = gmtime(&tm_tmp.tv_sec); \
    printf(COLOR_STR_DARK_GRAY " " TIME_STR " %s:%03d, %s> " COLOR_STR_NONE " " str, \
    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, filename(__FILE__), __LINE__, __FUNCTION__, ## args); } while (0)

#endif


#define assert_param(expect) \
do { \
    if (!(expect)) \
    { \
        log_error("assert failed, expect(%s)\n", #expect); \
        return; \
    } \
} while (0)

#define assert_param_return(expect, ret) \
do { \
    if (!(expect)) \
    { \
        log_error("assert failed, expect \"%s\"\n", #expect); \
        return ret; \
    } \
} while (0)

#define assert_param_do(expect, jobs) \
do { \
    if (!(expect)) \
    { \
        log_error("assert failed, expect \"%s\"\n", #expect); \
        { jobs; } \
    } \
} while (0)



