﻿#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
    nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
    DrawBox(10,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(17,WIDTH+10);
	printw("SCORE");
	DrawBox(18,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
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

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(19,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for( i = 0; i < 4; i++ ){
		move(11+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

    // 전역변수: field배열, int blockRotate, int blockX, int blockY
    // int currentBlock은 nextBlock[0] (현재 block의 아이디)
    // block 배열 형식: block[NUM_OF_SHAPE][NUM_OF_ROTATE][HEIGHT][WIDTH]
	
	int yCoor, xCoor, count = 0; //block내의 y좌표, x좌표

	for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++) { //y축
		for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++) //x축
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

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	
    /* ProcessCommand로부터 CheckToMove가 된 값을 받는다. 
	즉 인자값들은 한 단계 이후의 블록 변수들 */

	int yCoor, xCoor, delX, delY, delRt, count = 0, yShd;
	delX = blockX; delY = blockY; delRt = blockRotate; yShd = blockY;

	switch (command) { //한 단계 이전 단계 블록의 좌표 구하기
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

for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++) { //한 단계 이전 블록의 좌표를 기반으로 삭제
	for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++)
		if (block[currentBlock][delRt][yCoor][xCoor] && (yCoor + delY >= 0)) {

			move(yCoor + delY + 1, xCoor + delX + 1);
			printw(".");
			count++;
		}
	if (count == 4) break;
}
    move(HEIGHT, WIDTH + 10);

    while(CheckToMove(f, currentBlock, delRt, yShd++, delX)); //그림자 블럭 위치 확인
    yShd -= 2;
 
    for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++)  //그림자 블럭 지우기
        for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++)
            if (block[currentBlock][delRt][yCoor][xCoor] && (yCoor + yShd >= 0)){
                
                move(yCoor + yShd + 1, xCoor + delX + 1);
                printw(".");
            }

    move(HEIGHT, WIDTH + 10);

	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){
	// user code
	
	//강의자료 p26-27의 플로우차트를 참고한다.

	if(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {

        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, ++blockY, blockX);
        timed_out = 0;
        return;
	}

    if (blockY == -1) {
        gameOver = 1;
        AddBlockToField(field, nextBlock[0], blockRotate, blockX, blockY);
        return;
    }
    
    AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
    score += DeleteLine(field);
    PrintScore(score);

    nextBlock[0] = nextBlock[1];
    nextBlock[1] = nextBlock[2];
    nextBlock[2] = rand()%7;
    DrawNextBlock(nextBlock);

    blockRotate = 0;
    blockY = -1;
    blockX = WIDTH / 2 - 2;
    DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
    timed_out = 0;
    
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	
    //Block이 추가된 영역의 필드값을 바꾼다.
    int yCoor, xCoor, touched, count;
	count = 0;
    touched = 0;
    
    for (yCoor = BLOCK_HEIGHT - 1; yCoor >= 0; yCoor--) {
        for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++) {
            if (block[currentBlock][blockRotate][yCoor][xCoor]){
                if ((blockY + yCoor) == HEIGHT - 1)
                    touched++;    
                else if (f[blockY + yCoor + 1][blockX + xCoor] == 1)
                    touched++;
				count++;
            }
        }
		if (count == 4) break;
    }

    score += touched * 10;
    PrintScore(score);

    for (yCoor = 0; yCoor < BLOCK_HEIGHT; yCoor++)
        for (xCoor = 0; xCoor < BLOCK_WIDTH; xCoor++)
            if(block[currentBlock][blockRotate][yCoor][xCoor] && (blockY + yCoor) < HEIGHT && (blockX + xCoor) < WIDTH)
                f[blockY + yCoor][blockX + xCoor] = 1;

	DrawField();
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.

	int xCoor, yCoor, tmpY, scoreCount = 0;

	for (yCoor = HEIGHT - 1; yCoor >= 0; ) { //필드의 가장 아래 행에서 시작, 한 행씩 올라온다

		for (xCoor = 0; xCoor < WIDTH; xCoor++) //훑고 있는 행의 열에서 빈 곳이 있으면 그 다음 행으로 
			if (!(f[yCoor][xCoor])) break;

		if (xCoor == WIDTH) { //한 행이 다 차 있는 경우, 즉 줄을 없애야 하는 경우

			scoreCount++; //점수 증가

			for (xCoor = 0; xCoor < WIDTH; xCoor++) {//행의 field 모두 0으로

				tmpY = yCoor;
				f[tmpY][xCoor] = 0;

				while (f[--tmpY][xCoor]) {
					f[tmpY + 1][xCoor] = 1;
					f[tmpY][xCoor] = 0;
				}
			}
		}
		else yCoor--; //삭제한 행은 다시 검사해야 한다  
	}

    DrawField();
    return scoreCount * scoreCount * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
    
    DrawBlock(y, x, blockID, blockRotate, ' ');
    DrawShadow(y, x, blockID, blockRotate);
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
    
    int yShd;
    yShd = y;

    while(CheckToMove(field, nextBlock[0], blockRotate, yShd++, x));
    yShd -= 2;
    DrawBlock(yShd, x, blockID, blockRotate, '/');

}


void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
