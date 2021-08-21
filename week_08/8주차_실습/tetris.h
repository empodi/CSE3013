#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>

#define WIDTH	10
#define HEIGHT	22
#define NOTHING	0
#define QUIT	'q'
#define NUM_OF_SHAPE	7
#define NUM_OF_ROTATE	4
#define BLOCK_HEIGHT	4
#define BLOCK_WIDTH	4
#define BLOCK_NUM	10
#define VISIBLE_BLOCKS 6

// menu number
#define MENU_PLAY '1'
#define MENU_EXIT '4'
#define MENU_RANK '2'

// ����� �̸��� ����
#define NAMELEN 16

#define CHILDREN_MAX 36

typedef struct _RecNode {

	int level;
	int accScore;
	int x, y, r;
    double totalTime;
	struct _RecNode* super;
	char recField[HEIGHT][WIDTH];
	struct _RecNode** arr;

} RecNode;

/* [blockShapeID][# of rotate][][]*/
const char block[NUM_OF_SHAPE][NUM_OF_ROTATE][BLOCK_HEIGHT][BLOCK_WIDTH] = {
	{/*[0][][][]					�̢̢̢�*/
		{/*[][0][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}
		}
	},
	{/*[1][][][];					  �̢̢�*/
		{/*[][0][][]				      ��*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0 ,1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[2][][][];					  �̢̢�*/
		{/*[][0][][]				  ��*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}
		}
	},
	{/*[3][][][];					  �̢̢�*/
		{/*[][0][][]				    ��*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	},
	{/*[4][][][];					  �̢�*/
		{/*[][0][][]				  �̢�*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[5][][][];					  �̢�*/
		{/*[][0][][]				�̢�*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}
		}
	},
	{/*[6][][][];					�̢�*/
		{/*[][0][][]				  �̢�*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	}
};

char field[HEIGHT][WIDTH];	/* ��Ʈ������ ���� ���� ȭ�� */
int nextBlock[BLOCK_NUM];	/* ���� ���� ID�� ���� ���� ID���� ����; [0]: ���� ��; [1]: ���� �� */
int blockRotate, blockY, blockX;	/* ���� ���� ȸ��, ���� Y ��ǥ, ���� X ��ǥ*/
int score;			/* ������ ����*/
int gameOver = 0;			/* ������ ����Ǹ� 1�� setting�ȴ�.*/
int timed_out;
int recommendR, recommendY, recommendX; // ��õ �� ��ġ ����. ���ʴ�� ȸ��, Y ��ǥ, X ��ǥ

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate);
void InitTetris();
void DrawOutline();
int GetCommand();
int ProcessCommand(int command);
void BlockDown(int sig);
int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX);
void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX);
void DrawField();
int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX);
int DeleteLine(char f[HEIGHT][WIDTH]);
void DrawNextBlock(int* nextBlock);
void PrintScore(int score);
void DrawBox(int y, int x, int height, int width);
void DrawBlock(int y, int x, int blockID, int blockRotate, char tile);
void DrawRecommend(int y, int x, int blockID, int blockRotate);
void DrawShadow(int y, int x, int blockID, int blockRotate);
void play();
char menu();
void createRankList();
void rank();
void writeRankFile();
void newRank(int score);
int recommend(RecNode* root);
void recurRec(RecNode* Troot, int* maxScore, int* xR, int* yR, int* rR, int* nodeCnt);
void recommendedPlay();
void copyField(char srcField[HEIGHT][WIDTH], char destField[HEIGHT][WIDTH]);
RecNode* findLevelOne(RecNode* Node);

#endif
