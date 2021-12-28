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

}  // namespace vszabo2ttt
