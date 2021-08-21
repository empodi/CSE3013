#include "ofApp.h"
#include <iostream>
#define SPACE 7	//�̷� �� ĭ�� �ʺ�. draw�� ���. ���Ƿ� ���� ����.
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
	page = 0;				// ������ �� ��� ������ 0���� �ʱ�ȭ�Ѵ�.
	rec = NULL;				// ������ �����ϴ� ���Ḯ��Ʈ�� ����� NULL�� �ʱ�ȭ �Ѵ�.

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
		freeRec();  // ���� ����� �����ϴ� ���Ḯ��Ʈ�� �޸� ����
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
	
	if (hFlag > 0) hTime++;				// �÷��̾ h�� ������ hFlag�� 1�� �ٲ�� ��Ʈ�� �� �� �ְ� �ȴ�. 

	if (hTime > 8) {					// ��Ʈ�� update �Լ��� 8�� ȣ��� ���� ��������. 
		hFlag = 0;						// 8�� �̻� ȣ��Ǹ� �ٽ� hFlag�� 0���� �ʱ�ȭ �ȴ�. 
		hTime = 0;
	}

	flow++;								// �⺻ �ð��� ��� ������Ʈ �ȴ�.
}
//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	int i, j, x, y, cnt;
	record* tNode;
	
	// TO DO : DRAW MAZE; 
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here

	if (page == 0) {			// ù��° ������. ���ӿ� ���� �⺻ ����� .maz ������ �� ���� ����Ѵ�. 
		ofSetColor(100);
		sprintf(str, "Open .maz file to play  \nPress h for hint  \nPress w to withdraw");
		recFont.drawString(str, 100, 100);
	}

	if (page == 1) {			// �ι�° ������. ������ ����.  

		ofSetColor(100);
		sprintf(str, "Select Level (1 ~ 3) to play");
		recFont.drawString(str, 100, 100);

		if (!isLevel) return;   // ���� ���� �Ʒ� �κ��� �ӵ��� select�� �Ŀ� ��µ� �� �ֵ��� �Ѵ�.

		sprintf(str, "LEVEL ");
		recFont.drawString(str, 100, 150);

		if (isLevel == 1) sprintf(str, "1 ");		// ������ �������� ��ǻ���� ��ΰ� �׷����� �ӵ��� ������.
		if (isLevel == 2) sprintf(str, "2 ");
		if (isLevel == 3) sprintf(str, "3 ");

		ofSetColor(0, 128, 255);
		recFont.drawString(str, 210, 150);

		ofSetColor(100);
		sprintf(str, "Selected");
		recFont.drawString(str, 250, 150);

		sprintf(str, "Press s or S to start");		// ������ �� �� s �Ǵ� S�� ������ ������ ���۵ȴ�. 
		recFont.drawString(str, 100, 200);
	}

	if (page == 2) {					// ����° ������

		ofSetColor(100);				// �̷��� �⺻ Ʋ�� �׸���. 
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
		ofDrawCircle(SPACE * (myX * 2 + 1), SPACE * (myY * 2 + 1), SPACE - 2); // �÷��̾��� ��ġ�� �׸���. 
		dfsdraw(); // ��ǻ���� Ż�� ��θ� �׸���. 

		if(hFlag)			// ��Ʈ�� �׸���. 
			hintdraw();

		if (flow % 20 > 5) {			// �÷��̾�� ��ǻ���� Ż�� ������ ǥ���Ѵ�. �����̵��� ���ڸ� ���Ƿ� �����Ͽ���. 
						
			ofSetColor(0, 128, 255);
			ofDrawCircle(SPACE * ((myDest % width) * 2 + 1), SPACE * ((myDest / width) * 2 + 1), SPACE - 2);
			ofSetColor(255, 100, 102);
			ofDrawCircle(SPACE * ((comDest % width) * 2 + 1), SPACE * ((comDest / width) * 2 + 1), SPACE - 2);
		}
	}

	if (page == 3) {		// ������ ������

		if (ifWin) {					// ��
			ofSetColor(0, 128, 255);
			sprintf(str, "You WON!!");
			recFont.drawString(str, 100, 100);
		}			
		else {							// ��
			ofSetColor(255, 100, 102);
			sprintf(str, "You LOST...");
			recFont.drawString(str, 100, 100);
		}

		cnt = 1;
		tNode = rec;

		ofSetColor(100);
		sprintf(str, "   Result              Maze Size           Level");
		myFont.drawString(str, 250, 170);

		for (; tNode; tNode = tNode->link) {		// ��������� �����ϴ� ���Ḯ��Ʈ�� ������ ����Ѵ�. 

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
		sprintf(str, "Select .maz file to play again");			// �ٽ� �÷����Ϸ��� ������ ���� �ȴ�. 
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

	if (page == 1) {	// �� ��° �������� ���

		if (key == '1') {	// ���� 1�� �� ���
			isLevel = 1;
			comSpeed = 0.5;
		}
		if (key == '2') {	// ���� 2�� �� ���
			isLevel = 2;
			comSpeed = 1.5;
		}
		if (key == '3') {	// ���� 3�� �� ���
			isLevel = 3;
			comSpeed = 3;
		}

		if ((key == 's' || key == 'S') && isLevel) {	// ������ �� �� s�� ������ ������ ������ �� �ִ�. 
			if (isOpen) 
				if (DFS1() && DFS2())	// ��ǻ���� ��ο� �÷��̾��� ��� Ž���� �Ϸ�Ǹ�
					page = 2;			// ������ �ϴ� �������� �Ѿ��. 
		}
	}

	if (page == 2) {									// ����° ������ (���� �÷���)

		node* curNode, * tNode;

		if (key == OF_KEY_RIGHT) {						// ������ �̵�

			if (myX == width - 1) return;				// ���� ���͸�

			curNode = graph[myY * width + myX];
			tNode = graph[myY * width + myX + 1];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {			// �̵��Ϸ��� ������ ���� ������ �������� Ȯ��
					myX++;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (key == OF_KEY_UP) {		// ���� �̵�

			if (myY == 0) return;	// ���� ���͸�

			curNode = graph[myY * width + myX];
			tNode = graph[(myY - 1) * width + myX];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {	// �̵��Ϸ��� ������ ���� ������ �������� Ȯ��
					myY--;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (key == OF_KEY_LEFT) {		// ���� �̵�

			if (myX == 0) return;		// ���� ���͸�

			curNode = graph[myY * width + myX];
			tNode = graph[myY * width + myX - 1];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {	// �̵��Ϸ��� ������ ���� ������ �������� Ȯ��
					myX--;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (key == OF_KEY_DOWN) {		// �Ʒ� �̵�

			if (myY >= height - 1) return;		// ���� ���͸�

			curNode = graph[myY * width + myX];
			tNode = graph[(myY + 1) * width + myX];

			while (curNode != NULL) {

				if (curNode->v == tNode->v) {	// �̵��Ϸ��� ������ ���� ������ �������� Ȯ��
					myY++;
					break;
				}
				curNode = curNode->link;
			}
		}

		if (myY * width + myX == myDest) {				// �÷��̾��� ��ġ�� Ż�� ������ ������ ��쿡��
			ifWin = 1;									// ifWin ������ 1�� ����
			Reset();									// Reset �Լ� ȣ��
		}

		if ((key == 'h' || key == 'H')) {		// ��Ʈ

			if (hCnt < 3) {						// ��Ʈ�� �� �Ǵ� ������ �� �� �ִ�.
				hFlag = 1;
				hCnt++;
			}
		}

		if (key == 'w' || key == 'W') {					// w�� ������ ���
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
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			int i, j, vert1, vert2;  
			HEIGHT = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {		
				string line = *it;
				HEIGHT++;
				WIDTH = line.size();
			}	//HEIGHT, WIDTH ���ϱ�

			input = (char**)malloc(sizeof(char*) * HEIGHT);		//HEIGHT, WIDTH ��ŭ �����Ҵ�
			for (i = 0; i < HEIGHT; i++) 
				input[i] = (char*)malloc(sizeof(char) * WIDTH);

			i = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, i++) {
				string line = *it; 
				for (j = 0; j < WIDTH; j++) 
					input[i][j] = line[j];
			}	//input�� maze.maz ����
			
			width = WIDTH / 2;
			height = HEIGHT / 2;	// maze�� ���� row, col
			total = width * height; // maze �� ����
			comDest = total - 1;	// ��ǻ���� Ż������
			myDest = width - 1;		// �÷��̾��� Ż�� ����

			route = (int**)malloc(sizeof(int*) * height);					// ��� ������ �����ϴ� route�迭 �����Ҵ�
			for (i = 0; i < height; i++)
				route[i] = (int*)malloc(sizeof(int) * width);

			for (i = 0; i < total; i++) {									 // graph ��������Ʈ, visited �ʱ�ȭ
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
			for (i = 1; i < HEIGHT; i++) {  //���� ����Ʈ ����
				j = 1;
				if (i % 2 == 1) j = 2;

				for ( ; j < WIDTH - 1; j += 2)	// input �迭���� ���� ��ġ�� �ش��ϴ� �ε����� Ȯ��
					if (input[i][j] == ' ') {	// ���� ��ġ�� �ش��ϴ� �ε����� ��ĭ�� ��� -> ���� �ո� ��� -> ��������Ʈ �߰�

						vert1 = (i / 2 - 1) * width + j / 2;
						vert2 = vert1 + width;					// i�� ¦��

						if (i % 2 == 1) { // i�� Ȧ���̸� ����
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
						tNode2->link = newNode1;			// ��������Ʈ ����
					}
			} //���� ����Ʈ for ��
		
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

	for (i = 0; i < total; i++) {				// ��������Ʈ �޸� ����
		
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
	free(route);					// �����Ҵ��� �迭�� ��� ����
	
	return;
}

void ofApp::freeStack() {		// DFS�� ���� �޸� ����
	node* freenode;
	while (top) {
		freenode = top;
		top = top->link;
		free(freenode);
	}

	return;
}

bool ofApp::DFS1()		// ��ǻ���� Ż�� ��� ���ϱ�
{
	int i, j, cnt, pNum, idx;
	node* tNode;
	top = NULL;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) 
			route[i][j] = visited[i * width + j] = 0;		// route, visited �迭 �ʱ�ȭ
		
	visited[0] = route[0][0] = 1;		// ��� Ž�� ���� (0�� 0������ ����)
	push(0);

	while (top) {

		if (top->v == comDest) {								// Ż�� ���� ã�� ���
			freeStack();											// ���� �޸� ����
			cnt = 0;
			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++)
					if (route[i][j] == 1)						// route �迭���� value�� 1�� �ε��� ���� = ��ΰ� ������ ������ ����
						cnt++;
		
			comIdx = cnt - 1;									// route �迭���� value�� 1�� �ε��� ���� = ��ΰ� ������ ������ ������ comIdx ������ ����
			comTrack = (pos*)malloc(sizeof(pos) * comIdx);		// comIdx ������ŭ comTrack �迭 ���� �Ҵ�
			
			i = 0; j = 0; idx = 0;								// �ٽ� ó������ ��� ���� ����
			
			while (1) {											// ��θ� route �迭�� �̿��� �����ϸ鼭 comTrack �迭�� ����
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
			return true;									// Ż�� ��θ� ã�����Ƿ� true ����
		}

		tNode = graph[top->v];		
		cnt = 0;
		while (tNode) {							// �⺻���� DFS
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

bool ofApp::DFS2()  // �÷��̾��� Ż�� ��� ���. DFS1�� �⺻ ������ �����ϴ�. �÷��̾��� Ż�� ��δ� ��Ʈ�� �׸��µ� ���. 
{
	int i, j, cnt, pNum, idx;
	node* tNode;
	top = NULL;

	myX = 0;			
	myY = height - 1;									// myX�� myY�� �ʱⰪ�� �÷��̾��� Ż�� ������ ��� �� ��ȣ�̴�. 

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) 
			route[i][j] = visited[i * width + j] = 0;		// route, visited �迭 �ʱ�ȭ

	visited[myY * width + myX] = route[myY][0] = 1;			// �÷��̾��� ��� �������� �׻� ���� ���� �Ʒ� ĭ�̴�.
	push(myY * width + myX);								// �׿� �ش��ϴ� �ε��� ��ȣ push

	while (top) {

		if (top->v == myDest) {								// �÷��̾��� Ż�� ��� ã�� ���
			freeStack();								// DFS�� ���� �޸� ����

			cnt = 0;

			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++)
					if (route[i][j] == 1)				// route �迭���� value�� 1�� �ε��� ���� = ��ΰ� ������ ������ ����
						cnt++;				

			myIdx = cnt - 1;								// route �迭���� value�� 1�� �ε��� ���� = ��ΰ� ������ ������ ������ myIdx ������ ����
			myTrack = (pos*)malloc(sizeof(pos) * myIdx);	// myIdx ������ŭ myTrack �迭 ���� �Ҵ�

			i = myY; 
			j = myX; 
			idx = 0;		// �ٽ� ó������ ��� ���� ����

			while (1) {		// ��θ� route �迭�� �̿��� �����ϸ鼭 myTrack �迭�� ����

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

			return true;			// Ż�� ��θ� ã�����Ƿ� true ����
		}

		tNode = graph[top->v]; 
		cnt = 0;

		while (tNode) {		// �⺻���� DFS
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

void ofApp::push(int vert) { //���� push

	node* newNode = (node*)malloc(sizeof(node));
	newNode->v = vert;
	newNode->link = top;
	top = newNode;

	return;
}

int ofApp::pop() {	//���� pop

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

void ofApp::dfsdraw()		// ��ǻ���� Ż�� ��θ� �׸���. 
{	
	int i, j, s1, e1, s2, e2;
	node* tnode;

	if (seq >= comIdx) {		// seq ������ Ż�� ����� �� ������ ���� �� �׷��� ������ ������ ��Ÿ����. 
								// �׷��� ������ ��ü ���� �̻��� �Ǹ� ��ǻ�Ͱ� ���� Ż�������� �������� �ǹ�. ��ǻ�� ��.
		ifWin = 0;				// �÷��̾ �������Ƿ� ifWin ������ 0 ����. 
		Reset();				// Reset �Լ�
		return;
	}

	ofSetColor(255, 100, 102); 	
	ofSetLineWidth(SPACE / 2);

	for (i = 0; i < seq; i++) {		// seq ������ ũ�⸸ŭ �켱 Ż���θ� �׷��ش�. ������� ������ �κи� �׸��� ��.
		s1 = comTrack[i].x1;
		e1 = comTrack[i].y1;
		s2 = comTrack[i].x2;
		e2 = comTrack[i].y2;
		ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s2 * 2 + 1));
	}

	s1 = comTrack[seq].x1; e1 = comTrack[seq].y1;
	s2 = comTrack[seq].x2; e2 = comTrack[seq].y2;		// ���� �׷����� �ִ� ������ �������� ����.

	if (s1 == s2) {			// �����̵�
		mov += comSpeed;	// mov ������ŭ �þ�� �׷�����. comSpeed�� mov ������ �þ�� ������ �����Ѵ�. 
		if (e1 > e2) {		// �����ʿ��� �������� �׷����� ���
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e1 * 2 + 1) - mov, SPACE * (s2 * 2 + 1));
			if (SPACE * (e2 * 2 + 1) + mov >= SPACE * (e1 * 2 + 1)) {
				seq++; mov = 0;
			}
		}
		else {			// ���ʿ��� �����ʷ� �׷����� ���
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e1 * 2 + 1) + mov, SPACE * (s2 * 2 + 1));
			if (SPACE * (e1 * 2 + 1) + mov >= SPACE * (e2 * 2 + 1)) {
				seq++; mov = 0;
			}
		}
	}
	else if (e1 == e2) {  // �����̵�
		mov += comSpeed;
		if (s1 > s2) {		// �Ʒ����� ���� �׷����� ���
			ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s1 * 2 + 1) - mov);
			if (SPACE * (s1 * 2 + 1) <= SPACE * (s2 * 2 + 1) + mov) {
				seq++; mov = 0;
			}
		}
		else {				// ������ �Ʒ��� �׷����� ���
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

	for (i = 0; i < myIdx; i++) {		// ��Ʈ�� �׸���. 
		s1 = myTrack[i].x1;
		e1 = myTrack[i].y1;
		s2 = myTrack[i].x2;
		e2 = myTrack[i].y2;
		ofDrawLine(SPACE * (e1 * 2 + 1), SPACE * (s1 * 2 + 1), SPACE * (e2 * 2 + 1), SPACE * (s2 * 2 + 1));
	}
}

void ofApp::Reset() {

	insertRec();		// ���� ���� ����� ������� ���Ḯ��Ʈ�� �����Ѵ�. 

	mov = 0;
	seq = 0;
	isOpen = 0;
	hCnt = 0;
	page = 3;
	isLevel = 0;		// ���� ���� ���� �������� �ʱ�ȭ�Ѵ�. 

	freeMemory();		// �̹� ���� �޸𸮸� �����Ѵ�. 

	return;
}

void ofApp::insertRec() {	// ���� ��� ���Ḯ��Ʈ�� ���� ���� ���� ����

	record* newNode = (record*)malloc(sizeof(record));	//���� ������ ������ ������ ���ο� ����ü �����Ҵ�
	newNode->link = NULL;
	newNode->winLose = ifWin;
	newNode->mazeW = width;
	newNode->mazeH = height;
	newNode->level = isLevel;

	if (rec == NULL) {					//���Ḯ��Ʈ�� �� ����ü ����
		rec = newNode;
	}
	else {
		record* tNode = rec;

		for (; tNode->link != NULL; tNode = tNode->link);
		tNode->link = newNode;
	}

	return;
}

void ofApp::freeRec() {		// ���Ḯ��Ʈ �޸� ����. ���α׷��� ����� �� ȣ��ȴ�.

	record* tnode = rec;
	record* delnode;

	for (; tnode; ) {
		delnode = tnode;
		tnode = tnode->link;
		free(delnode);
	}
	return;
}
