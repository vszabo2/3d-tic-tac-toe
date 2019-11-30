#include "ofApp.h"

const float ofApp::slot_size_ = 1;
const float ofApp::cursor_size_factor_ = 0.8;
const float ofApp::marker_size_factor_ = 0.6;

glm::vec3 ofApp::GetCenterOfPosition(Position position) {
    glm::vec3 result(position.x, position.y, position.z);
    result += 0.5;
    result *= slot_size_;
    return result;
}

void ofApp::DrawField() {
    for (int i = 0; i <= board_.GetSideLength(); ++i) {
        for (int j = 0; j <= board_.GetSideLength(); ++j) {
            float a = i * slot_size_;
            float b = j * slot_size_;
            ofDrawLine(0, a, b, field_size_, a, b);
            ofDrawLine(a, 0, b, a, field_size_, b);
            ofDrawLine(a, b, 0, a, b, field_size_);
        }
    }
}

void ofApp::DrawCursor() {
    ofPushStyle();
    ofSetColor(255, 255, 0, 128);
    ofDrawSphere(GetCenterOfPosition(cursor_position_),
                 cursor_size_factor_ * slot_size_ / 2);
    ofPopStyle();
}

void ofApp::DrawMarkers() {
    char sl = board_.GetSideLength();
    for (char i = 0; i < sl; ++i) {
        for (char j = 0; j < sl; ++j) {
            for (char k = 0; k < sl; ++k) {
                Position pos = {i, j, k};
                char marker = board_[pos];
                if (marker != Board::EMPTY) {
                    DrawMarker(marker, pos);
                }
            }
        }
    }
}

void ofApp::DrawMarker(char playerIdx, Position position) {
    ofPushStyle();
    ofSetColor(colors_[playerIdx]);
    ofDrawSphere(GetCenterOfPosition(position),
                 marker_size_factor_ * slot_size_ / 2);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::setup() {
    field_size_ = board_.GetSideLength() * slot_size_;
    board_[{0, 0, 0}] = 1;
    board_[{1, 1, 1}] = 0;
    cursor_position_ = {0, 0, 0};

    // This order of cam setup calls is important.
    // It is important to set the position before the target, so that setTarget
    // updates the orientation of the camera. It is important to call
    // setDistance so that the distance isn't changed on the first update.
    cam_.setPosition(field_size_ / 2, field_size_ / 2, 10.5 * field_size_);
    cam_.setTarget(glm::vec3(field_size_ / 2));
    cam_.setDistance(10 * field_size_);
    cam_.setNearClip(9 * field_size_);
    cam_.setFov(11);
    // It would be nice to be able to disable mouse zooming, but the library
    // doesn't seem to support this individually, so disabling would probably
    // mean reimplementing all other mouse controls.

    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    cam_.begin();
    DrawField();
    DrawMarkers();
    DrawCursor();
    cam_.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
