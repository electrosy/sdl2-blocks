#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

#include "GameState.h"
#include "StateIDs.h"
#include <vector>

namespace ley {

class GameStateMachine {

public:

    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();
    void update(ley::Command command);
    void render();
    StateID getStateId();
    bool isStateDone() const;
    ley::UIWidget* activeUIElement();
    void commitUI();
    void UI_ToggleFocus();
    GameState* back();

protected:

private:

    std::vector<GameState*> mGameStates;
};

}

#endif