#include "tetris.h"
#include "LinkedList.h"
#include <time.h>

static struct sigaction act, oact;
LinkedList list;
RecNode* root; 

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	createRankList();
    
    root = (RecNode*)malloc(sizeof(RecNode));
    root->totalTime = 0;

	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit = 1; break;
        case MENU_RANK: rank(); break;
		default: break;
		}
	}
    writeRankFile();
	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	int i, j;

	for (j = 0; j < HEIGHT; j++)
		for (i = 0; i < WIDTH; i++)
			field[j][i] = 0;

	for (i = 0; i < BLOCK_NUM; i++)
		nextBlock[i] = rand() % 7;

	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	score = 0;
	gameOver = 0;
	timed_out = 0;

	DrawOutline();
	DrawField();
	recommend(root);
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {
	int i, j;
	/* ���� �������� ������ �µθ��� �׸���.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* next block�� �����ִ� ������ �µθ��� �׸���.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK");
	DrawBox(3, WIDTH + 10, 4, 8);
	DrawBox(10, WIDTH + 10, 4, 8);

	/* score�� �����ִ� ������ �µθ��� �׸���.*/
	move(17, WIDTH + 10);
	printw("SCORE");
	DrawBox(18, WIDTH + 10, 1, 8);
}

int GetCommand() {
	int command;
	command = wgetch(stdscr);
	switch (command) {
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret = 1;
	int drawFlag = 0;
	switch (command) {
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
			blockRotate = (blockRotate + 1) % 4;
		break;
	case KEY_DOWN:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
			blockX++;
		break;
	case KEY_LEFT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
			blockX--;
		break;
	default:
		break;
	}
	if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
	return ret;
}

void DrawField() {
	int i, j;
	for (j = 0; j < HEIGHT; j++) {
		move(j + 1, 1);
		for (i = 0; i < WIDTH; i++) {
			if (field[j][i] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score) {
	move(19, WIDTH + 11);
	printw("%8d", score);
}

void DrawNextBlock(int* nextBlock) {
	int i, j;
	for (i = 0; i < 4; i++) {
		move(4 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[1]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for (i = 0; i < 4; i++) {
		move(11 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
	int i, j;
	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j = 0; j < height; j++) {
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}
	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();
			newRank(score);
			
			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////ù���� �ǽ����� �����ؾ� �� �Լ�/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code

	// ��������: field�迭, int blockRotate, int blockX, int blockY
	// int currentBlock�� nextBlock[0] (���� block�� ���̵�)
	// block �迭 ����: block[NUM_OF_SHAPE][NUM_OF_ROTATE][HEIGHT][WIDTH]

	int yCoor, xCoor, count = 0; //block���� y��ǥ, x��ǥ

	for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++) { //y��
		for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++) //x��
			if (block[currentBlock][blockRotate][yCoor][xCoor]) {

				if (f[yCoor + blockY][xCoor + blockX])
					return 0;
				if (yCoor + blockY >= HEIGHT)
					return 0;
				if (xCoor + blockX < 0)
					return 0;
				if (xCoor + blockX >= WIDTH)
					return 0;
				count++;
			}
		if (count == 4) break;
	}

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code

	//1. ���� ��� ������ ã�´�. ProcessCommand�� switch���� ������ ��
	//2. ���� ��� ������ �����. DrawBlock�Լ� ������ ��.
	//3. ���ο� ��� ������ �׸���. 

	/* ProcessCommand�κ��� CheckToMove�� �� ���� �޴´�.
	�� ���ڰ����� �� �ܰ� ������ ��� ������ */

	int yCoor, xCoor, delX, delY, delRt, count = 0, yShd;
	delX = blockX; delY = blockY; delRt = blockRotate; yShd = blockY;

	switch (command) { //�� �ܰ� ���� �ܰ� ����� ��ǥ ���ϱ�
	case KEY_UP:
		delRt = (blockRotate + 3) % 4;
		break;
	case KEY_DOWN:
		delY--;
		break;
	case KEY_RIGHT:
		delX--;
		break;
	case KEY_LEFT:
		delX++;
		break;
	default:
		break;
	}

	for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++) { //�� �ܰ� ���� ����� ��ǥ�� ������� ����
		for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++)
			if (block[currentBlock][delRt][yCoor][xCoor] && (yCoor + delY >= 0)) {

				move(yCoor + delY + 1, xCoor + delX + 1);
				printw(".");
				count++;
			}
		if (count == 4) break;
	}
	move(HEIGHT, WIDTH + 10);

	while (CheckToMove(f, currentBlock, delRt, yShd++, delX)); //�׸��� �� ��ġ Ȯ��
	yShd -= 2;

	for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++)  //�׸��� �� �����
		for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++)
			if (block[currentBlock][delRt][yCoor][xCoor] && (yCoor + yShd >= 0)) {

				move(yCoor + yShd + 1, xCoor + delX + 1);
				printw(".");
			}

	move(HEIGHT, WIDTH + 10);

	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
	// user code
	int i;
	//�����ڷ� p26-27�� �÷ο���Ʈ�� �����Ѵ�.

	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {

		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, ++blockY, blockX);
		timed_out = 0;
		return;
	}

	if (blockY == -1) {
		gameOver = 1;
		AddBlockToField(field, nextBlock[0], blockRotate, blockX, blockY);
		return;
	}

	score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
	score += DeleteLine(field);
	DrawField();
	PrintScore(score);

	for (i = 0; i < BLOCK_NUM; i++) {
		if (i == BLOCK_NUM - 1)
			nextBlock[i] = rand() % 7;
		else
			nextBlock[i] = nextBlock[i + 1];
	}

	DrawNextBlock(nextBlock);

	recommend(root);

	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	timed_out = 0;

}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code

	//Block�� �߰��� ������ �ʵ尪�� �ٲ۴�.
	int yCoor, xCoor, touched, count;
	count = 0;
	touched = 0;

	for (yCoor = BLOCK_HEIGHT - 1; yCoor >= 0; yCoor--) {
		for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++) {
			if (block[currentBlock][blockRotate][yCoor][xCoor]) {
				if ((blockY + yCoor) == HEIGHT - 1)
					touched++;
				else if (f[blockY + yCoor + 1][blockX + xCoor] == 1)
					touched++;
				count++;
			}
		}
		if (count == 4) break;
	}

	for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++)
		for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++)
			if (block[currentBlock][blockRotate][yCoor][xCoor] && (blockY + yCoor) < HEIGHT && (blockX + xCoor) < WIDTH)
				f[blockY + yCoor][blockX + xCoor] = 1;

	return (touched * 10);
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	// user code

	//1. �ʵ带 Ž���Ͽ�, �� �� ������ �ִ��� Ž���Ѵ�.
	//2. �� �� ������ ������ �ش� ������ �����. ��, �ش� �������� �ʵ尪�� ��ĭ�� ������.

	int xCoor, yCoor, tmpY, scoreCount = 0;

	for (yCoor = HEIGHT - 1; yCoor >= 0; ) { //�ʵ��� ���� �Ʒ� �࿡�� ����, �� �྿ �ö�´�

		for (xCoor = 0; xCoor < WIDTH; xCoor++) //�Ȱ� �ִ� ���� ������ �� ���� ������ �� ���� ������ 
			if (!(f[yCoor][xCoor])) break;

		if (xCoor == WIDTH) { //�� ���� �� �� �ִ� ���, �� ���� ���־� �ϴ� ���

			scoreCount++; //���� ����

			for (xCoor = 0; xCoor < WIDTH; xCoor++) {//���� field ��� 0����

				tmpY = yCoor;
				f[tmpY][xCoor] = 0;
				tmpY--;

				for (; tmpY > 0; tmpY--) {
					if (f[tmpY][xCoor]) {
						f[tmpY + 1][xCoor] = 1;
						f[tmpY][xCoor] = 0;
					}
				}
			}
		}
		else yCoor--; //������ ���� �ٽ� �˻��ؾ� �Ѵ�  
	}

	return scoreCount * scoreCount * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {

	DrawRecommend(recommendY, recommendX, blockID, recommendR);
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	// user code

	int yShd;
	yShd = y;

	while (CheckToMove(field, nextBlock[0], blockRotate, yShd++, x));
	yShd -= 2;
	DrawBlock(yShd, x, blockID, blockRotate, '/');

}


