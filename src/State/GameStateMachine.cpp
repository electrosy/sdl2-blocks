#include "../../inc/State/GameStateMachine.h"

namespace ley {

void ley::GameStateMachine::pushState(GameState *pState) {
    mGameStates.push_back(pState);
    mGameStates.back()->onEnter();
}

void ley::GameStateMachine::popState() {
    if(!mGameStates.empty()) {
        if(mGameStates.back()->onExit()) {
            delete mGameStates.back();
            mGameStates.pop_back();
        }
    }
}

void ley::GameStateMachine::changeState(GameState *pState) {
    if(!mGameStates.empty()) {
        if(mGameStates.back()->getStateID() == pState->getStateID()) {
            return; // Do nothing
        }

        if(mGameStates.back()->onExit()) {
            delete mGameStates.back();
            mGameStates.pop_back();
        }
    }

    mGameStates.push_back(pState);
    mGameStates.back()->onEnter();
}

void ley::GameStateMachine::update(ley::Command command) {
    if(!mGameStates.empty()) {
        mGameStates.back()->update(command);
    }
}

}