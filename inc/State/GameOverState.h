#include "GameState.h"

#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H


namespace ley {

class GameOverState : public ley::GameState {

public:
    GameOverState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sGameOverStateID; }

private:

    static const std::string sGameOverStateID;

};

}

#endif