void createRankList() {
	// user code

	ListInit(&list);

	int num, score, i;
	char name[NAMELEN];
	FILE* fp;

	fp = fopen("rank.txt", "r");
	if (fp == NULL) {
		printw("Rank File Error!!");
		return;
	}
	
	fscanf(fp, "%d", &num);

	for (i = 0; i < num; i++) {
		fscanf(fp, "%s%d", name, &score);
		ListInsert(&list, score, name);
	}

	fclose(fp);
}

void rank() {
	// user code

    int x, y, sel, delRank;
    char targetName[NAMELEN];

    do {
        clear();
        printw("1. list ranks from X to Y \n");
        printw("2. list ranks by a specific name \n");
        printw("3. delete a specific rank \n");
        sel = wgetch(stdscr);
        
    } while ('1' > sel || sel > '3');

    echo();

    if (sel == '1') {
	    printw("\nX: ");
	    scanw("%d", &x);
	    printw("Y: ");
	    scanw("%d", &y);
        
        if (x < 1 || x > list.count) x = 1;
        if (y < 1 || y > list.count) y = list.count;

        if (x > y)
            printw("\nsearch failure: no rank in the list \n");
        else {
            printw("\n       name      |   score \n");
            printw("--------------------------------- \n");
            printListRange(&list, x, y);
        }
    }
    else if (sel == '2') {
        
        printw("\ninput the name: ");
        scanw("%s", targetName);
        
        Node* tNode = list.first;
    
        int i, cnt;
        cnt = 0;

        printw("\n       name      |   score \n");
        printw("--------------------------------- \n");

        for (i = 0; i < list.count; i++){
            
            if (strcmp(targetName, tNode->name) == 0){
                printw(" %-16s|   %d \n", tNode->name, tNode->score);
                cnt++;
            }
            tNode = tNode->next;
        }
        
        if (cnt == 0) 
            printw("\nsearch failure: no name in the list \n");
    }
    else {
        
        printw("\ninput the rank: ");
        scanw("%d", &delRank);

        if (DeleteList(&list, delRank))
            printList(&list);
        else
            printw("\nsearch failure: the rank not in the list \n");
    }


    noecho();
    getch();
}

