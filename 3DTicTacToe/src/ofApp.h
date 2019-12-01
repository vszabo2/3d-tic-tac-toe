#pragma once

#include <boost/asio.hpp>

#include "cube.h"
#include "ofMain.h"

typedef Point<char> Position;

struct GameConfig {
    unsigned short side_length;
    unsigned short player_count;
    unsigned short player_index;
    unsigned short my_port;
    std::string next_address;
    unsigned short next_port;
};

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
    class Board : public Cube<char, char> {
       public:
        static const char EMPTY = -1;
        Board(char side_length) : Cube<char, char>(side_length) { fill(EMPTY); }
    } board_;

    glm::vec3 GetCenterOfPosition(Position position);
    void DrawField();
    void DrawCursor();
    void DrawMarkers();
    void DrawMarker(char playerIdx, Position position);

   public:
    ofApp(const GameConfig& config)
        : colors_({ofColor::red, ofColor::green, ofColor::blue, ofColor::purple,
                   ofColor::chocolate}),
          game_config_(config),
          board_(config.side_length) {}

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
};
