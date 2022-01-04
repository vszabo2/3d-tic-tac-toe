#pragma once

#include <functional>

#include <boost/asio.hpp>

#include "ofApp.h"

namespace vszabo2ttt {

class ofApp;

class State {
   protected:
    ofApp* const app_;

   public:
    explicit State(ofApp* app) : app_(app) {}
    virtual ~State(){};

    virtual void draw() = 0;
    virtual void keyPressed(int key) = 0;
};

class StateSetup : public State {
   private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::endpoint next_player_endpoint_;
    bool sock_next_connected_;
    bool sock_prev_connected_;

    std::function<void(const boost::system::error_code&)> accept_handler_;
    std::function<void(const boost::system::error_code&)> connect_handler_;

    void onAccept(const boost::system::error_code& error);
    void onConnect(const boost::system::error_code& error);

    void StartGameIfReady();

   public:
    explicit StateSetup(ofApp* app);

    virtual void draw();
    virtual void keyPressed(int key) {}
};

class StateMove : public State {
   private:
   public:
    explicit StateMove(ofApp* app);

    virtual void draw();
    virtual void keyPressed(int key);
};

class StateWait : public State {
   private:
    boost::asio::streambuf recv_buf_;

    std::function<void(const boost::system::error_code&, std::size_t)>
        read_handler_;

    void onRead(const boost::system::error_code& error,
                std::size_t bytes_transferred);

   public:
    explicit StateWait(ofApp* app);

    virtual void draw();
    virtual void keyPressed(int key) {}
};

}  // namespace vszabo2ttt
