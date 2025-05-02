#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"
#include "../../UIElement.h"

#ifndef OPTIONMENUSTATE_H
#define OPTIONMENUSTATE_H

namespace ley {

class OptionMenuState : public ley::GameState {

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sOptionMenuID;
    ley::Font mBoardSizeLabelFont;
    ley::Font mDelayLabelFont;
    ley::Font mRepeatLabelFont;
    ley::TextEntry mLocalTextEntry;
    ley::TextEntry mKeyDelayTextEntry;
    ley::TextEntry mKeyRepeatTextEntry;
    ley::Sprite mBackground;
    Renderables mRenderables;
    Renderables mDebugRenderables;
    int optionItem; //Store the option selected from the options menu.
    ley::UIMenu mOptionUI;
    std::string mPreviousOptionsValue;
    std::string mPreviousKeyDelayValue;
    std::string mPreviousKeyRepeatValue;
    
public:

    OptionMenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();
    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();
    virtual void commitBoardSize();
    virtual void commitKeyDelay();
    virtual void commitKeyRepeat();
    virtual std::string getStateID() const { return sOptionMenuID; }

};

}

#endif