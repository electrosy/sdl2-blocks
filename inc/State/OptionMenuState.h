#include "GameState.h"

#include "../../Video.h"
#include "../../inc/GameModel.h"
#include "../UI/UIMenu.h"

#ifndef OPTIONMENUSTATE_H
#define OPTIONMENUSTATE_H

namespace ley {

class OptionMenuState : public ley::GameState {

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sOptionMenuID;
    
    //labels
    ley::Font mBoardSizeLabelFont;
    ley::Font mDelayLabelFont;
    ley::Font mRepeatLabelFont;
    ley::Font mGuideGridOnLabelFont;
    ley::Font mWallKickOnLabelFont;
    //text entries
    ley::TextEntry mBoardSizeTextEntry;
    ley::TextEntry mKeyDelayTextEntry;
    ley::TextEntry mKeyRepeatTextEntry;
    ley::TextEntry mGuideGridOnTextEntry;
    ley::TextEntry mWallKickOnTextEntry;

    ley::Sprite mBackground;
    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;
    int optionItem; //Store the option selected from the options menu.
    ley::UIMenu mOptionUI;
    std::string mPreviousOptionsValue;
    std::string mPreviousKeyDelayValue;
    std::string mPreviousKeyRepeatValue;
    std::string mPreviousGuideGridOnValue;
    std::string mPreviousWallKickOnValue;
    void positionOptionsLabels();
    
public:

    OptionMenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();
    virtual bool onEnter() override;
    virtual bool onExit() override;
    virtual bool onReEnter() override;
    virtual bool onPause() override;
    virtual std::string getStateID() const { return sOptionMenuID; }
    
    void initTextEntryMessages();
    void commitBoardSize();
    void commitKeyDelay();
    void commitKeyRepeat();
    void commitGuideGridOn();
    void commitWallKickOn();

};

}

#endif