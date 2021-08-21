#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _cell {

	int set;
	int wall;
} cell;

void setMaze(cell** arr, int width, int height, int check);
void refill(cell** arr, int width);

int main() {

	int width, height, i, j, k, temp;
	cell** maze;
	FILE* fp;
	char c;

	srand(time(NULL));

	printf("input width: "); scanf("%d", &width);
	printf("input height: "); scanf("%d", &height);

	maze = (cell**)malloc(sizeof(cell*) * 2);
	for (i = 0; i < 2; i++)
		maze[i] = (cell*)malloc(sizeof(cell) * width);

	fp = fopen("maze.maz", "w");

	for (i = 0; i < width; i++) {
		maze[0][i].set = maze[0][i].wall = i + 1;
		maze[1][i].set = maze[1][i].wall = 0;
	}

	for (i = 0; i < 2 * width + 1; i++) {
		if (i % 2)
			fprintf(fp, "-");
		else
			fprintf(fp, "+");
	} fprintf(fp, "\n");


	for (i = 0; i < height; i++) {

		setMaze(maze, width, height, i);

		if (i == height - 1) {

			fprintf(fp, "|");
			for (j = 0; j < width - 1; j++) {
				fprintf(fp, " ");
				if (maze[0][j].set != maze[0][j + 1].set) {
					fprintf(fp, " ");
					temp = maze[0][j + 1].set;
					maze[0][j + 1].set = maze[0][j].set;
					for (k = j + 2; k < width; k++)
						if (temp == maze[0][k].set)
							maze[0][k].set = maze[0][j].set;
				}
				else
					if (maze[0][j].wall == maze[0][j + 1].wall) fprintf(fp, " ");
					else fprintf(fp, "|");
			} fprintf(fp, " |\n");
			continue;
		}

		fprintf(fp, "|");
		for (j = 0; j < width - 1; j++) {
			fprintf(fp, " ");

			if (maze[0][j].wall == maze[0][j + 1].wall) fprintf(fp, " ");
			else fprintf(fp, "|");
		} fprintf(fp, " |\n");

		for (j = 0; j < width; j++) {
			fprintf(fp, "+");
			if (maze[0][j].set != maze[1][j].set) fprintf(fp, "-");
			else fprintf(fp, " ");
		} fprintf(fp, "+\n");
	}

	for (i = 0; i < 2 * width + 1; i++) {
		if (i % 2)
			fprintf(fp, "-");
		else
			fprintf(fp, "+");
	} 

	fclose(fp);

	for (i = 0; i < 2; i++)
		free(maze[i]);
	free(maze);

	system("pause");
	return 0;
}

void setMaze(cell** maze, int width, int height, int check) {

	int i, j, k, temp, flag, cnt, vertNum, checknum, numCnt, newset;

	if (check > 0) refill(maze, width);
	//---------------------------------------------------------------------------------

	for (i = 0; i < width - 1; i++) {

		if (maze[0][i].set != maze[0][i + 1].set) {
			flag = rand() % 5;
			if (flag < 3) {
				temp = maze[0][i + 1].set;
				maze[0][i + 1] = maze[0][i];

				for (j = 0; j < width; j++)
					if (maze[0][j].set == temp && j != i)
						maze[0][j].set = maze[0][i].set;
			}
		}
	}

	//--------------------------------------------------------------------------------- 여기까지 arr 첫째줄

	temp = i = vertNum = 0;
	while (i < width) {
		flag = rand() % 28;

		if (flag < 1) {
			maze[1][i] = maze[0][i];	// 수직 내리기
			if (i > 0 && maze[1][i].set == maze[1][i - 1].set && maze[1][i].wall == maze[1][i - 1].wall) {
				if (maze[0][i].wall == maze[0][i - 1].wall && maze[0][i].set == maze[0][i - 1].set) {
					while (1) {
						newset = (i + 1) * (i + width) + maze[0][i].wall + rand() % 100;
						numCnt = 0;
						for (j = 0; j < 2; j++)
							for (k = 0; k < width; k++)
								if (newset == maze[j][k].set) numCnt++;
						if (numCnt == 0) break;
					}
					maze[1][i].set = newset;
				}
			}
			vertNum++;
		}

		if (i == 0 && maze[0][i].set != maze[0][i + 1].set && vertNum == 0) {
			maze[1][i] = maze[0][i]; // 수직 내리기
			vertNum++;
		}

		if (i < width - 1 && maze[0][i].set == maze[0][i + 1].set) i++;
		else {	// 그 다음 인덱스와 다른 경우 -> vertNum이 0이면 돌아가고 1보다 크면 진행한다.
			if (vertNum == 0) {
				for (j = temp; j <= i; j++)
					maze[1][j].set = maze[1][j].wall = 0;
				i = temp;
			}
			else {
				vertNum = 0;
				temp = i++;
			}
		}

		if (i == width - 1 && maze[0][i].set != maze[0][i - 1].set && vertNum == 0)
			maze[1][i] = maze[0][i++];
	}

	for (i = 0; i < width; i++)
		if (maze[1][i].set == 0) {

			while (1) {
				newset = maze[1][i].wall = (i + 1) * (i + width) + maze[0][i].wall + rand() % 10;
				numCnt = 0;
				for (j = 0; j < 2; j++)
					for (k = 0; k < width; k++)
						if (newset == maze[j][k].set) numCnt++;
				if (numCnt == 0) break;
			}
			maze[1][i].set = newset;
		}
}

void refill(cell** arr, int width) {

	int i, j;

	for (i = 0; i < width; i++) {

		arr[0][i].set = arr[1][i].set;
		arr[0][i].wall = arr[1][i].wall;
		arr[1][i].set = arr[1][i].wall = 0;
	}

	for (i = 0; i < width - 1; i++)
		if (arr[0][i].wall >= arr[0][i + 1].wall)
			for (j = i + 1; j < width; j++)
				arr[0][j].wall += arr[0][i].wall;
}