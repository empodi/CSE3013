#include "ofApp.h"
#include <iostream>
#define SPACE 7	//미로 한 칸의 너비. draw에 사용. 임의로 변경 가능.
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	mov = 0;
	seq = 0;
	isOpen = 0;
	ifWin = 0;
	hFlag = 0;
	hTime = 0;
	hCnt = 0;
	flow = 0;		
	isLevel = 0;
	comSpeed = 0;
	page = 0;				// 시작할 때 모든 변수를 0으로 초기화한다.
	rec = NULL;				// 전적을 저장하는 연결리스트의 헤더를 NULL로 초기화 한다.

	ofSetWindowTitle("Maze Game"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);
	recFont.loadFont("verdana.ttf", 20, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	// Create a menu using ofxWinMenu

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	// Create a "File" popup menu
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	// Add popup items to the File menu

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	// View popup menu
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	// Help popup menu
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup

// Menu function

// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		freeRec();  // 전적 기록을 저장하는 연결리스트의 메모리 해제
		ofExit(); // Quit the application
	}
	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}
	// Help menu
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction
//--------------------------------------------------------------
void ofApp::update() {
	
	if (hFlag > 0) hTime++;				// 플레이어가 h를 누르면 hFlag가 1로 바뀌고 힌트를 볼 수 있게 된다. 

	if (hTime > 8) {					// 힌트는 update 함수가 8번 호출될 동안 보여진다. 
		hFlag = 0;						// 8번 이상 호출되면 다시 hFlag가 0으로 초기화 된다. 
		hTime = 0;
	}

	flow++;								// 기본 시간이 계속 업데이트 된다.
}
//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	int i, j, x, y, cnt;
	record* tNode;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here

	if (page == 0) {			// 첫번째 페이지. 게임에 대한 기본 설명과 .maz 파일을 고를 것을 명시한다. 
		ofSetColor(100);
		sprintf(str, "Open .maz file to play  \nPress h for hint  \nPress w to withdraw");
		recFont.drawString(str, 100, 100);
	}

	if (page == 1) {			// 두번째 페이지. 레벨을 고른다.  

		ofSetColor(100);
		sprintf(str, "Select Level (1 ~ 3) to play");
		recFont.drawString(str, 100, 100);

		if (!isLevel) return;   // 현재 줄의 아래 부분은 속도가 select된 후에 출력될 수 있도록 한다.

		sprintf(str, "LEVEL ");
		recFont.drawString(str, 100, 150);

		if (isLevel == 1) sprintf(str, "1 ");		// 레벨이 높을수록 컴퓨터의 경로가 그려지는 속도가 빠르다.
		if (isLevel == 2) sprintf(str, "2 ");
		if (isLevel == 3) sprintf(str, "3 ");

		ofSetColor(0, 128, 255);
		recFont.drawString(str, 210, 150);

		ofSetColor(100);
		sprintf(str, "Selected");
		recFont.drawString(str, 250, 150);

		sprintf(str, "Press s or S to start");		// 레벨을 고른 후 s 또는 S를 누르면 게임이 시작된다. 
		recFont.drawString(str, 100, 200);
	}

	if (page == 2) {					// 세번째 페이지

		ofSetColor(100);				// 미로의 기본 틀을 그린다. 
		ofSetLineWidth(3);
		ofDrawLine(0, 0, (WIDTH - 1) * SPACE, 0);
		ofDrawLine(0, 0, 0, (HEIGHT - 1) * SPACE);
		ofDrawLine(0, (HEIGHT - 1) * SPACE, (WIDTH - 1) * SPACE, (HEIGHT - 1) * SPACE);
		ofDrawLine((WIDTH - 1) * SPACE, 0, (WIDTH - 1) * SPACE, (HEIGHT - 1) * SPACE);

		for (i = 1; i < HEIGHT - 1; i++)
			for (j = 1; j < WIDTH - 1; j++)
				if (input[i][j] == '|')
					ofDrawLine(j * SPACE, (i - 1) * SPACE, j * SPACE, (i + 1) * SPACE);
				else if (input[i][j] == '-')
					ofDrawLine((j - 1) * SPACE, i * SPACE, (j + 1) * SPACE, i * SPACE);

		ofSetColor(0, 128, 255);
		ofDrawCircle(SPACE * (myX * 2 + 1), SPACE * (myY * 2 + 1), SPACE - 2); // 플레이어의 위치를 그린다. 
		dfsdraw(); // 컴퓨터의 탈출 경로를 그린다. 

		if(hFlag)			// 힌트를 그린다. 
			hintdraw();

		if (flow % 20 > 5) {			// 플레이어와 컴퓨터의 탈출 지점을 표시한다. 깜빡이도록 숫자를 임의로 설정하였다. 
						
			ofSetColor(0, 128, 255);
			ofDrawCircle(SPACE * ((myDest % width) * 2 + 1), SPACE * ((myDest / width) * 2 + 1), SPACE - 2);
			ofSetColor(255, 100, 102);
			ofDrawCircle(SPACE * ((comDest % width) * 2 + 1), SPACE * ((comDest / width) * 2 + 1), SPACE - 2);
		}
	}

	if (page == 3) {		// 마지막 페이지

		if (ifWin) {					// 승
			ofSetColor(0, 128, 255);
			sprintf(str, "You WON!!");
			recFont.drawString(str, 100, 100);
		}			
		else {							// 패
			ofSetColor(255, 100, 102);
			sprintf(str, "You LOST...");
			recFont.drawString(str, 100, 100);
		}

		cnt = 1;
		tNode = rec;

		ofSetColor(100);
		sprintf(str, "   Result              Maze Size           Level");
		myFont.drawString(str, 250, 170);

		for (; tNode; tNode = tNode->link) {		// 전적기록을 저장하는 연결리스트의 정보를 출력한다. 

			ofSetColor(100);
			sprintf(str, "Round %2d: ", cnt);
			recFont.drawString(str, 100, 150 + 50 * cnt);

			if (tNode->winLose) {
				ofSetColor(0, 128, 255);
				sprintf(str, " WIN");
				recFont.drawString(str, 250, 150 + 50 * cnt);
			}
			else {
				ofSetColor(255, 100, 102);
				sprintf(str, "LOSE");
				recFont.drawString(str, 250, 150 + 50 * cnt);
			}

			ofSetColor(100);
			sprintf(str, "   %4d   X %4d    %d \n", tNode->mazeH, tNode->mazeW, tNode->level);
			recFont.drawString(str, 320, 150 + 50 * cnt);
			cnt++;
		}

		ofSetColor(100);
		sprintf(str, "Select .maz file to play again");			// 다시 플레이하려면 파일을 고르면 된다. 
		recFont.drawString(str, 100, 170 + 50 * cnt);
	}
	
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(100);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw

