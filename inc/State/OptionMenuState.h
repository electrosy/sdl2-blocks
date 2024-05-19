#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"
#include "../../UIElement.h"

#ifndef OPTIONMENUSTATE_H
#define OPTIONMENUSTATE_H

namespace ley {

class OptionMenuState : public ley::GameState {
public:

    OptionMenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();
    virtual void onCommandEnter();
    virtual void UI_ToggleFocus();

    virtual std::string getStateID() const { return sOptionMenuID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sOptionMenuID;

    ley::Sprite mBackground;

    Renderables mRenderables;
    Renderables mDebugRenderables;

    int optionItem; //Store the option selected from the options menu.

    ley::UIMenu optionUI;

    ley::Font mTextErrorMessage;
    ley::Font mTextEntryHelpMessage;

    ley::Timer mTextErrorTimer;

    std::string previousOptionsValue;

};

}

#endif