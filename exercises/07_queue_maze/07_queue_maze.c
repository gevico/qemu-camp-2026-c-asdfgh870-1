#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct node {
    int row;
    int col;
    TAILQ_ENTRY(node) nodes;
} Node;

TAILQ_HEAD(queue_head, node) q_head;

int queue_size = 0;
int direction[5] = {-1, 0, 1, 0, -1};
int visited[MAX_ROW][MAX_COL] = {0};
// 记录路径上的点的前一个点的坐标
int prev_row[MAX_ROW][MAX_COL] = {0};
int prev_col[MAX_ROW][MAX_COL] = {0};

void bfs(int row, int col){
	while (!TAILQ_EMPTY(&q_head)) {
		for (int i = 0; i < queue_size; i++) {
			Node *np = TAILQ_FIRST(&q_head);
			TAILQ_REMOVE(&q_head, np, nodes);
			visited[np->row][np->col] = 1;
			queue_size--;
			for (int k = 0; k < 4; ++k) {
				int new_row = np->row + direction[k];
				int new_col = np->col + direction[k + 1];
				if (new_row < 0 || new_row >= MAX_ROW || new_col < 0 || new_col >= MAX_COL || maze[new_row][new_col] == 1)
					continue;
				if (visited[new_row][new_col])
					continue;
				visited[new_row][new_col] = 1;
				prev_row[new_row][new_col] = np->row;
				prev_col[new_row][new_col] = np->col;
				if (new_row == 4 && new_col == 4) {			
					return;
				}
				queue_size++;
				Node *n2 = malloc(sizeof(Node));
				n2->row = new_row;
				n2->col = new_col;
				TAILQ_INSERT_TAIL(&q_head, n2, nodes);
			}
			free(np);
		}
    }
}

int main(void)
{
    TAILQ_INIT(&q_head);
	Node *n1 = malloc(sizeof(Node));
	n1->row = 0;
	n1->col = 0;
	TAILQ_INSERT_HEAD(&q_head, n1, nodes);
	queue_size++;
	bfs(0, 0);
	int path_count = 0;
	char** path = malloc(MAX_COL * MAX_ROW  * sizeof(char*));
	for (int i = 0; i < MAX_COL * MAX_ROW; i++) {
		path[i] = malloc(2 * sizeof(char));
	}
	int r=4,c=4;
	while (r!=0 || c!=0) {
		int prev_r = r;
		int prev_c = c;
		r=prev_row[prev_r][prev_c];
		c=prev_col[prev_r][prev_c];
		path[path_count][0] = r + '0';
		path[path_count][1] = c + '0';
		path_count++;
	}
	printf("(4, 4)\n");
	for (int i = 0; i < path_count; i++) {
		printf("(%c, %c)\n", path[i][0], path[i][1]);
	}
	return 0;
}