void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen
void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	if (page == 1) {	// 두 번째 페이지인 경우

		if (key == '1') {	// 레벨 1을 고른 경우
			isLevel = 1;
			comSpeed = 0.5;
		}
		if (key == '2') {	// 레벨 2를 고른 경우
			isLevel = 2;
			comSpeed = 1.5;
		}
		if (key == '3') {	// 레벨 3을 고른 경우
			isLevel = 3;
			comSpeed = 3;
		}

		if ((key == 's' || key == 'S') && isLevel) {	// 레벨을 고른 후 s를 누르면 게임을 시작할 수 있다. 
			if (isOpen) 
				if (DFS1() && DFS2())	// 컴퓨터의 경로와 플레이어의 경로 탐색이 완료되면
					page = 2;			// 게임을 하는 페이지로 넘어간다. 
		}
	}

	if (page == 2) {									// 세번째 페이지 (직접 플레이)

		node* curNode, * tNode;

		if (key == OF_KEY_RIGHT) {						// 오른쪽 이동

			if (myX == width - 1) return;				// 범위 필터링

			curNode = graph[myY * width + myX];
			tNode = graph[myY * width + myX + 1];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {			// 이동하려는 정점이 현재 정점과 인접한지 확인
					myX++;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (key == OF_KEY_UP) {		// 위쪽 이동

			if (myY == 0) return;	// 범위 필터링

			curNode = graph[myY * width + myX];
			tNode = graph[(myY - 1) * width + myX];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {	// 이동하려는 정점이 현재 정점과 인접한지 확인
					myY--;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (key == OF_KEY_LEFT) {		// 왼쪽 이동

			if (myX == 0) return;		// 범위 필터링

			curNode = graph[myY * width + myX];
			tNode = graph[myY * width + myX - 1];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {	// 이동하려는 정점이 현재 정점과 인접한지 확인
					myX--;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (key == OF_KEY_DOWN) {		// 아래 이동

			if (myY >= height - 1) return;		// 범위 필터링

			curNode = graph[myY * width + myX];
			tNode = graph[(myY + 1) * width + myX];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {	// 이동하려는 정점이 현재 정점과 인접한지 확인
					myY++;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (myY * width + myX == myDest) {				// 플레이어의 위치가 탈출 지점에 도착한 경우에는
			ifWin = 1;									// ifWin 변수를 1로 설정
			Reset();									// Reset 함수 호출
		}

		if ((key == 'h' || key == 'H')) {		// 힌트

			if (hCnt < 3) {						// 힌트는 한 판당 세번만 볼 수 있다.
				hFlag = 1;
				hCnt++;
			}
		}

		if (key == 'w' || key == 'W') {					// w를 누르면 기권
			ifWin = 0;
			Reset();
		}
	}

} // end keyPressed
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				page = isOpen = 1;
			}

			ofBuffer buffer(file);

			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			int i, j, vert1, vert2;  
			HEIGHT = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {		
				string line = *it;
				HEIGHT++;
				WIDTH = line.size();
			}	//HEIGHT, WIDTH 구하기

			input = (char**)malloc(sizeof(char*) * HEIGHT);		//HEIGHT, WIDTH 만큼 동적할당
			for (i = 0; i < HEIGHT; i++) 
				input[i] = (char*)malloc(sizeof(char) * WIDTH);

			i = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, i++) {
				string line = *it; 
				for (j = 0; j < WIDTH; j++) 
					input[i][j] = line[j];
			}	//input에 maze.maz 삽입
			
			width = WIDTH / 2;
			height = HEIGHT / 2;	// maze의 실제 row, col
			total = width * height; // maze 방 개수
			comDest = total - 1;	// 컴퓨터의 탈출지점
			myDest = width - 1;		// 플레이어의 탈출 지점

			route = (int**)malloc(sizeof(int*) * height);					// 경로 정보를 저장하는 route배열 동적할당
			for (i = 0; i < height; i++)
				route[i] = (int*)malloc(sizeof(int) * width);

			for (i = 0; i < total; i++) {									 // graph 인접리스트, visited 초기화
				if (i == 0) {
					graph = (node**)malloc(sizeof(node*) * total);
					visited = (int*)malloc(sizeof(int) * total);
				}
				graph[i] = (node*)malloc(sizeof(node));
				graph[i]->v = i;
				graph[i]->link = NULL;
				visited[i] = 0;
			}

			node* tNode1, *tNode2, *newNode1, *newNode2;
			for (i = 1; i < HEIGHT; i++) {  //인접 리스트 시작
				j = 1;
				if (i % 2 == 1) j = 2;

				for ( ; j < WIDTH - 1; j += 2)	// input 배열에서 벽의 위치에 해당하는 인덱스만 확인
					if (input[i][j] == ' ') {	// 벽의 위치에 해당하는 인덱스가 빈칸인 경우 -> 길이 뚫린 경우 -> 인접리스트 추가

						vert1 = (i / 2 - 1) * width + j / 2;
						vert2 = vert1 + width;					// i가 짝수

						if (i % 2 == 1) { // i가 홀수이면 변경
							vert1 = (i / 2) * width + j / 2 - 1;
							vert2 = vert1 + 1;
						}

						tNode1 = graph[vert1]; tNode2 = graph[vert2];

						while (tNode1->link != NULL || tNode2->link != NULL) {
							if (tNode1->link != NULL) tNode1 = tNode1->link;
							if (tNode2->link != NULL) tNode2 = tNode2->link;
						}
						newNode1 = (node*)malloc(sizeof(node));
						newNode2 = (node*)malloc(sizeof(node));
						newNode1->link = newNode2->link = NULL;
						newNode1->v = vert1;
						newNode2->v = vert2;
						tNode1->link = newNode2;;
						tNode2->link = newNode1;			// 인접리스트 연결
					}
			} //인접 리스트 for 끝
		
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}

void ofApp::freeMemory() {

	int i, j;
	node* freenode;

	for (i = 0; i < total; i++) {				// 인접리스트 메모리 해제
		
		while (graph[i]) {
			freenode = graph[i];
			graph[i] = graph[i]->link;
			free(freenode);
		}
	}
	free(graph);

	free(comTrack);
	free(myTrack);
	free(visited);		

	for (i = 0; i < HEIGHT; i++)
		free(input[i]);
	free(input);
	
	for (i = 0; i < height; i++) 
		free(route[i]);
	free(route);					// 동적할당한 배열들 모두 해제
	
	return;
}

void ofApp::freeStack() {		// DFS용 스택 메모리 해제
	node* freenode;
	while (top) {
		freenode = top;
		top = top->link;
		free(freenode);
	}

	return;
}

bool ofApp::DFS1()		// 컴퓨터의 탈출 경로 구하기
{
	int i, j, cnt, pNum, idx;
	node* tNode;
	top = NULL;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) 
			route[i][j] = visited[i * width + j] = 0;		// route, visited 배열 초기화
		
	visited[0] = route[0][0] = 1;		// 경로 탐색 시작 (0행 0열에서 시작)
	push(0);

	while (top) {

		if (top->v == comDest) {								// 탈출 지점 찾은 경우
			freeStack();											// 스택 메모리 해제
			cnt = 0;
			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++)
					if (route[i][j] == 1)						// route 배열에서 value가 1인 인덱스 개수 = 경로가 지나온 정점의 개수
						cnt++;
		
			comIdx = cnt - 1;									// route 배열에서 value가 1인 인덱스 개수 = 경로가 지나온 정점의 개수를 comIdx 변수에 저장
			comTrack = (pos*)malloc(sizeof(pos) * comIdx);		// comIdx 변수만큼 comTrack 배열 동적 할당
			
			i = 0; j = 0; idx = 0;								// 다시 처음부터 경로 추적 시작
			
			while (1) {											// 경로를 route 배열을 이용해 추적하면서 comTrack 배열에 저장
				tNode = graph[i * width + j];
				route[i][j] = 2;
				comTrack[idx].x1 = i; 
				comTrack[idx].y1 = j;

				while (tNode != NULL) {
					i = tNode->v / width;
					j = tNode->v % width;
					if (route[i][j] == 1) {
						comTrack[idx].x2 = i; 
						comTrack[idx].y2 = j;
						route[i][j] = 2;
						break;
					}
					tNode = tNode->link;
				}
				idx++;
				if (idx == comIdx) break;
			}
			return true;									// 탈출 경로를 찾았으므로 true 리턴
		}

		tNode = graph[top->v];		
		cnt = 0;
		while (tNode) {							// 기본적인 DFS
			if (!visited[tNode->v]) {
				visited[tNode->v] = 1;
				push(tNode->v);
				i = tNode->v / width;
				j = tNode->v % width;
				route[i][j] = 1;
				cnt++;
				break;
			}
			tNode = tNode->link;
		}
		if (cnt == 0) {
			pNum = pop();
			i = pNum / width;
			j = pNum % width;
			route[i][j] = 0;
		}
	}

	freeStack();
	return false;
}

bool ofApp::DFS2()  // 플레이어의 탈출 경로 계산. DFS1과 기본 형식은 동일하다. 플레이어의 탈출 경로는 힌트를 그리는데 사용. 
{
	int i, j, cnt, pNum, idx;
	node* tNode;
	top = NULL;

	myX = 0;			
	myY = height - 1;									// myX와 myY의 초기값은 플레이어의 탈출 지점의 행과 열 번호이다. 

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) 
			route[i][j] = visited[i * width + j] = 0;		// route, visited 배열 초기화

	visited[myY * width + myX] = route[myY][0] = 1;			// 플레이어의 경로 시작점은 항상 왼쪽 가장 아래 칸이다.
	push(myY * width + myX);								// 그에 해당하는 인덱스 번호 push

	while (top) {

		if (top->v == myDest) {								// 플레이어의 탈출 경로 찾은 경우
			freeStack();								// DFS용 스택 메모리 해제

			cnt = 0;

			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++)
					if (route[i][j] == 1)				// route 배열에서 value가 1인 인덱스 개수 = 경로가 지나온 정점의 개수
						cnt++;				

			myIdx = cnt - 1;								// route 배열에서 value가 1인 인덱스 개수 = 경로가 지나온 정점의 개수를 myIdx 변수에 저장
			myTrack = (pos*)malloc(sizeof(pos) * myIdx);	// myIdx 변수만큼 myTrack 배열 동적 할당

			i = myY; 
			j = myX; 
			idx = 0;		// 다시 처음부터 경로 추적 시작

			while (1) {		// 경로를 route 배열을 이용해 추적하면서 myTrack 배열에 저장

				tNode = graph[i * width + j];
				route[i][j] = 2;
				myTrack[idx].x1 = i;
				myTrack[idx].y1 = j;

				while (tNode != NULL) {
					i = tNode->v / width;
					j = tNode->v % width;

					if (route[i][j] == 1) {
						myTrack[idx].x2 = i; 
						myTrack[idx].y2 = j;
						route[i][j] = 2;
						break;
					}
					tNode = tNode->link;
				}
				idx++;

				if (idx == myIdx) break;	
			}

			return true;			// 탈출 경로를 찾았으므로 true 리턴
		}

		tNode = graph[top->v]; 
		cnt = 0;

		while (tNode) {		// 기본적인 DFS
			if (!visited[tNode->v]) {
				visited[tNode->v] = 1;
				push(tNode->v);
				i = tNode->v / width;
				j = tNode->v % width;
				route[i][j] = 1;
				cnt++;
				break;
			}
			tNode = tNode->link;
		}
		if (cnt == 0) {
			pNum = pop();
			i = pNum / width;
			j = pNum % width;
			route[i][j] = 0;
		}
	}

	freeStack();
	return false;
}

