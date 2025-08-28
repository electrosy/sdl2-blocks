#include "GameState.h"

#include "../gfx/Video.h"
#include "../../inc/GameModel.h"
#include "../../inc/UI/UIMenu.h"

#ifndef KEYBOARDOPTIONSSTATE_H
#define KEYBOARDOPTIONSSTATE_H

namespace ley {

class KeyboardOption : public ley::Renderable {
public:
    KeyboardOption(int x, int y, std::string label, std::string value);
    void render(SDL_Renderer * r, bool d);
    ley::Font* getLabelFontPtr() { return &mLabelFont; };
    ley::TextEntry* getTextEntryValuePtr() { return &mValueTextEntry; };
private:
    ley::Font mLabelFont;
    ley::TextEntry mValueTextEntry;
    SDL_Point mPos;    
};


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
    ley::Font mButtonTitleFont;
    ley::Font font_objects[20]; //Fonts use to display the label and value for keyboard and gamepad mapping
    std::vector<ley::Font*> mFonts; //Pointers to all the fonts.
    std::vector<ley::KeyboardOption> mKeyBoardOptions;
    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;
    ley::UIMenu mMainUI;
};

}

#endif