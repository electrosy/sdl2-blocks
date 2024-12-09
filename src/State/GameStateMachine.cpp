#include "../../inc/State/GameStateMachine.h"

namespace ley {

void ley::GameStateMachine::pushState(GameState *pState) {
    //Call the OnPause for the current state.        
    if(!mGameStates.empty()) {
        mGameStates.back()->onPause();
    }
    //Then push the new state and call onEnter()
    mGameStates.push_back(pState);
    mGameStates.back()->onEnter();
}

void ley::GameStateMachine::popState() {
    
    //Remove the current state
    if(!mGameStates.empty()) {
        if(mGameStates.back()->onExit()) {
            delete mGameStates.back();
            mGameStates.pop_back();
        }
    }

    //If there is another state remaining in the state call the onReEnter()
    if(!mGameStates.empty()) {
        mGameStates.back()->onReEnter();
    }
}

void ley::GameStateMachine::changeState(GameState *pState) {
    if(!mGameStates.empty()) {
        if(mGameStates.back()->getStateID() == pState->getStateID()) {
            return; // Do nothing if we try to change to the same state.
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

void ley::GameStateMachine::render() {
    if(!mGameStates.empty()) {
        mGameStates.back()->render();
    }
}

std::string ley::GameStateMachine::getStateId() {
    if(!mGameStates.empty()) {
        return mGameStates.back()->getStateID();
    }

    return "";
}

bool ley::GameStateMachine::isStateDone() {
    if(!mGameStates.empty()) {
        return mGameStates.back()->isDone();
    }
    return false;
}

ley::TextEntry* ley::GameStateMachine::activeUIElement() {
    if(!mGameStates.empty()) {
        return mGameStates.back()->activeUIelement();
    }
    else {
        return nullptr;
    }
}

void ley::GameStateMachine::UI_ToggleFocus() {
    if(!mGameStates.empty()) {
        return mGameStates.back()->UI_ToggleFocus();
    }
}

void ley::GameStateMachine::commitUI() {
    if(!mGameStates.empty()) {
        return mGameStates.back()->commitUI();
    }
}

}