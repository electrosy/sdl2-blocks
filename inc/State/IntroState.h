#include "GameState.h"

#include "../../Video.h"

#ifndef INTROSTATE_H
#define INTROSTATE_H

namespace ley {

class IntroState : public ley::GameState {
public:

    IntroState(ley::Video * v);
    virtual void update();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sIntroID; }

private:

    ley::Video * videoSystem;
    static const std::string sIntroID;
};

}

#endif