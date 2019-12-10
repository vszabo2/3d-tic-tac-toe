#pragma once

#include <boost/asio.hpp>
#include <functional>

#include "ofApp.h"

namespace cs126ttt {

class ofApp;

class State {
   protected:
    ofApp* const app_;

   public:
    explicit State(ofApp* app) : app_(app) {}
    virtual ~State() = 0;

    virtual void draw() = 0;
};

class StateSetup : public State {
   private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::endpoint next_player_endpoint_;
    bool sock_next_connected_;

    std::function<void(const boost::system::error_code&)> accept_handler_;
    std::function<void(const boost::system::error_code&)> connect_handler_;

    void onAccept(const boost::system::error_code& error);
    void onConnect(const boost::system::error_code& error);

   public:
    explicit StateSetup(ofApp* app);

    virtual ~StateSetup();

    virtual void draw();
};

class StateMove : public State {
   private:
   public:
    explicit StateMove(ofApp* app);
    virtual ~StateMove();

    virtual void draw();
};

class StateWait : public State {
   private:
    std::function<void(const boost::system::error_code&, std::size_t)>
        read_handler_;

    void onRead(const boost::system::error_code& error,
                std::size_t bytes_transferred);

   public:
    explicit StateWait(ofApp* app);
    virtual ~StateWait();

    virtual void draw();
};

}  // namespace cs126ttt