void ofApp::push(int vert) { //스택 push

	node* newNode = (node*)malloc(sizeof(node));
	newNode->v = vert;
	newNode->link = top;
	top = newNode;

	return;
}

int ofApp::pop() {	//스택 pop

	if (!top) {
		printf("Stack Error \n");
		return 0;
	}

	int ret;
	node* delNode = top;
	top = top->link;

	ret = delNode->v;

	return ret;
}

void ofApp::dfsdraw()		// 컴퓨터의 탈출 경로를 그린다. 
{	
	int i, j, s1, e1, s2, e2;
	node* tnode;

	if (seq >= comIdx) {		// seq 변수는 탈출 경로의 총 선분의 개수 중 그려진 선분의 개수를 나타낸다. 
								// 그려진 개수가 전체 개수 이상이 되면 컴퓨터가 먼저 탈출지점에 도착함을 의미. 컴퓨터 승.
		ifWin = 0;				// 플레이어가 패했으므로 ifWin 변수에 0 저장. 
		Reset();				// Reset 함수
		return;
	}

	ofSetColor(255, 100, 102); 	
	ofSetLineWidth(SPACE / 2);

	for (i = 0; i < seq; i++) {		// seq 변수의 크기만큼 우선 탈출경로를 그려준다. 현재까지 지나온 부분만 그리는 것.
		s1 = comTrack[i].x1;
		e1 = comTrack[i].y1;
		s2 = comTrack[i].x2;
		e2 = comTrack[i].y2;
		ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s2 * 2 + 1));
	}

	s1 = comTrack[seq].x1; e1 = comTrack[seq].y1;
	s2 = comTrack[seq].x2; e2 = comTrack[seq].y2;		// 현재 그려지고 있는 선분의 시작점과 끝점.

	if (s1 == s2) {			// 수평이동
		mov += comSpeed;	// mov 변수만큼 늘어나며 그려진다. comSpeed로 mov 변수가 늘어나는 정도를 조정한다. 
		if (e1 > e2) {		// 오른쪽에서 왼쪽으로 그려지는 경우
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e1 * 2 + 1) - mov, SPACE * (s2 * 2 + 1));
			if (SPACE * (e2 * 2 + 1) + mov >= SPACE * (e1 * 2 + 1)) {
				seq++; mov = 0;
			}
		}
		else {			// 왼쪽에서 오른쪽로 그려지는 경우
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e1 * 2 + 1) + mov, SPACE * (s2 * 2 + 1));
			if (SPACE * (e1 * 2 + 1) + mov >= SPACE * (e2 * 2 + 1)) {
				seq++; mov = 0;
			}
		}
	}
	else if (e1 == e2) {  // 수직이동
		mov += comSpeed;
		if (s1 > s2) {		// 아래에서 위로 그려지는 경우
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s1 * 2 + 1) - mov);
			if (SPACE * (s1 * 2 + 1) <= SPACE * (s2 * 2 + 1) + mov) {
				seq++; mov = 0;
			}
		}
		else {				// 위에서 아래로 그려지는 경우
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s1 * 2 + 1) + mov);
			if (SPACE * (s1 * 2 + 1) + mov >= SPACE * (s2 * 2 + 1)) {
				seq++; mov = 0;
			}
		}
	}
	return;
}

