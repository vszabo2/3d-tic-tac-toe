#include "ofApp.h"

#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    talky.setup("127.0.0.1");

}

//--------------------------------------------------------------
void ofApp::update(){
    TalkyMessage message;
    talky >> message;
    std::cout << "Contents Type: "
              << message.getHeader().getContentsType() << std::endl;
    NdArray<int> arr(0, 0);
    if (message >> (TalkySerialisable&) arr) {
        std::cout << arr.GetSideLength() << '^'
                  << arr.GetNumDimensions() << " array:" << std::endl;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << arr[{i, j}] << ' ';
            }
        }
        std::cout << std::endl;
    }
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
