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
    virtual bool onReEnter();

    virtual std::string getStateID() const { return sPauseID; }

private:


    static const std::string sPauseID;

    ley::Sprite controlsSprite;
};

}

#endif