void ofApp::hintdraw() {

	int i, s1, e1, s2, e2;
	
	ofSetColor(178, 255, 102);

	for (i = 0; i < myIdx; i++) {		// 힌트를 그린다. 
		s1 = myTrack[i].x1;
		e1 = myTrack[i].y1;
		s2 = myTrack[i].x2;
		e2 = myTrack[i].y2;
		ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s2 * 2 + 1));
	}
}

void ofApp::Reset() {

	insertRec();		// 현재 판의 기록을 전적기록 연결리스트에 저장한다. 

	mov = 0;
	seq = 0;
	isOpen = 0;
	hCnt = 0;
	page = 3;
	isLevel = 0;		// 다음 판을 위해 변수들을 초기화한다. 

	freeMemory();		// 이번 판의 메모리를 해제한다. 

	return;
}

void ofApp::insertRec() {	// 전적 기록 연결리스트에 현재 라운드 정보 저장

	record* newNode = (record*)malloc(sizeof(record));	//현재 라운드의 정보를 저장한 새로운 구조체 동적할당
	newNode->link = NULL;
	newNode->winLose = ifWin;
	newNode->mazeW = width;
	newNode->mazeH = height;
	newNode->level = isLevel;

	if (rec == NULL) {					//연결리스트에 새 구조체 삽입
		rec = newNode;
	}
	else {
		record* tNode = rec;

		for (; tNode->link != NULL; tNode = tNode->link);
		tNode->link = newNode;
	}

	return;
}

void ofApp::freeRec() {		// 연결리스트 메모리 해제. 프로그램이 종료될 때 호출된다.

	record* tnode = rec;
	record* delnode;

	for (; tnode; ) {
		delnode = tnode;
		tnode = tnode->link;
		free(delnode);
	}
	return;
}
