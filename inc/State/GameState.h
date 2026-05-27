#include "../Input.h"
#include "../../inc/UI/UIWidget.h"
#include "../UI/TextEntry.h"
#include "StateIDs.h"

#include <string>
#include <SDL2/SDL.h>

#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace ley {

// Design note: states only hold a GameModel* (not a GameController*) by design.
// GameController owns the state machine and dispatches Commands to the active state;
// states read/mutate model data and signal intent via GameModel::stateChange().
// This keeps the state hierarchy decoupled from the controller.

class GameState {

private:

protected:
    bool done = false;
    static UIWidget* mActiveUIElement;
    static std::string mCurrentInputContext;
    
public:
    virtual void update(ley::Command command) = 0;
    virtual void render() = 0;
    virtual void loadRenderables() = 0;
    
    virtual bool onEnter() = 0; //when the state is pushed on
    virtual bool onExit() = 0; //when the state is poped off
    virtual bool resume() = 0; // called when the state below this one becomes active again (the state on top was popped)
    virtual bool onPause() = 0; //When another state is pushed on after this one.
    ley::UIWidget* activeUIelement();
    virtual void UI_ToggleFocus();
    virtual void commitUI();
    bool isDone(); // Returns true when everything in this state is finished.
    virtual StateID getStateID() const = 0;
    virtual ~GameState() {SDL_Log("Gamestate dtor()");};
    std::string currentInputContext();
};

}

#endif