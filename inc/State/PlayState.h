#include "GameState.h"

#include "../../Video.h"
#include "../../inc/GameModel.h"

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

auto constexpr STATUSMESSAGE_POS_X_PX = 150;
auto constexpr STATUSMESSAGE_POS_Y_PX = 10;

namespace ley {

class PlayState : public ley::GameState {

public:

    PlayState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sPlayID; };

private:

    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;

    //Font
    ley::Font mStatusFont;

    //Timers
    ley::Timer fallTimer; //Time to force the blockdown

    ley::Timer statusTimer;
    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sPlayID;

    void resetGame();
};

}

#endif