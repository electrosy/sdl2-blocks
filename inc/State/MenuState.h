#include "GameState.h"

#include "../../Video.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H

namespace ley {

class MenuState : public ley::GameState {
public:

    MenuState(ley::Video * v);
    virtual void update(ley::Command command, ley::GameModel * gm);
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sMenuID; }

private:

    ley::Video * videoSystem;
    static const std::string sMenuID;
};

}

#endif