#include "state.h"

namespace cs126ttt {

void StateWait::onRead(const boost::system::error_code& error,
                       std::size_t bytes_transferred) {
    if (error) {
        ofExit();
    } else {
        recv_buf_.commit(bytes_transferred);
        if (recv_buf_.in_avail() < MESSAGE_SIZE) {
            app_->sock_prev_.async_read_some(
                recv_buf_.prepare(MESSAGE_SIZE - recv_buf_.in_avail()),
                read_handler_);
            return;
        }

        char message[MESSAGE_SIZE];
        size_t bytes_got = recv_buf_.sgetn(message, MESSAGE_SIZE);
        assert(bytes_got == MESSAGE_SIZE);
        assert(recv_buf_.in_avail() == 0);

        bool transition_move = false;
        try {
            transition_move = app_->ProcessMove(message);
        } catch (InvalidPositionException& ex) {
            std::cerr << ex.what() << std::endl;
        }

        if (transition_move) {
            app_->SetState<StateMove>();
        } else {
            app_->sock_prev_.async_read_some(recv_buf_.prepare(MESSAGE_SIZE),
                                             read_handler_);
        }
    }
}

StateWait::StateWait(ofApp* app)
    : State(app),
      read_handler_(std::bind(&StateWait::onRead, this, std::placeholders::_1,
                              std::placeholders::_2)) {
    app_->sock_prev_.async_read_some(recv_buf_.prepare(MESSAGE_SIZE),
                                     read_handler_);
}

void StateWait::draw() {
    app_->cam_.begin();
    app_->DrawBoard();
    app_->cam_.end();
    app_->DrawWinText();
}

}  // namespace cs126ttt
