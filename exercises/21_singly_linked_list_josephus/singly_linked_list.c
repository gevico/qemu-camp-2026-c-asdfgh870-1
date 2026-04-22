#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    link new_node = malloc(sizeof(struct node));
    new_node->item = item;
    new_node->next = NULL;
    return new_node;
}

// 释放节点
void free_node(link p) { free(p); }

// 查找节点
link search(unsigned char key) {
    link current = head;
    while (current != NULL) {
        if (current->item == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 在链表头部插入节点
void insert(link p) {
    p->next = head;
    head = p;
}

// 删除指定节点
void delete(link p) {
    if (p == head) {
        head = head->next;
        free_node(p);
        return;
    }
    link prev = head;
    while (prev->next != p) {
        prev = prev->next;
    }
    prev->next = p->next;
    free_node(p);
}

// 遍历链表
void traverse(void (*visit)(link)) {
    link current = head;
    while (current != NULL) {
        visit(current);
        current = current->next;
    }
}

// 销毁整个链表
void destroy(void) {
    // TODO: 在这里添加你的代码
    // I AM NOT DONE
    traverse(free_node);
    head = NULL;
}

// 在链表头部推入节点
void push(link p) { 
    insert(p);
 }

// 从链表头部弹出节点
link pop(void) {
    link p = head;
    head = head->next;
    return p;
}

// 释放链表内存
void free_list(link list_head) {
    traverse(free_node);
    head = NULL;
}
