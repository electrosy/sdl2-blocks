#include "GameState.h"

#include "../../Video.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H

namespace ley {

class MenuState : public ley::GameState {
public:

    MenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();

    virtual std::string getStateID() const { return sMenuID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sMenuID;

    Renderables mRenderables;
    Renderables mDebugRenderables;
};

}

#endif