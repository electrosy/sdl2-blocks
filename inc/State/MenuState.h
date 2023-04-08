#include "GameState.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H

namespace ley {

class MenuState : public ley::GameState {
public:

    virtual void update();
    virtual void render();

    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const { return sMenuID; }

private:

    static const std::string sMenuID;
};

}

#endif