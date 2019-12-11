#include "state.h"

namespace cs126ttt {

void StateSetup::onAccept(const boost::system::error_code& error) {
    std::cout << "Accepted. Error: " << error << std::endl;
    acceptor_.close();
    if (error) {
        app_->prev_player_connection_status_ =
            "Failed to start server for previous player.";
    } else {
        std::stringstream stream;
        stream << "Previous player connected from "
               << app_->sock_prev_.remote_endpoint().address().to_string() << ':'
               << app_->sock_prev_.remote_endpoint().port();
        app_->prev_player_connection_status_ = stream.str();

        sock_prev_connected_ = true;
        StartGameIfReady();
    }
}

void StateSetup::onConnect(const boost::system::error_code& error) {
    std::cout << "Connected. Error: " << error << std::endl;
    if (error) {
        app_->next_player_connection_status_ =
            "Failed to connect to next player. Retrying...";
        app_->sock_next_.async_connect(next_player_endpoint_, connect_handler_);
    } else {
        app_->sock_next_.set_option(boost::asio::ip::tcp::no_delay(true));

        std::stringstream stream;
        stream << "Connected to next player via "
               << app_->sock_next_.local_endpoint().address().to_string() << ':'
               << app_->sock_next_.local_endpoint().port();
        app_->next_player_connection_status_ = stream.str();

        sock_next_connected_ = true;
        StartGameIfReady();
    }
}

void StateSetup::StartGameIfReady() {
    if (sock_next_connected_ && sock_prev_connected_) {
        if (app_->game_config_.player_index == 0) {
            app_->SetState<StateMove>();
        } else {
            app_->SetState<StateWait>();
        }
    }
}

StateSetup::StateSetup(ofApp* app)
        : State(app),
          acceptor_(app->io_context_),
          next_player_endpoint_(
              boost::asio::ip::make_address(app->game_config_.next_address),
              app->game_config_.next_port),
          sock_next_connected_(false),
          sock_prev_connected_(false),
          accept_handler_(std::bind(&StateSetup::onAccept, this, std::placeholders::_1)),
          connect_handler_(
              std::bind(&StateSetup::onConnect, this, std::placeholders::_1)) {
    boost::asio::ip::tcp::endpoint server_endpoint(boost::asio::ip::tcp::v4(),
                                                   app_->game_config_.my_port);
    acceptor_.open(boost::asio::ip::tcp::v4());
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor_.bind(server_endpoint);
    acceptor_.listen(1);
    acceptor_.async_accept(app_->sock_prev_, accept_handler_);

    app_->sock_next_.async_connect(next_player_endpoint_, connect_handler_);
}

void StateSetup::draw() {
    ofDrawBitmapString("Setup", 0, 20);
    ofDrawBitmapString(app_->prev_player_connection_status_, 0, 40);
    ofDrawBitmapString(app_->next_player_connection_status_, 0, 60);
}

}  // namespace cs126ttt
