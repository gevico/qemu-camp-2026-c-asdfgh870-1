#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    Node* head = malloc(sizeof(Node));
    head->id = 1;
    head->next = head;
    Node* prev = head;
    for (int i = 2; i <= n; i++) {
        Node* p = malloc(sizeof(Node));
        p->id = i;
        p->next = head;
        prev->next = p;
        prev = p;
    }
    return head;
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* p = current;
        current = current->next;
        free(p);
    }
}
