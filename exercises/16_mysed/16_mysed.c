#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

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

    return 0;
}

void replace_first_occurrence(char* str, const char* old_str, const char* new_str) {
    if (str == NULL || old_str == NULL || new_str == NULL) {
        return;
    }

    // 查找目标位置
    char* pos = strstr(str, old_str);
    if (pos == NULL) {
        return; // 没找到，直接返回
    }

    int old_len = strlen(old_str);
    int new_len = strlen(new_str);
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
    memcpy(pos, new_str, new_len);
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
