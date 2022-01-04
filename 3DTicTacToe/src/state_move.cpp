#include <algorithm>

#include "state.h"

namespace vszabo2ttt {

StateMove::StateMove(ofApp* app) : State(app) {}

void StateMove::draw() {
    app_->cam_.begin();
    app_->DrawBoard();
    app_->DrawCursor();
    app_->cam_.end();
    app_->DrawWinText();
}

void StateMove::keyPressed(int key) {
    switch (key) {
        case 'w':
            app_->cursor_position_.y =
                std::min(app_->cursor_position_.y + 1,
                         app_->game_config_.side_length - 1);
            break;

        case 'a':
            app_->cursor_position_.x =
                std::max(app_->cursor_position_.x - 1, 0);
            break;

        case 's':
            app_->cursor_position_.y =
                std::max(app_->cursor_position_.y - 1, 0);
            break;

        case 'd':
            app_->cursor_position_.x =
                std::min(app_->cursor_position_.x + 1,
                         app_->game_config_.side_length - 1);
            break;

        case 'q':
            app_->cursor_position_.z =
                std::max(app_->cursor_position_.z - 1, 0);
            break;

        case 'e':
            app_->cursor_position_.z =
                std::min(app_->cursor_position_.z + 1,
                         app_->game_config_.side_length - 1);
            break;

        case OF_KEY_RETURN:
            char message[MESSAGE_SIZE] = {
                static_cast<char>(app_->game_config_.player_index),
                static_cast<char>(app_->cursor_position_.x),
                static_cast<char>(app_->cursor_position_.y),
                static_cast<char>(app_->cursor_position_.z),
            };

            bool is_next_state_move = true;
            try {
                is_next_state_move = app_->ProcessMove(message);
            } catch (InvalidPositionException&) {
                // do nothing and give the player the opportunity
                // to select a different position
            }

            if (!is_next_state_move) {
                app_->io_context_.restart();
                app_->SetState<StateWait>();
            }
            break;
    }
}

}  // namespace vszabo2ttt
