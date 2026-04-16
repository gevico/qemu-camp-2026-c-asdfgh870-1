// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
  str[strcspn(str, ",")] = '\0';
  str[strcspn(str, ".")] = '\0';
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};

  while(fgets(line, sizeof(line), file) != NULL){
    line[strcspn(line, "\n")] = '\0';
    // 去掉#前缀，从 line+1 开始复制单词
    strcpy(current_word, line + 1);
    if (fgets(line, sizeof(line), file) != NULL) {
      line[strcspn(line, "\n")] = '\0';
      // 去掉"Trans:"前缀，从 line+6 开始复制翻译
      strcpy(current_translation, line + 6);
      hash_table_insert(table, current_word, current_translation);
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("../exercises/20_mybash/src/mytrans/dict.txt", table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 text.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok 按空格分割单词
    char *token = strtok(line, " ");
    while (token != NULL) {
        to_lowercase(token);
        trim(token);
        const char *translation = hash_table_lookup(table, token);
        if (translation != NULL) {
            printf("原文: %s\t翻译: %s \n", token, translation);
        } else {
            printf("原文: %s\t未找到该单词的翻译。\n", token);
        }
        token = strtok(NULL, " ");
    }
  }

  free_hash_table(table);
  return 0;
}