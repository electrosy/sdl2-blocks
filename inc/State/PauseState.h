#include "BaseState.h"


#ifndef PAUSESTATE_H
#define PAUSESTATE_H

namespace ley {

class PauseState : public ley::BaseState {

public:

    PauseState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool resume();

    virtual StateID getStateID() const { return StateID::Pause; }

private:



    ley::Sprite controlsSprite;
};

}

#endif