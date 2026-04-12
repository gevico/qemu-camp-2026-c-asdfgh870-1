#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

bool visited[MAX_ROW][MAX_COL] = {false};

int move[4][2] = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1},
};
bool dfs(int row, int col, char** path, int* path_count) {
    // 越界 / 是墙 / 已经访问过 → 走不通
    if (row < 0 || row >= MAX_ROW || col < 0 || col >= MAX_COL)
        return false;
    if (maze[row][col] == 1 || visited[row][col])
        return false;

    // ✅ 到达终点 (4,4)
    if (row == 4 && col == 4) {
        path[*path_count][0] = row + '0';
        path[*path_count][1] = col + '0';
        (*path_count)++;
        return true;
    }

    // 标记当前位置已访问
    visited[row][col] = true;

    // 把当前点加入路径
    path[*path_count][0] = row + '0';
    path[*path_count][1] = col + '0';
    (*path_count)++;

    // 尝试 4 个方向
    for (int i = 0; i < 4; i++) {
        int new_row = row + move[i][0];
        int new_col = col + move[i][1];

        // 递归走下一步
        if (dfs(new_row, new_col, path, path_count)) {
            return true;
        }
    }
    (*path_count)--;
    return false;
}

int main(void)
{
	char** path = malloc(MAX_COL * MAX_ROW  * sizeof(char*));
	for (int i = 0; i < MAX_COL * MAX_ROW; i++) {
		path[i] = malloc(2 * sizeof(char));
	}
	int path_count = 0;
	dfs(0, 0, path,&path_count);
	for (int i = path_count - 1; i >= 0; i--) {
		printf("(%c, %c)\n", path[i][0], path[i][1]);
	}
	return 0;
}