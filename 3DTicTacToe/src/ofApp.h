#pragma once

#include <string>

#include <boost/asio.hpp>
#include <ofMain.h>

#include "game_types.h"
#include "get_winner.h"
#include "state.h"

namespace vszabo2ttt {

class State;

class ofApp : public ofBaseApp {
   private:
    static constexpr float slot_size_ = 1;
    static constexpr float cursor_size_factor_ = 0.8;
    static constexpr float marker_size_factor_ = 0.6;

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
    std::string winner_message_;

    State* curr_state_;

    template <class StateClass>
    void SetState() {
        delete curr_state_;
        curr_state_ = new StateClass(this);
    }

    void SendMove(const char message[]);
    // returns true if the next state should be Read, false if Move
    bool ProcessMove(const char message[]);

    glm::vec3 GetCenterOfPosition(Position position);

    void DrawCursor();
    void DrawField();
    void DrawHints();
    void DrawMarkers();
    void DrawMarker(unsigned char player_index, Position position);
    void DrawWinText();
    void DrawBoard();

   public:
    ofApp(const GameConfig& config);

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

}  // namespace vszabo2ttt
