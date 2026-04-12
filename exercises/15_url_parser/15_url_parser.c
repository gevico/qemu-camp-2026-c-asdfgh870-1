#define _GNU_SOURCE // 用于使用 strtok_r,不写就会使用编译器隐士的 strtok_r 函数
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char *url) {
    int err = 0;
    char *query = NULL;

    // 1. 检查输入是否为空
    if (url == NULL || strlen(url) == 0) {
        printf("错误: 输入URL为空\n");
        return -1;
    }

    // 2. 分配内存 + 拷贝字符串（必须多分配1字节存结束符\0）
    size_t url_len = strlen(url);
    query = malloc(url_len + 1);  // 这里修复！必须+1
    if (query == NULL) {
        printf("错误: 内存分配失败\n");
        return -1;
    }
    memcpy(query, url, url_len);
    query[url_len] = '\0';  // 手动加字符串结束符，关键！

    // 3. 查找 ? 位置
    char *query1 = strstr(query, "?");
    if (query1 == NULL) {
        printf("错误: URL中没有查询参数\n");
        err = -1;
        goto exit;
    }
    query1 += 1;  // 跳过 ? 字符

    // 4. 循环解析参数（修复 strtok 嵌套问题）
    char *out_save_ptr = NULL;     // 必须初始化为 NULL！！！
    char *inner_save_ptr2 = NULL;  // 也必须初始化！
    char *param = NULL;
    param = strtok_r(query1, "&", &out_save_ptr);

    while (param != NULL) {
        // 拆分 key=value    char *save_ptr2 = NULL;  // 也必须初始化！
        char *key = strtok_r(param, "=", &inner_save_ptr2);
        char *value = strtok_r(NULL, "=", &inner_save_ptr2);

        if (key == NULL || value == NULL) {
            printf("错误: 无效参数格式: %s\n", param);
            err = -1;
            goto exit;
        }

        printf("key = %s, value = %s\n", key, value);

        // 取下一个参数
        param = strtok_r(NULL, "&", &out_save_ptr);
    }

exit:
    free(query);  // 必须释放内存！
    return err;
}

int main() {
    const char *test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}