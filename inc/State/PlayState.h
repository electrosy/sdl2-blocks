#include "GameState.h"

#include "../../Video.h"

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

namespace ley {

class PlayState : public ley::GameState {
public:

    PlayState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sPlayID; }

private:

    ley::Timer firstTimer;
    ley::Timer secondTimer; 
    ley::Timer thirdTimer; 
    ley::Timer fourthTimer;
    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sPlayID;
};

}

#endif