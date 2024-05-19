#include "GameState.h"

#include <vector>

#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

namespace ley {

class GameStateMachine {

public:

    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();
    void update(ley::Command command);
    void render();
    std::string getStateId();
    bool isStateDone();
    ley::TextEntry* textEntry();
    void commitUI();
    void UI_ToggleFocus();

private:

    std::vector<GameState*> mGameStates;
};

}

#endif