#include "GameState.h"

#include "../../Video.h"

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

namespace ley {

class PlayState : public ley::GameState {
public:

    PlayState(ley::Video * v);
    virtual void update();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sPlayID; }

private:

    ley::Timer firstTimer;
    ley::Timer secondTimer; 
    ley::Timer thirdTimer; 
    ley::Timer fourthTimer;
    ley::Video * videoSystem;
    static const std::string sPlayID;
};

}

#endif