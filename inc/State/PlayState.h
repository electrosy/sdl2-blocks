#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

auto constexpr VOLUME_POS_X_PX = 500;
auto constexpr VOLUME_POS_Y_PX = 10;

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

//    int blockFallSpeed = 1000; //how many milisecond to make the block fall // not used

    Renderables mRenderables;
    Renderables mDebugRenderables;

    //Font
    ley::Font statusFont;

    //Timers
    ley::Timer fallTimer; //Time to force the blockdown
    ley::Timer firstTimer;
    ley::Timer secondTimer; 
    ley::Timer thirdTimer; 
    ley::Timer fourthTimer;
    ley::Timer statusTimer;
    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sPlayID;

    void resetGame();
};

}

#endif