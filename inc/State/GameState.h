#include <string>
#include <SDL2/SDL.h>

#include "../../Input.h"
#include "../../Video.h"
#include "../../GameModel.h"

#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace ley {

class GameState {

public:
    virtual void update(ley::Command command) = 0;
    virtual void render() = 0;
    virtual void loadRenderables() = 0;
    
    virtual bool onEnter() = 0; //when the state is pushed on
    virtual bool onExit() = 0; //when the state is poped off
    virtual bool onReEnter() = 0; //rentering the state after the previous was popped off.

    virtual std::string getStateID() const = 0;
};

}

#endif