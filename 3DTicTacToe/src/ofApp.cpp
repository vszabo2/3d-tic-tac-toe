#include "ofApp.h"

#include <sstream>
#include <thread>

#include "state.h"

namespace vszabo2ttt {

void ofApp::SendMove(const char message[]) {
    boost::asio::streambuf send_buf_;
    send_buf_.sputn(message, MESSAGE_SIZE);
    sock_next_.send(send_buf_.data());
    send_buf_.consume(MESSAGE_SIZE);
}

bool ofApp::ProcessMove(const char message[]) {
    Position position = {static_cast<unsigned char>(message[1]),
                         static_cast<unsigned char>(message[2]),
                         static_cast<unsigned char>(message[3])};
    if (board_[position] != Board::EMPTY) {
        throw InvalidPositionException();
    }

    if (message[0] !=
        (game_config_.player_index + 1) % game_config_.player_count) {
        SendMove(message);
    }

    board_[position] = message[0];

    if (IsWinningMove(board_, position)) {
        std::ostringstream sstream;
        sstream << "Player " << (short)message[0] << " has won!";
        winner_message_ = sstream.str();
    }

    return message[0] ==
           (game_config_.player_index + game_config_.player_count - 1) %
               game_config_.player_count;
}

glm::vec3 ofApp::GetCenterOfPosition(Position position) {
    glm::vec3 result(position.x, position.y, position.z);
    result += 0.5;
    result *= slot_size_;
    return result;
}

void ofApp::DrawCursor() {
    ofPushStyle();
    ofSetColor(255, 255, 0, 128);
    ofDrawSphere(GetCenterOfPosition(cursor_position_),
                 cursor_size_factor_ * slot_size_ / 2);
    ofPopStyle();
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

void ofApp::DrawHints() {
    const float half_field = field_size_ / 2;
    const float distance = field_size_ / 4;
    const float far_distance = field_size_ + distance;

    ofPushStyle();
    ofSetColor(ofColor::black);
    ofDrawBitmapString("W", glm::vec3(half_field, far_distance, half_field));
    ofDrawBitmapString("A", glm::vec3(-distance, half_field, half_field));
    ofDrawBitmapString("S", glm::vec3(half_field, -distance, half_field));
    ofDrawBitmapString("D", glm::vec3(far_distance, half_field, half_field));
    ofDrawBitmapString("Q", glm::vec3(half_field, half_field, -distance));
    ofDrawBitmapString("E", glm::vec3(half_field, half_field, far_distance));
    ofPopStyle();
}

void ofApp::DrawMarkers() {
    unsigned char sl = board_.GetSideLength();
    for (unsigned char i = 0; i < sl; ++i) {
        for (unsigned char j = 0; j < sl; ++j) {
            for (unsigned char k = 0; k < sl; ++k) {
                Position pos = {i, j, k};
                unsigned char marker = board_[pos];
                if (marker != Board::EMPTY) {
                    DrawMarker(marker, pos);
                }
            }
        }
    }
}

void ofApp::DrawMarker(unsigned char player_index, Position position) {
    ofPushStyle();
    ofSetColor(colors_[player_index]);
    ofDrawSphere(GetCenterOfPosition(position),
                 marker_size_factor_ * slot_size_ / 2);
    ofPopStyle();
}

void ofApp::DrawWinText() {
    if (winner_message_.size() > 0)
        ofDrawBitmapStringHighlight(winner_message_, 10, 20);
}

void ofApp::DrawBoard() {
    DrawField();
    DrawMarkers();
    DrawHints();
}

ofApp::ofApp(const GameConfig& config)
    : colors_{ofColor::red, ofColor::green, ofColor::blue, ofColor::purple,
              ofColor::chocolate},
      game_config_(config),
      board_(config.side_length),
      io_context_(),
      sock_next_(io_context_),
      sock_prev_(io_context_) {}

//--------------------------------------------------------------
void ofApp::setup() {
    field_size_ = board_.GetSideLength() * slot_size_;
    cursor_position_ = {0, 0, 0};

    next_player_connection_status_ = "Connecting to next player...";
    prev_player_connection_status_ =
        "Waiting for previous player to connect...";

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

    curr_state_ = new StateSetup(this);
}

//--------------------------------------------------------------
void ofApp::update() {
    io_context_.poll();
    std::this_thread::yield();
}

//--------------------------------------------------------------
void ofApp::draw() { curr_state_->draw(); }

//--------------------------------------------------------------
void ofApp::keyPressed(int key) { curr_state_->keyPressed(key); }

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

}  // namespace vszabo2ttt
