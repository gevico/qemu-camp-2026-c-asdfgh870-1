#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

// 如果定义了 DEBUG_LEVEL 才开启宏
#ifdef DEBUG_LEVEL

// 可变参数调试宏
#define DEBUG_PRINT(fmt, ...) \
    do { \
        /* LEVEL 1：输出函数名 + 行号 */ \
        if (DEBUG_LEVEL == 1) { \
            printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__); \
        } \
        /* LEVEL 2：额外输出变量 */ \
        else if (DEBUG_LEVEL == 2) { \
            printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
        } \
        /* LEVEL 3：输出堆栈 */ \
        else if (DEBUG_LEVEL == 3) { \
            printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
            void *buf[30]; \
            int n = backtrace(buf, 30); \
            printf("Call stack:\n"); \
            backtrace_symbols_fd(buf, n, 1); \
        } \
    } while(0)

#else

// 未定义 DEBUG_LEVEL 时宏不做事
#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif

// 测试代码
void test() {
    int x = 42;









    DEBUG_PRINT("x=%d", x);  // 第 48 行（题目要求必须在这里）
}

int main() {
    test();
    return 0;
}