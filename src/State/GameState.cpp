#include "../../inc/State/GameState.h"

namespace ley {

TextEntry* GameState::mActiveUIElement = {};

bool GameState::onPause() {
    std::string pause = "Pausing " + getStateID();
    SDL_Log(pause.c_str());

    return true;
}

bool GameState::isDone() {
    return done;
}

void GameState::commitUI() {
    ;
}

// TODO this mTextEntry can no go into the individual states and this method will return a pointer to the active element.
ley::TextEntry* GameState::activeUIelement() {
    return /*&mTextEntry;*/ mActiveUIElement;
}

void GameState::UI_ToggleFocus(/*ley::TextEntry* activeUIElement*/) {
    
    // TODO it would be best if we didn't have to repeat this code for each state.
    /*
    if(activeUIElement->hasFocus()) {
        mActiveUIElement = {};    
    } 
    else {
        mActiveUIElement = activeUIElement;
    }
    activeUIElement->toggleFocus();
    */
}

}