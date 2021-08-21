/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#define SPACE 10	//미로 한 칸의 너비. draw에 사용. 임의로 변경 가능.
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = false;
	isBFS = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup
//
// Menu function
//
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
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			if (DFS()) {
				isDFS = true;
				isBFS = false;
			}
			bShowInfo = bChecked;

		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen) {
			if (BFS()) {
				isDFS = false;
				isBFS = true;
			}
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction
//--------------------------------------------------------------
void ofApp::update() {

}
//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	ofDrawLine(0, 0, (WIDTH - 1) * SPACE, 0);
	ofDrawLine(0, 0, 0, (HEIGHT - 1) * SPACE);
	ofDrawLine(0, (HEIGHT - 1) * SPACE, (WIDTH - 1) * SPACE, (HEIGHT - 1) * SPACE);
	ofDrawLine((WIDTH - 1) * SPACE, 0, (WIDTH - 1) * SPACE, (HEIGHT - 1) * SPACE);

	for (int i = 1; i < HEIGHT - 1; i++) 
		for (int j = 1; j < WIDTH - 1; j++) 
			if (input[i][j] == '|') 
				ofDrawLine(j * SPACE, (i - 1) * SPACE, j * SPACE, (i + 1) * SPACE);
			else if (input[i][j] == '-') 
				ofDrawLine((j - 1) * SPACE, i * SPACE, (j + 1) * SPACE, i * SPACE);

	if (isDFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (isBFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
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
				isOpen = 1;
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
			
			maze_row = WIDTH / 2;
			maze_col = HEIGHT / 2;	// maze의 실제 row, col
			total = maze_row * maze_col; // maze 방 개수

			route = (int**)malloc(sizeof(int*) * maze_col);		//경로 정보를 저장하는 route배열. HEIGHT, WIDTH 만큼 동적할당
			for (i = 0; i < maze_col; i++)
				route[i] = (int*)malloc(sizeof(int) * maze_row);

			for (i = 0; i < total; i++) {  // graph 인접리스트, visited 초기화
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

						vert1 = (i / 2 - 1) * maze_row + j / 2;
						vert2 = vert1 + maze_row;					// i가 짝수

						if (i % 2 == 1) { // i가 홀수이면 변경
							vert1 = (i / 2) * maze_row + j / 2 - 1;
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
						tNode2->link = newNode1;
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

	//TO DO
	// malloc한 memory를 free해주는 함수
	int i, j;
	node* freenode;

	for (i = 0; i < total; i++) {
		
		while (graph[i]) {
			freenode = graph[i];
			graph[i] = graph[i]->link;
			free(freenode);
		}
	}

	for (i = 0; i < maze_col; i++) 
		free(route[i]);
	free(route);
}

void ofApp::freeStack() {
	node* freenode;
	while (top) {
		freenode = top;
		top = top->link;
		free(freenode);
	}
}

void ofApp::freeQueue() {

	node* freenode;
	while(front != rear) {
		freenode = front;
		front = front->link;
		free(freenode);
	}
}


bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)

	int i, j, cnt, pNum;
	top = NULL;

	for (i = 0; i < maze_col; i++)
		for (j = 0; j < maze_row; j++) {
			route[i][j] = 0;
			visited[i * maze_row + j] = 0;
		}

	visited[0] = route[0][0] = 1;

	push(0);

	node* tNode;
	while (top) {

		if (top->v == total - 1) {
			freeStack();
			return true;
		}
		
		tNode = graph[top->v];
		cnt = 0;
		while (tNode) {
			if (!visited[tNode->v]) {
				visited[tNode->v] = 1;
				push(tNode->v);
				i = tNode->v / maze_row;
				j = tNode->v % maze_row;
				route[i][j] = 1;
				cnt++;
				break;
			}
			tNode = tNode->link;
		}

		if (cnt == 0) {
			pNum = pop();
			i = pNum / maze_row;
			j = pNum % maze_row;
			route[i][j] = 2;
		}
	}

	freeStack();

	return false;
}

bool ofApp::BFS() {

	int i, j, next, idx, x, y, fnum, tIdx, num, rIdx, flag;
	node* tnode;
	int* arr;
	arr = (int*)malloc(sizeof(int) * total);
	idx = 0;
	front = rear = NULL;

	for (i = 0; i < maze_col; i++)
		for (j = 0; j < maze_row; j++) {
			route[i][j] = 0;
			visited[i * maze_row + j] = 0;
		}

	visited[0] = route[0][0] = 1;

	insertQueue(0);

	while (front) {
		next = delQueue();

		if (next > 0) {
			arr[idx++] = next;
			x = next / maze_row;
			y = next % maze_row;
			route[x][y] = 2;
		}

		if (next == total - 1) {

			rIdx = idx - 1;
			num = arr[rIdx];
			x = num / maze_row;
			y = num % maze_row;
			route[x][y] = 1;

			tIdx = rIdx - 1;
			tnode = graph[arr[rIdx]];

			while (rIdx > 0) {
				flag = 0;
				
				while (tnode) {
					if (tnode->v == arr[tIdx]) {
						flag = 1; 
						break;
					}
					tnode = tnode->link;
				}

				if (flag == 1) {
					x = arr[tIdx] / maze_row;
					y = arr[tIdx] % maze_row;
					route[x][y] = 1;
					rIdx = tIdx--;
					tnode = graph[arr[rIdx]];
				}
				else {
					tIdx--;
					tnode = graph[arr[rIdx]];
				}
			}
			free(arr);
			freeQueue();
			return true;
		}

		for (tnode = graph[next]; tnode; tnode = tnode->link) 
			if (!visited[tnode->v]) {
				visited[tnode->v] = 1;
				insertQueue(tnode->v);
				x = tnode->v / maze_row;
				y = tnode->v % maze_row;
				route[x][y] = 2;
			}
	}
	free(arr);
	freeQueue();
	return false;
}

void ofApp::push(int vert) {

	node* newNode = (node*)malloc(sizeof(node));
	newNode->v = vert;
	newNode->link = top;
	top = newNode;
}

int ofApp::pop() {

	if (!top) {
		printf("Stack Error \n");
		return 0;
	}

	int ret;
	node* delNode = top;
	top = top->link;

	ret = delNode->v;
	free(delNode);

	return ret;
}

void ofApp::insertQueue(int vert) {

	node* newNode = (node*)malloc(sizeof(node));
	newNode->v = vert;
	newNode->link = NULL;

	if (front == NULL) 
		front = rear = newNode;
	else {
		rear->link = newNode;
		rear = newNode;
	}
}

int ofApp::delQueue() {

	if (front == NULL) {
		printf("Queue is Empty!! \n");
		return 0;
	}

	int ret;
	node* delnode = front;

	front = front->link;
	ret = delnode->v;

	free(delnode);
	return ret;
}

void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	
	int i, j, rnum, x, y;
	node* tnode;

	for(i = 0; i < maze_col; i++)
		for (j = 0; j < maze_row; j++) {

			if (route[i][j] > 0) {
				tnode = graph[i * maze_row + j];
				while (tnode != NULL) {
					rnum = tnode->v;
					x = rnum / maze_row;
					y = rnum % maze_row;

					if (route[x][y] > 0) {
						ofSetLineWidth(5);
						if (route[i][j] == 1 && route[x][y] == 1)
							ofSetColor(255, 100, 102);
						else {
							ofSetColor(51, 102, 255); 	ofSetLineWidth(3);
						}
						ofDrawLine(SPACE * (j * 2 + 1), SPACE * (i * 2 + 1), SPACE * (y * 2 + 1), SPACE * (x * 2 + 1));
					}
					tnode = tnode->link;
				}
			}
		}

}

void ofApp::bfsdraw() {

	dfsdraw();
}

