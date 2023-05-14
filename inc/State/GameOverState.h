#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"

#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H


namespace ley {

class GameOverState : public ley::GameState {

public:
    GameOverState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();

    virtual std::string getStateID() const { return sGameOverStateID; }

private:

    static const std::string sGameOverStateID;

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;

    Renderables mRenderables;
    Renderables mDebugRenderables;
};

}

#endif