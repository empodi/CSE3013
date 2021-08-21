
#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon

typedef struct node {	// �̷��� �����׷����� �׸��� ���� ����ü
	int v;
	node* link;
} node;

typedef struct position {	// Ż�� ��θ� �׸� �� ����ϱ� ���� ����ü

	int x1;					// Ż�� ��θ� ���� ������ �׸���. ������ �������� ���� ����.
	int y1;
	int x2;
	int y2;
} pos;

typedef struct _record {	// �÷��̾��� ������ ���Ḯ��Ʈ�� �����ϱ� ���� ����ü

	int winLose;			// ���� ����
	int mazeW;				// �÷����� �̷��� �ʺ�
	int mazeH;				// �÷����� �̷��� ����
	int level;				// �÷����� ���� ����
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
		void freeMemory(); // �����Ҵ� ����
		bool DFS1(); // ��ǻ���� ��� ���
		bool DFS2(); // �÷��̾��� ��ΰ��
		void dfsdraw(); // ��ǻ���� ��� �׸���

		void hintdraw(); // �÷��̾��� ��� ��Ʈ �׸���

		void push(int vert); //���� push
		int pop();  //���� pop
		void freeStack(); //���� �޸� ����

		void Reset(); // �� ���� ������ �� �Ϸ�� ���� ���� �� �޸� �ʱ�ȭ
		void insertRec(); // �� ���� ������ �� ���� ���� ����� ���Ḯ��Ʈ�� ����
		void freeRec();	// Menu���� Exit�� �� �� ���� ���Ḯ��Ʈ �޸� ����

		int HEIGHT;				//�̷��� ����
		int WIDTH;				 //�̷��� �ʺ�
		char** input;			 //�ؽ�Ʈ ������ ��� ������ ��� ������ �迭�̴�.
		int* visited;				//�湮���θ� ������ ������
		int width;					// ���� ���� �̷� ���� ����
		int height;					// ���� ���� �̷� ���� ����
		int total;					//�̷�ĭ�� ��ü ���� (�� * ��)

		int isOpen;				//������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �� ���Ȱ� 1�̸� ���ȴ�.
		int ifWin;				// ���� ���� ����
		int isLevel;			 // ������ �����ߴ��� �� �ߴ��� ����

		int page;				// ������ �� �� �������� �����ȴ�. ���� ������ ����.
		int flow;				// �ð� �帧. update �Լ��� 1�� �þ��.
		int seq;				// ��ǻ���� Ż�� ��θ� �׸� �� ���. ��θ� ���� ������ �׸� �� �� ��° ���б��� �׷������� �����ϴ� ����.
		double mov;				 // ��ǻ���� Ż�� ��ΰ� �׷����� ������ ������ �Ǵ� ����. �����ϰ� �����Ѵ�.
		double comSpeed;		// ��ǻ���� Ż�� ��ΰ� �׷����� �ӵ�
		int hFlag;	
		int hTime;
		int hCnt;					// �� �� ������ ��Ʈ�� �׸� �� ���ȴ�.

		node** graph;				//��������Ʈ
		int** route;				//�̷ο� ���� �������� ��θ� ǥ���ϴ� �������迭. ��ο� �ش��ϸ� 1 �ƴϸ� 0.
		node* top;					 //����

		int myX;					// �÷��̾��� x�� ��ġ
		int myY;					// �÷��̾��� y�� ��ġ
		int myDest;					// �÷��̾��� ������
		int comDest;			 // ��ǻ���� ������
		int comIdx;				// ��ǻ���� ��θ� �����ϴ� �迭�� �ε��� ����. Ż���θ� ���д����� �������� �� ������ �� ����
		int myIdx;				 // �÷��̾��� ��θ� �����ϴ� �迭�� �ε��� ����. Ż���θ� ���д����� �������� �� ������ �� ����
		pos* comTrack;			// ��ǻ���� ��θ� �����ϴ� �迭
		pos* myTrack;			// �÷��̾��� ��θ� �����ϴ� �迭
		record* rec;			// ���� ����� ���� ���Ḯ��Ʈ�� ���

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