void writeRankFile() {
	// user code
    FILE* fp;
    
    Node* tNode = list.first;
    int i,  num;
    num = list.count;

    fp = fopen("rank.txt", "w");
    if (fp == NULL) {
        printw("Rank File Error!!");
        return;
    }
    
    fprintf(fp, "%d \n", num);
    for (i = 0; i < num; i++){
        fprintf(fp, "%s %d \n", tNode->name, tNode->score);    
        tNode = tNode->next;
    }

    fclose(fp);
}

void newRank(int score) {
	// user code

    char newName[NAMELEN];

    clear();
    echo();

    printw("your name: ");
    scanw("%s", newName);
    noecho();

    ListInsert(&list, score, newName);
    printList(&list);

    getch();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	// user code
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode* root) {

	int max = 0; // �̸� ���̴� ���� ��õ ��ġ���� ������� �� ���� �� �ִ� �ִ� ����
	int evalSize, nodeCnt = 0;
	int xRec = 0, yRec = 0, rRec = 0;
	time_t start, stop;
	double duration;

	root->level = root->accScore = root->x = root->y = root->r = 0;
	root->super = NULL;

	start = time(NULL);

	copyField(field, root->recField);

	recurRec(root, &max, &xRec, &yRec, &rRec, &nodeCnt);
	recommendX = xRec;
	recommendY = yRec;
	recommendR = rRec;

	stop = time(NULL);

	duration = (double)difftime(stop, start);
    root->totalTime += duration;
	evalSize = sizeof(RecNode) * nodeCnt;        
    
    move(25, 0);
    printw("Visible Block: %d \n", VISIBLE_BLOCKS);
   	printw("Number of Nodes created: %d \nSize of each Node: %d bytes \n", nodeCnt, sizeof(RecNode));
	printw("Evalsize = %d X %d = %d bytes \n", sizeof(RecNode), nodeCnt, evalSize);
	printw("time: %lf seconds \n", duration);
    printw("Accumulated time: %lf seconds \n", root->totalTime);

	return max;
}

void recurRec(RecNode* Troot, int* maxScore, int* xR, int* yR, int* rR, int* nodeCnt) {

	if (Troot->level >= VISIBLE_BLOCKS) return;

	int i, rotate, xCoor, yTop, yTemp, cIdx; 
    yTop = cIdx = 0;

	Troot->arr = (RecNode**)malloc(sizeof(RecNode*) * CHILDREN_MAX);
    for (i = 0; i < CHILDREN_MAX; i++)
        Troot->arr[i] = (RecNode*)malloc(sizeof(RecNode));

    (*nodeCnt) += CHILDREN_MAX;

	for (rotate = 0; rotate < 4; rotate++) {
		for (xCoor = -3; xCoor < WIDTH + 3; xCoor++) {

			yTemp = -1;

			while (CheckToMove(Troot->recField, nextBlock[Troot->level], rotate, yTemp++, xCoor));
			yTemp -= 2;

			if (yTop > yTemp) continue;

			if (yTop < yTemp) cIdx = 0;

			yTop = yTemp;

			Troot->arr[cIdx]->accScore = Troot->accScore;
			Troot->arr[cIdx]->level = (Troot->level + 1);
			Troot->arr[cIdx]->super = Troot;

			copyField(Troot->recField, (Troot->arr[cIdx])->recField);

			Troot->arr[cIdx]->accScore += AddBlockToField(Troot->arr[cIdx]->recField, nextBlock[Troot->level], rotate, yTemp, xCoor);
			Troot->arr[cIdx]->accScore += DeleteLine(Troot->arr[cIdx]->recField);
			Troot->arr[cIdx]->x = xCoor;
			Troot->arr[cIdx]->y = yTemp;
			Troot->arr[cIdx]->r = rotate;
			cIdx++;
		}
	}

	for (i = 0; i < cIdx; i++) {
		recurRec(Troot->arr[i], maxScore, xR, yR, rR, nodeCnt);
		if (*maxScore < Troot->arr[i]->accScore) {
			*maxScore = Troot->arr[i]->accScore;
			RecNode* targetNode = findLevelOne(Troot->arr[i]);
			*xR = targetNode->x;
			*yR = targetNode->y;
			*rR = targetNode->r;
		}
	}
}

RecNode* findLevelOne(RecNode* Node) {

	RecNode* tempNode = Node;

	while (1) {

		if (tempNode->level == 1) break;
		tempNode = tempNode->super;
	}
	return tempNode;
}

void copyField(char srcField[HEIGHT][WIDTH], char destField[HEIGHT][WIDTH]) {

	int i, j;

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			if (srcField[i][j] == 1)
				destField[i][j] = 1;
			else
				destField[i][j] = 0;
		}
}

void recommendedPlay() {
	// user code
    
}
