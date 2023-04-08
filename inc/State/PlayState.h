#include "GameState.h"

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

namespace ley {

class PlayState : public ley::GameState {
public:

    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sPlayID; }

private:

    static const std::string sPlayID;
};

}

#endif