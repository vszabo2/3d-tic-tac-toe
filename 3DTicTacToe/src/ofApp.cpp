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

void ofApp::DrawMarker(char player_index, Position position) {
    ofPushStyle();
    ofSetColor(colors_[player_index]);
    ofDrawSphere(GetCenterOfPosition(position),
                 marker_size_factor_ * slot_size_ / 2);
    ofPopStyle();
}

inline void ofApp::DrawBoard() {
    DrawField();
    DrawMarkers();
}

void ofApp::onAccept(const boost::system::error_code& error) {
    std::cout << "Accepted. Error: " << error << std::endl;
    acceptor_.close();
    if (error) {
        prev_player_connection_status_ =
            "Failed to start server for previous player.";
    } else {
        std::stringstream stream;
        stream << "Previous player connected from "
               << sock_prev_.remote_endpoint().address().to_string() << ':'
               << sock_prev_.remote_endpoint().port();
        prev_player_connection_status_ = stream.str();
        StartGameIfReady();
    }
}

void ofApp::onConnect(const boost::system::error_code& error) {
    std::cout << "Connected. Error: " << error << std::endl;
    if (error) {
        next_player_connection_status_ =
            "Failed to connect to next player. Retrying...";
        sock_next_.async_connect(next_player_endpoint_, connect_handler_);
    } else {
        sock_next_.set_option(boost::asio::ip::tcp::no_delay(true));

        std::stringstream stream;
        stream << "Connected to next player via "
               << sock_next_.local_endpoint().address().to_string() << ':'
               << sock_next_.local_endpoint().port();
        next_player_connection_status_ = stream.str();

        sock_next_connected_ = true;
        StartGameIfReady();
    }
}

void ofApp::onRead(const boost::system::error_code& error,
                   std::size_t bytes_transferred) {
    std::cerr << "Read. Error: " << error << std::endl;
    if (error) {
        ofExit();
    } else {
        recv_buf_.commit(bytes_transferred);
        if (recv_buf_.in_avail() < 4) {
            sock_prev_.async_read_some(
                recv_buf_.prepare(4 - recv_buf_.in_avail()), read_handler_);
            return;
        }

        char message[4];
        size_t bytes_got = recv_buf_.sgetn(message, 4);
        assert(bytes_got == 4);
        assert(recv_buf_.in_avail() == 0);

        if (message[0] !=
            (game_config_.player_index + 1) % game_config_.player_count) {
            SendMove(message);
        }

        board_[{message[1], message[2], message[3]}] = message[0];

        if (message[0] ==
            (game_config_.player_index + game_config_.player_count - 1) %
                game_config_.player_count) {
            active_draw_ = &ofApp::drawMove;
        } else {
            sock_prev_.async_read_some(recv_buf_.prepare(4), read_handler_);
        }
    }
}

void ofApp::StartGameIfReady() {
    if (sock_next_connected_ && sock_prev_.is_open() &&
        active_draw_ == &ofApp::drawSetup) {
        if (game_config_.player_index == 0) {
            active_draw_ = &ofApp::drawMove;
        } else {
            active_draw_ = &ofApp::drawWait;
            sock_prev_.async_read_some(recv_buf_.prepare(4), read_handler_);
        }
    }
}

void ofApp::SendMove(const char message[]) {
    send_buf_.sputn(message, 4);
    int bytes_sent = sock_next_.send(send_buf_.data());
    send_buf_.consume(4);
    std::cerr << "Sent " << bytes_sent << " bytes" << std::endl;
}

//--------------------------------------------------------------
void ofApp::setup() {
    field_size_ = board_.GetSideLength() * slot_size_;
    cursor_position_ = {0, 0, 0};
    active_draw_ = &ofApp::drawSetup;

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

    accept_handler_.owner = this;
    connect_handler_.owner = this;
    read_handler_.owner = this;

    boost::asio::ip::tcp::endpoint server_endpoint(boost::asio::ip::tcp::v4(),
                                                   game_config_.my_port);
    acceptor_.open(boost::asio::ip::tcp::v4());
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor_.bind(server_endpoint);
    acceptor_.listen(1);
    acceptor_.async_accept(sock_prev_, accept_handler_);

    sock_next_.async_connect(next_player_endpoint_, connect_handler_);
}

//--------------------------------------------------------------
void ofApp::update() { io_context_.poll(); }

//--------------------------------------------------------------
void ofApp::draw() { (this->*active_draw_)(); }

void ofApp::drawSetup() {
    ofDrawBitmapString("Setup", 0, 20);
    ofDrawBitmapString(prev_player_connection_status_, 0, 40);
    ofDrawBitmapString(next_player_connection_status_, 0, 60);
}

void ofApp::drawMove() {
    cam_.begin();
    DrawBoard();
    DrawCursor();
    cam_.end();
}

void ofApp::drawWait() {
    cam_.begin();
    DrawBoard();
    cam_.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (active_draw_ != &ofApp::drawMove) return;

    switch (key) {
        case 'w':
            cursor_position_.y = std::min(cursor_position_.y + 1,
                                          game_config_.side_length - 1);
            break;
        case 'a':
            cursor_position_.x = std::max(cursor_position_.x - 1, 0);
            break;
        case 's':
            cursor_position_.y = std::max(cursor_position_.y - 1, 0);
            break;
        case 'd':
            cursor_position_.x = std::min(cursor_position_.x + 1,
                                          game_config_.side_length - 1);
            break;
        case 'q':
            cursor_position_.z = std::max(cursor_position_.z - 1, 0);
            break;
        case 'e':
            cursor_position_.z = std::min(cursor_position_.z + 1,
                                          game_config_.side_length - 1);
            break;
        case OF_KEY_RETURN:
            // TODO: make this work properly for one player
            if (board_[cursor_position_] != Board::EMPTY) break;

            board_[cursor_position_] = game_config_.player_index;

            char message[4] = {game_config_.player_index, cursor_position_.x,
                               cursor_position_.y, cursor_position_.z};
            SendMove(message);
            active_draw_ = &ofApp::drawWait;
            io_context_.restart();
            sock_prev_.async_read_some(recv_buf_.prepare(4), read_handler_);
            break;
    }
}

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
