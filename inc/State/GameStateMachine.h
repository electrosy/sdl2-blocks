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
    void update();
    void loadRenderables();

private:

    std::vector<GameState*> mGameStates;
};

}

#endif