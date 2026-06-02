#ifndef OPTIONMENUSTATE_H
#define OPTIONMENUSTATE_H

#include "BaseState.h"
#include "../UI/UIMenu.h"

namespace ley {
class OptionMenuState : public ley::BaseState {
private:

    
    //labels
    ley::Font mBoardSizeLabelFont;
    ley::Font mDelayLabelFont;
    ley::Font mRepeatLabelFont;
    ley::Font mGuideGridOnLabelFont;
    ley::Font mWallKickOnLabelFont;
    ley::Font mDropCoolDownLabelFont;
    ley::Font mShowProgressBarLabelFont;
    ley::Font mStartLevelLabelFont;
    //text entries
    ley::TextEntry mBoardSizeTextEntry;
    ley::TextEntry mKeyDelayTextEntry;
    ley::TextEntry mKeyRepeatTextEntry;
    ley::TextEntry mGuideGridOnTextEntry;
    ley::TextEntry mWallKickOnTextEntry;
    ley::TextEntry mDropCoolDownTextEntry;
    ley::TextEntry mShowProgressBarTextEntry;
    ley::TextEntry mStartLevelTextEntry;
    std::vector<ley::TextEntry*> mTextEntries;

    ley::Sprite mBackground;
    int optionItem; //Store the option selected from the options menu.
    ley::UIMenu mOptionUI;
    std::string mPreviousOptionsValue;
    std::string mPreviousKeyDelayValue;
    std::string mPreviousKeyRepeatValue;
    std::string mPreviousGuideGridOnValue;
    std::string mPreviousWallKickOnValue;
    std::string mPreviousDropCoolDownValue;
    std::string mPreviousShowProgressBarValue;
    std::string mPreviousStartLevelValue;
    void positionOptionsLabels();
    
public:

    OptionMenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();
    virtual bool onExit()  override;
    virtual bool onEnter() override;
    virtual bool resume() override;
    virtual StateID getStateID() const { return StateID::OptionMenu; }
    
    void initTextEntryMessages();
    void commitBoardSize();
    void commitKeyDelay();
    void commitKeyRepeat();
    void commitGuideGridOn();
    void commitWallKickOn();
    void commitHardDropCoolDown();
    void commitShowProgressBar();
    void commitStartLevel();
    void initStartLevelTextEntry();
    void initTextEntry(
        ley::TextEntry& entry,
        bool visible,
        std::function<void()> charSound,
        std::function<void()> backspaceSound,
        int width,
        SDL_Point pos,
        const std::string& regex,
        const std::string& errorMsg,
        const std::string& helpMsg
    );

};

}

#endif