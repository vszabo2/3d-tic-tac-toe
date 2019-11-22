#include "ofApp.h"

#include "nd_array.h"

//--------------------------------------------------------------
void ofApp::setup(){
    talky.setup();
    header.setCompany("VS");
    header.setProtocol("TT");
    header.setVersion(2);
}

//--------------------------------------------------------------
void ofApp::update(){
    NdArray<int> arr(3, 2);

    int val = 8;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            arr[{i, j}] = val--;
        }
    }

    header.setContentsType(floor(ofGetElapsedTimef()));
    TalkyMessage message(header);
    message << (TalkySerialisable&) arr;
    talky << message;
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
