#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    char* cmd_copy = strdup(cmd);
    char* token = strtok(cmd_copy + 2, "/");
    if (token == NULL) {
        return -1;
    }
    *old_str = strdup(token);
    token = strtok(NULL, "/");
    if (token == NULL) {
        return -1;
    }
    *new_str = strdup(token);
    free(cmd_copy);

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    // 查找目标位置
    char* pos = strstr(str, old);
    if (pos == NULL) {
        return; // 没找到，直接返回
    }

    int old_len = strlen(old);
    int new_len = strlen(new);  
    int original_len = strlen(str);

    // 计算替换后总长度，严格限制不超过 MAX_LINE_LENGTH - 1
    int final_len = original_len - old_len + new_len;
    if (final_len >= MAX_LINE_LENGTH) {
        fprintf(stderr, "警告：替换后长度超限，不执行替换\n");
        return;
    }

    // 计算需要移动的偏移
    int shift = new_len - old_len;
    // 原字符串尾部位置
    char* end_pos = str + original_len;

    // 1. 向后移动（new 比 old 长）
    if (shift > 0) {
        // 从后往前搬，避免覆盖
        for (char* p = end_pos; p >= pos + old_len; p--) {
            *(p + shift) = *p;
        }
    }
    // 2. 向前移动（new 比 old 短）
    else if (shift < 0) {
        char* p = pos + old_len;
        while (*p != '\0') {
            *(p + shift) = *p;
            p++;
        }
        *(p + shift) = '\0'; // 补结束符
    }

    // 3. 写入新字符串
    memcpy(pos, new, new_len);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}