#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"

#ifndef PAUSESTATE_H
#define PAUSESTATE_H

namespace ley {

class PauseState : public ley::GameState {

public:

    PauseState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sPauseID; }

private:

    Renderables mRenderables;
    Renderables mDebugRenderables;

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sPauseID;

    ley::Sprite controlsSprite;
};

}

#endif