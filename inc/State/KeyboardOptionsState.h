#include "GameState.h"

#include "../gfx/Video.h"
#include "../../inc/GameModel.h"
#include "../../inc/UI/UIMenu.h"

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
    bool mCaptureNewInput = false;
    bool mAddMapping = false;
    static const std::string sKeyboardOptionsID;
    ley::Font mDirectionsFont;
    ley::Font mTitleFont;
    ley::Font mButtonTitleFont;
    std::vector<ley::Font> mLabelFonts;
    std::vector<ley::Font> mValueFonts;
    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;
    ley::UIMenu mMainUI;
    int fontWidth(ley::Font* inFont);
    void reassignKey(std::string keycode, bool addMapping);
    void reassignKeyboard(ley::Command command, SDL_Scancode scancode, bool addMapping);
    std::vector<std::pair<SDL_Scancode, std::string>> findKeysByValue(const KeyBindingsType* bindings, const std::pair<SDL_Keymod, ley::Command>& targetValue);
    std::vector<KeyBindingsType::const_iterator> findIteratorsByValue(const KeyBindingsType* bindings,const std::pair<SDL_Keymod, ley::Command>& targetValue);
    void initalizeMenu();
};

}

#endif