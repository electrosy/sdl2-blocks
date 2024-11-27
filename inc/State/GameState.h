#include "../../Input.h"
#include "../TextEntry.h"

#include <string>
#include <SDL2/SDL.h>

#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace ley {

class GameState {

protected:
    bool done = false;
    static TextEntry* mActiveUIElement; //TODO does this need to be static to be shared across all the child states?
public:
    virtual void update(ley::Command command) = 0;
    virtual void render() = 0;
    virtual void loadRenderables() = 0;
    
    virtual bool onEnter() = 0; //when the state is pushed on
    virtual bool onExit() = 0; //when the state is poped off
    virtual bool onReEnter() = 0; //rentering the state after the previous was popped off. //TODO this should probably be called resume
    virtual bool onPause() = 0; //When another state is pushed on after this one.
    ley::TextEntry* activeUIelement();
    virtual void UI_ToggleFocus(/*ley::TextEntry* activeUIelement*/);
    virtual void onCommandEnter();
    bool isDone(); // Returns true when everything in this state is finished.
    
    virtual std::string getStateID() const = 0;

    virtual ~GameState() {SDL_Log("Gamestate dtor()");};
};

}

#endif