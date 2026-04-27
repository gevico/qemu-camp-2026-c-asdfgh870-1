#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode *t = malloc(sizeof(QueueNode));
    t->tree_node = tree_node;
    t->next = NULL;
    QueueNode *q_node = q->front;
    if (q_node == NULL) {
        q->front = q->rear = t;
        return;
    } else {
        while(q_node->next != NULL) {
            q_node = q_node->next;
        }
        q_node->next = t;
        q->rear = t;
    }
}

TreeNode* dequeue(Queue *q) {
    if (q->front == NULL) {
        return NULL;
    }
    TreeNode *tree_node = q->front->tree_node;
    q->front = q->front->next;
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    TreeNode *tree_root = (TreeNode*)malloc(sizeof(TreeNode));
    tree_root->val = level_order[0];
    tree_root->left = NULL;
    tree_root->right = NULL;
    Queue *q = create_queue();
    enqueue(q, tree_root);
    int i = 1;
    while (!is_empty(q)) {
        TreeNode *tree_node = dequeue(q);
        if (level_order[i] != INT_MIN) {
            TreeNode *left_node = (TreeNode*)malloc(sizeof(TreeNode));
            left_node->val = level_order[i];
            left_node->left = NULL;
            left_node->right = NULL;
            tree_node->left = left_node;
            enqueue(q, left_node);
        }
        if (++i >= size) {
            break;
        }
        if (level_order[i] != INT_MIN) {
            TreeNode *right_node = (TreeNode*)malloc(sizeof(TreeNode));
            right_node->val = level_order[i];
            right_node->left = NULL;
            right_node->right = NULL;
            tree_node->right = right_node;
            enqueue(q, right_node);
        }
        if (++i >= size) {
            break;
        }
    }
    free_queue(q);
    return tree_root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    // Morris Traversal
    if (root == NULL) {
        return;
    }
    TreeNode *current = root;
    TreeNode *mostRight = NULL;
    while (current != NULL) {
        // cur表示当前节点，mostRight表示cur的左孩子的最右节点
        mostRight = current->left;
        if (mostRight != NULL) {
            // cur有左孩子，找到cur左子树最右节点
            while (mostRight->right != NULL && mostRight->right != current) {
                mostRight = mostRight->right;
            }
            // mostRight的右孩子指向空，让其指向cur，cur向左移动
            if (mostRight->right == NULL) {
                mostRight->right = current; 
                printf("%d ", current->val);  // 此时第一次访问节点，记录答案
                current = current->left;
                continue;          // 直接进入下一次循环（容易忘）
            } else {
                // mostRight的右孩子指向cur置为空，此时已经是第二次到该位置了，需要还原指针，让cur向右移动cur向右移动
                mostRight->right = NULL;
                current = current->right;
            }
        } else {
            /// 没有左子树的节点只到达一次直接记录答案, cur 向右移动
            printf("%d ", current->val);
            current = current->right;
        }
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
