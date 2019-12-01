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

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket sock_next_;
    boost::asio::ip::tcp::socket sock_prev_;
    boost::asio::ip::tcp::endpoint next_player_endpoint_;
    boost::asio::streambuf send_buf_;
    boost::asio::streambuf recv_buf_;
    bool sock_next_connected_;

    std::string next_player_connection_status_;
    std::string prev_player_connection_status_;
    template <typename HandlerType>
    class Handler {
        HandlerType handler_;

       public:
        ofApp* owner;
        Handler(HandlerType handler) : handler_(handler) {}
        template <typename... Args>
        void operator()(Args... args) {
            (owner->*handler_)(args...);
        }
    };
    Handler<void (ofApp::*)(const boost::system::error_code&)> connect_handler_;
    Handler<void (ofApp::*)(const boost::system::error_code&, std::size_t)> read_handler_;

    void onAccept(const boost::system::error_code& error);
    void onConnect(const boost::system::error_code& error);
    void onRead(const boost::system::error_code& error, std::size_t bytes_transferred);
    void StartGameIfReady();
    void SendMove(const char message[]);

    glm::vec3 GetCenterOfPosition(Position position);
    void DrawField();
    void DrawCursor();
    void DrawMarkers();
    void DrawMarker(char playerIdx, Position position);
    inline void DrawBoard();

    void (ofApp::*active_draw_)();
    void drawSetup();
    void drawMove();
    void drawWait();

   public:
    ofApp(const GameConfig& config)
        : colors_({ofColor::red, ofColor::green, ofColor::blue, ofColor::purple,
                   ofColor::chocolate}),
          game_config_(config),
          board_(config.side_length),
          io_context_(),
          acceptor_(io_context_),
          sock_next_(io_context_),
          sock_prev_(io_context_),
          next_player_endpoint_(
              boost::asio::ip::make_address(config.next_address),
              config.next_port),
          sock_next_connected_(false),
          connect_handler_(&ofApp::onConnect),
          read_handler_(&ofApp::onRead) {}

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
