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

    virtual std::string getStateID() const { return sIntroID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sIntroID;

    Renderables mRenderables;
    Renderables mDebugRenderables;
};

}

#endif