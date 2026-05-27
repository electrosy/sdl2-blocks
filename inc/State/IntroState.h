#include "BaseState.h"


#ifndef INTROSTATE_H
#define INTROSTATE_H

namespace ley {

class IntroState : public ley::BaseState {
public:

    IntroState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool resume();

    virtual std::string getStateID() const { return sIntroID; }

private:

    static const std::string sIntroID;

    ley::Sprite mLogo;
    bool lastLogo = false; //true when last logo has been displayed
    int mAnimationStep = 0; //keep track of the different animation steps

};

}

#endif