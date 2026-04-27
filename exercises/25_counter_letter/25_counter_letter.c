#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// 定义BST节点结构
typedef struct TreeNode {
    char letter;        // 存储字母
    int count;          // 计数
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* create_node(char letter) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->letter = tolower(letter);  // 转换为小写
    newNode->count = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 向BST中插入节点或更新计数
TreeNode* insert_or_update(TreeNode* root, char letter) {
    if (root == NULL) {
        return create_node(letter);
    }
    if (tolower(letter) < root->letter) {
        root->left = insert_or_update(root->left, letter);
    } else if (tolower(letter) > root->letter) {
        root->right = insert_or_update(root->right, letter);
    } else {
        root->count++;
    }
    return root;
}

// 中序遍历BST并打印结果（按字母顺序）
void inorder_traversal(TreeNode* root) {
    if (root != NULL) {
        TreeNode* cur = root;
        TreeNode* mostRight = NULL;
        while (cur != NULL) {
            // cur表示当前节点，mostRight表示cur的左孩子的最右节点
            mostRight = cur->left;
            if (mostRight != NULL) {
                // cur有左孩子，找到cur左子树最右节点
                while (mostRight->right != NULL && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }
                // mostRight的右孩子指向空，让其指向cur，cur向左移动
                if (mostRight->right == NULL) {
                    mostRight->right = cur;
                    cur = cur->left;
                    continue;            // 直接进入下一次循环（容易忘）
                } else {
                    printf("%c:%d\n", cur->letter, cur->count);        // 第二次到达，记录答案 
                    mostRight->right = NULL;  // mostRight的右孩子指向cur，让其指向空，cur向右移动     
                    cur = cur->right;
                }
            } else {
                printf("%c:%d\n", cur->letter, cur->count);   // 没有左子树的节点只到达一次直接记录答案, cur 向右移动
                cur = cur->right;
            }
        }
    }
}

// 释放BST内存
void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(int argc, char *argv[]) {
    const char* file_path = "paper.txt";
    
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    TreeNode* root = NULL;
    int c;
    
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {  // 只处理字母字符
            root = insert_or_update(root, c);
        }
    }
    
    fclose(file);
    
    // 按字母顺序输出结果
    inorder_traversal(root);
    
    // 释放内存
    free_tree(root);
    
    return 0;
}