// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
  // 移除字符串末尾的逗号和句号
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
  int in_entry = 0;

  while(fgets(line, sizeof(line), file) != NULL){
    line[strcspn(line, "\n")] = '\0';
    strcpy(current_word, line+1);
    if (fgets(line, sizeof(line), file) != NULL) {
      line[strcspn(line, "\n")] = '\0';
      strcpy(current_translation, line+6);
      hash_table_insert(table, current_word, current_translation);
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}
