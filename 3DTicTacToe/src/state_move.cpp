#include "state.h"

namespace cs126ttt {

StateMove::StateMove(ofApp* app) : State(app) {}

void StateMove::draw() {
    app_->cam_.begin();
    app_->DrawBoard();
    app_->DrawCursor();
    app_->cam_.end();
}

}  // namespace cs126ttt
