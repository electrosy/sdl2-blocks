#include "BaseState.h"


#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H


namespace ley {

class GameOverState : public ley::BaseState {

public:
    GameOverState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool resume();

    virtual StateID getStateID() const { return StateID::GameOver; }

private:


    ley::Font fontGameOver; //Game over font


};

}

#endif