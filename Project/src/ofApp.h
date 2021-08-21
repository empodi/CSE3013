
#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon

typedef struct node {	// 미로의 인접그래프를 그리기 위한 구조체
	int v;
	node* link;
} node;

typedef struct position {	// 탈출 경로를 그릴 때 사용하기 위한 구조체

	int x1;					// 탈출 경로를 선분 단위로 그린다. 선분의 시작점과 끝점 저장.
	int y1;
	int x2;
	int y2;
} pos;

typedef struct _record {	// 플레이어의 전적을 연결리스트로 저장하기 위한 구조체

	int winLose;			// 승패 여부
	int mazeW;				// 플레이한 미로의 너비
	int mazeH;				// 플레이한 미로의 높이
	int level;				// 플레이한 판의 레벨
	struct _record* link;
} record;

class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key); // Traps escape key if exit disabled
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		bool readFile(); 
		void freeMemory(); // 동적할당 해제
		bool DFS1(); // 컴퓨터의 경로 계산
		bool DFS2(); // 플레이어의 경로계산
		void dfsdraw(); // 컴퓨터의 경로 그리기

		void hintdraw(); // 플레이어의 경로 힌트 그리기

		void push(int vert); //스택 push
		int pop();  //스택 pop
		void freeStack(); //스택 메모리 해제

		void Reset(); // 한 판이 끝났을 때 완료된 판의 변수 및 메모리 초기화
		void insertRec(); // 한 판이 끝났을 때 현재 판의 기록을 연결리스트에 삽입
		void freeRec();	// Menu에서 Exit을 할 때 전적 연결리스트 메모리 해제

		int HEIGHT;				//미로의 높이
		int WIDTH;				 //미로의 너비
		char** input;			 //텍스트 파일의 모든 정보를 담는 이차원 배열이다.
		int* visited;				//방문여부를 저장할 포인터
		int width;					// 가로 기준 미로 방의 개수
		int height;					// 세로 기준 미로 방의 개수
		int total;					//미로칸의 전체 개수 (행 * 열)

		int isOpen;				//파일이 열렸는지를 판단하는 변수. 0이면 안 열렸고 1이면 열렸다.
		int ifWin;				// 승패 여부 저장
		int isLevel;			 // 레벨을 선택했는지 안 했는지 여부

		int page;				// 게임은 총 네 페이지로 구성된다. 현재 페이지 저장.
		int flow;				// 시간 흐름. update 함수로 1씩 늘어난다.
		int seq;				// 컴퓨터의 탈출 경로를 그릴 때 사용. 경로를 선분 단위로 그릴 때 몇 번째 선분까지 그려졌는지 저장하는 변수.
		double mov;				 // 컴퓨터의 탈출 경로가 그려지는 길이의 기준이 되는 변수. 일정하게 증가한다.
		double comSpeed;		// 컴퓨터의 탈출 경로가 그려지는 속도
		int hFlag;	
		int hTime;
		int hCnt;					// 위 세 변수는 힌트를 그릴 때 사용된다.

		node** graph;				//인접리스트
		int** route;				//미로와 같은 사이즈의 경로를 표시하는 이차원배열. 경로에 해당하면 1 아니면 0.
		node* top;					 //스택

		int myX;					// 플레이어의 x축 위치
		int myY;					// 플레이어의 y축 위치
		int myDest;					// 플레이어의 도착점
		int comDest;			 // 컴퓨터의 도착점
		int comIdx;				// 컴퓨터의 경로를 저장하는 배열의 인덱스 개수. 탈출경로를 선분단위로 나누었을 때 선분의 총 개수
		int myIdx;				 // 플레이어의 경로를 저장하는 배열의 인덱스 개수. 탈출경로를 선분단위로 나누었을 때 선분의 총 개수
		pos* comTrack;			// 컴퓨터의 경로를 저장하는 배열
		pos* myTrack;			// 플레이어의 경로를 저장하는 배열
		record* rec;			// 전적 기록을 위한 연결리스트의 헤더

		// Menu
		ofxWinMenu * menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		// Used by example app
		ofTrueTypeFont myFont;
		ofTrueTypeFont recFont;
        ofImage myImage;
		float windowWidth, windowHeight;
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;

		// Example functions
 		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);
};
