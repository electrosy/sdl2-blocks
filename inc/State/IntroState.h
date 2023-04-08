#include "GameState.h"

#ifndef INTROSTATE_H
#define INTROSTATE_H

namespace ley {

class IntroState : public ley::GameState {
public:

    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sIntroID; }

private:

    static const std::string sIntroID;
};

}

#endif