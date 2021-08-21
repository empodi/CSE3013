#include "ofApp.h"
#define EPSILON 0.01

//--------------------------------------------------------------
void ofApp::setup() {

    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second

    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255, 255, 255);
    ofSetLineWidth(4);

    draw_flag = 0;
    load_flag = 0;
    lineIdx = -1;
    dotIdx = -1;
    selectDot = 0;
    dot_diameter = 20.0f;

    water_flag = 0;
    wIdx = 0;
    drop = 0.0;
    seq = 0;
    WaterObj* water;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

    ofSetColor(127, 23, 31);  // Set the drawing color to brown

    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);

    if (draw_flag) {

        int i;
        double xInc;

        ofSetColor(255, 0, 0);
        for (i = 0; i <= lineIdx; i++)
            ofDrawLine(lineArr[i].xCoor1, lineArr[i].yCoor1, lineArr[i].xCoor2, lineArr[i].yCoor2);

        for (i = 0; i <= dotIdx; i++) {

            if (i == selectDot)
                ofSetColor(255, 0, 0);
            else
                ofSetColor(0);

            ofDrawCircle(dotArr[i].xCoor, dotArr[i].yCoor, 10);
        }

        // 2nd week portion.
        ofSetLineWidth(3);
        ofSetColor(0, 0, 255);

        if (water_flag) {

            for (i = 0; i <= wIdx; i++)
                if (water[i].isDrawn && seq > 0)
                    ofDrawLine(water[i].wx1, water[i].wy1, water[i].wx2, water[i].wy2);

            if (seq > wIdx) return;

            if (seq % 2 == 0) {
                drop += 2;
                drop += drop / 100.0;
                ofDrawLine(water[seq].wx1, water[seq].wy1, water[seq].wx1, water[seq].wy1 + drop);
            }
            else if (seq % 2 == 1) {
                drop += abs(water[seq].slope);
                drop += drop / 100.0;
                xInc = drop / water[seq].slope;
                ofDrawLine(water[seq].wx1, water[seq].wy1, water[seq].wx1 + xInc, water[seq].wy1 + drop);
            }
            if (water[seq].wy1 + drop >= water[seq].wy2) {
                water[seq].isDrawn = 1;
                seq++;
                drop = 0;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    if (key == 'q') {
        // Reset flags
        draw_flag = 0;

        delete(dotArr);
        delete(lineArr);

        cout << "Dynamically allocated memory has been freed." << endl;

        _Exit(0);
    }
    if (key == 'd') {
        if (!load_flag) return;

        draw_flag = 1;
    }
    if (key == 's') {
        // 2nd week portion.
        if (!load_flag || water_flag) return;

        water = (WaterObj*)malloc(sizeof(WaterObj) * (2 * num_of_line + 1));
        wIdx = 0;
        initializeWaterLines();

        water_flag = 1;
    }
    if (key == 'e') {
        // 2nd week portion.
        if (!load_flag || !water_flag) return;

        water_flag = 0;
        drop = 0;
        seq = 0;
        delete(water);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

    if (key == 'l' || key == 'L') {
        // Open the Open File Dialog
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

        // Check whether the user opened a file
        if (openFileResult.bSuccess) {
            ofLogVerbose("User selected a file");

            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }

    if (key == OF_KEY_RIGHT && !water_flag) {

        selectDot++;
        selectDot %= (dotIdx + 1);

        cout << "Selcted Dot Coordinate is (" << dotArr[selectDot].xCoor << ", " << dotArr[selectDot].yCoor << ")" << endl;
    }
    if (key == OF_KEY_LEFT && !water_flag) {

        selectDot--;

        if (selectDot < 0) selectDot += (dotIdx + 1);

        cout << "Selcted Dot Coordinate is (" << dotArr[selectDot].xCoor << ", " << dotArr[selectDot].yCoor << ")" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    //Path to the comma delimited file
    //string fileName = "input.txt";
    string fileName = openFileResult.getName();
    ofFile file(fileName);

    if (!file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;

    ofBuffer buffer(file);

    int input_type = 0, input_type_flag = 0;


    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;

        // Split line into strings
        vector<string> words = ofSplitString(line, " ");

        if (words.size() == 1) {
            if (input_type == 0) { // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                lineArr = new lineInfo[num_of_line];
            }
            else { // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
                dotArr = new dotInfo[num_of_dot];
            }
        }
        else if (words.size() >= 2) {
            int x1, y1, x2, y2;
            if (input_type == 0) { // Input for actual information of lines
                input_type_flag++;
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());
                if (y1 > y2) {
                    int temp;
                    temp = x1; x1 = x2; x2 = temp;
                    temp = y1; y1 = y2; y2 = temp;
                }
                if (x1 <= ofGetWidth() && x2 <= ofGetWidth() && y1 <= ofGetHeight() && y2 <= ofGetHeight()) {
                    lineArr[++lineIdx].xCoor1 = x1;
                    lineArr[lineIdx].xCoor2 = x2;
                    lineArr[lineIdx].yCoor1 = y1;
                    lineArr[lineIdx].yCoor2 = y2;
                }
                if (input_type_flag == num_of_line) input_type++;
            }
            else { // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());

                if (x1 <= ofGetWidth() && y1 <= ofGetHeight()) {
                    dotArr[++dotIdx].xCoor = x1;
                    dotArr[dotIdx].yCoor = y1;
                }
            }
        } // End of else if.
    } // End of for-loop (Read file line by line).
}

double ofApp::calSlope(int x1, int y1, int x2, int y2) {

    int xx = x1 - x2;
    int yy = y1 - y2;

    return ((double)yy / xx);
}

int ofApp::checkRange(int xPos, int yPos, int idx) {

    if (yPos < lineArr[idx].yCoor1 || yPos > lineArr[idx].yCoor2) return 0;

    int smallX = lineArr[idx].xCoor1;
    int bigX = lineArr[idx].xCoor2;

    if (smallX > bigX) {
        smallX = lineArr[idx].xCoor2;
        bigX = lineArr[idx].xCoor1;
    }
    if (xPos < smallX || xPos > bigX) return 0;

    return 1;
}

void ofApp::initializeWaterLines() {

    int i, startX, wFall;
    double lineSlope, chgSlope;
    startX = water[0].wx1 = dotArr[selectDot].xCoor;
    wFall = water[0].wy1 = dotArr[selectDot].yCoor;

    for (; wFall < ofGetHeight(); wFall++) {
        for (i = 0; i <= lineIdx; i++) {

            lineSlope = calSlope(lineArr[i].xCoor1, lineArr[i].yCoor1, lineArr[i].xCoor2, lineArr[i].yCoor2);
            chgSlope = calSlope(startX, wFall, lineArr[i].xCoor2, lineArr[i].yCoor2);

            if (abs(lineSlope - chgSlope) <= EPSILON && checkRange(startX, wFall, i)) {

                water[wIdx].wx2 = water[wIdx + 1].wx1 = startX;
                water[wIdx].wy2 = water[wIdx + 1].wy1 = wFall;
                water[wIdx + 1].wx2 = water[wIdx + 2].wx1 = startX = lineArr[i].xCoor2;
                water[wIdx + 1].wy2 = water[wIdx + 2].wy1 = wFall = lineArr[i].yCoor2;
                water[wIdx].slope = water[wIdx + 2].slope = 0; 
                water[wIdx + 1].slope = chgSlope;
                water[wIdx].isDrawn = water[wIdx + 1].isDrawn = water[wIdx + 2].isDrawn = 0;
                wIdx += 2;
                break;
            }
        }
    }
    if (wIdx == 0) water[wIdx].slope = 0;
    water[wIdx].wx2 = water[wIdx].wx1;
    water[wIdx].wy2 = ofGetHeight() - 20;
}