#include "ofApp.h"

#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    talky.setup("127.0.0.1");

}

//--------------------------------------------------------------
void ofApp::update(){
    std::cout << "A" << std::endl;
    TalkyMessage message;
    talky >> message;
    NdArray<int> arr(0, 0);
    if (message >> arr) {
        std::cout << arr.GetSideLength() << std::endl;
        std::cout << arr.GetNumDimensions() << std::endl;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << arr[{i, j}];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "B" << std::endl;
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
