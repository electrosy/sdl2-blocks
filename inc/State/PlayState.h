#include "BaseState.h"


#ifndef PLAYSTATE_H
#define PLAYSTATE_H

auto constexpr STATUSMESSAGE_POS_X_PX = 150;
auto constexpr STATUSMESSAGE_POS_Y_PX = 10;

namespace ley {

class PlayState : public ley::BaseState {

public:

    PlayState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onReEnter();

    virtual std::string getStateID() const { return sPlayID; };

private:
    ley::Timer mLastHardDrop; //Time that has expired since last hard drop

    //Font
    ley::Font mStatusFont;

    //Timers
    ley::Timer mFallTimer; //Time to force the blockdown

    ley::Timer mStatusTimer;
    static const std::string sPlayID;

    void resetGame();
};

}

#endif