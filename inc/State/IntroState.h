#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"

#ifndef INTROSTATE_H
#define INTROSTATE_H

namespace ley {

class IntroState : public ley::GameState {
public:

    IntroState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sIntroID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sIntroID;

    ley::Sprite mLogo;
    bool lastLogo = false; //true when last logo has been displayed
    int mAnimationStep = 0; //keep track of the different animation steps

    Renderables mRenderables;
    Renderables mDebugRenderables;
};

}

#endif