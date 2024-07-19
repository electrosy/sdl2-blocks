#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"

#ifndef KEYBOARDOPTIONSSTATE_H
#define KEYBOARDOPTIONSSTATE_H

namespace ley {

class KeyboardOptionsState : public ley::GameState {
public:

    KeyboardOptionsState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sKeyboardOptionsID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sKeyboardOptionsID;

    ley::Font mTitleFont;
    ley::Font font_objects[1];
    std::vector<ley::Font*> fonts;

    Renderables mRenderables;
    Renderables mDebugRenderables;
};

}

#endif