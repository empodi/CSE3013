#pragma once

#include "ofMain.h"

class WaterObj {

public:
	int wx1;
	int	wy1;
	int wx2;
	int wy2;
	int isDrawn;
	double slope;

};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	class lineInfo {

	public:
		int xCoor1;
		int yCoor1;
		int xCoor2;
		int yCoor2;
	};

	class dotInfo {

	public:
		int xCoor;
		int yCoor;
	};


	lineInfo* lineArr;
	dotInfo* dotArr;

	/* WaterFall-related member variables Regions */
	WaterObj* water;

	// flag variables
	int draw_flag;
	int load_flag;
	int water_flag;

	// Line segment and dot related variables
	int num_of_line, num_of_dot;
	int lineIdx, dotIdx, wIdx;
	float dot_diameter;
	int selectDot;

	int seq;
	double drop;

	/* WaterFall-related member functions */

	void processOpenFileSelection(ofFileDialogResult openFileResult);
	double calSlope(int x1, int y1, int x2, int y2);
	int checkRange(int xPos, int yPos, int idx);
	void initializeWaterLines(); // 2nd week portion.

};


