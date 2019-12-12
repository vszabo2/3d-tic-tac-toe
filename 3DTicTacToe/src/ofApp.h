#pragma once

#include <boost/asio.hpp>
#include <functional>

#include "game_types.h"
#include "ofMain.h"
#include "state.h"

namespace cs126ttt {

class State;

class ofApp : public ofBaseApp {
   private:
    static const float slot_size_;
    static const float cursor_size_factor_;
    static const float marker_size_factor_;

    const ofColor colors_[5];

    float field_size_;
    ofEasyCam cam_;
    Position cursor_position_;
    const GameConfig& game_config_;
    Board board_;

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket sock_next_;
    boost::asio::ip::tcp::socket sock_prev_;

    std::string next_player_connection_status_;
    std::string prev_player_connection_status_;

    void SendMove(const char message[]);

    glm::vec3 GetCenterOfPosition(Position position);
    void DrawCursor();
    void DrawField();
    void DrawMarkers();
    void DrawMarker(char player_index, Position position);
    void DrawBoard();

    State* curr_state_;
    template <class StateClass>
    void SetState() {
        delete curr_state_;
        curr_state_ = new StateClass(this);
    }

   public:
    ofApp(const GameConfig& config)
        : colors_({ofColor::red, ofColor::green, ofColor::blue, ofColor::purple,
                   ofColor::chocolate}),
          game_config_(config),
          board_(config.side_length),
          io_context_(),
          sock_next_(io_context_),
          sock_prev_(io_context_) {}

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

    friend class StateSetup;
    friend class StateMove;
    friend class StateWait;
};

}  // namespace cs126ttt
