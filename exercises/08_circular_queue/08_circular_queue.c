#include <stdbool.h>
#include <stdio.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

bool in_queue[MAX_PEOPLE];

int main() {
    Queue q;
    int total_people = 50;
    for (int i = 0; i < total_people; i++) {
        q.data[i].id = i + 1;
    }
    
    // 我的实现
    // int report_interval = 5;
    // for (int i = 0; i < MAX_PEOPLE; i++) {
    //     in_queue[i] = true;
    // }

    // q.head = 0;
    // q.tail = total_people - 1;
    // q.count = total_people;

    // int index = 0;
    // while (q.count > 1) {
    //     int i = 1;
    //     while (i < report_interval) {
    //         if (in_queue[index]) {
    //             i++;
    //             if (index < q.tail) {
    //                 index++;
    //             } else {
    //                 index = q.head;
    //             }
    //             continue;
    //         }
    //         index++;
    //     }
    //     // 避免无限循环
    //     while (!in_queue[index]) {
    //         if (index == q.head) {
    //             q.head = index++;
    //         }
    //         if (index == q.tail) {
    //             int tmp_index = index;
    //             while (!in_queue[tmp_index]) tmp_index--;
    //             q.tail = tmp_index;
    //             index = q.head;
    //             continue;
    //         }
    //         index++;
    //     }
    //     in_queue[index] = false;
    //     printf("淘汰: %d\n", q.data[index].id);
    //     // 已经移除了一个，寻找下一个开始点
    //     while (!in_queue[index]) {
    //         if (index == q.head) {
    //             while (!in_queue[index]) index++;
    //             q.head = index;
    //             continue;
    //         }
    //         if (index == q.tail) {
    //             int tmp_index = index;
    //             while (!in_queue[tmp_index]) tmp_index--;
    //             q.tail = tmp_index;
    //             index = q.head;
    //             continue;
    //         }
    //         index++;
    //     }
    //     q.count--;
    // }

    int index = 0;    // 当前遍历的数组下标
    int count = 0;    // 报数计数器
    int m = 5;        // 报数间隔
    int out = 0;      // 出列计数器

    // 循环：直到只剩最后1个人（出列人数 = n-1）
    while (out < 49) {
        // 只处理还在圈中的人
        if (q.data[index].id != 0) {
            count++;  // 报数+1

            // 报到m的人出列
            if (count == m) {
                printf("淘汰: %d\n", q.data[index].id);
                q.data[index].id = 0;  // 标记出列
                out++;     // 出列人数+1
                count = 0; // 重置报数
            }
        }

        // 下标向后移动，到达末尾回到开头（循环圈）
        index++;
        if (index == 50) {
            index = 0;
        }
    }

    for (int i = 0; i < total_people; i++) {
        if (q.data[i].id != 0) {
            q.head = i;
            break;
        }
    }
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}