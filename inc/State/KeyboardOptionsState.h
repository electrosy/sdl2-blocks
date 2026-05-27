#include "BaseState.h"

#include "../../inc/UI/UIMenu.h"

#ifndef KEYBOARDOPTIONSSTATE_H
#define KEYBOARDOPTIONSSTATE_H

namespace ley {

class KeyboardOptionsState : public ley::BaseState {
public:

    KeyboardOptionsState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onExit()  override;
    virtual bool onEnter();
    virtual bool resume();

    virtual std::string getStateID() const { return sKeyboardOptionsID; }

private:

    bool mCaptureNewInput = false;
    bool mAddMapping = false;
    static const std::string sKeyboardOptionsID;
    ley::Font mDirectionsFont;
    ley::Font mTitleFont;
    ley::Font mButtonTitleFont;
    std::vector<ley::Font> mLabelFonts;
    std::vector<ley::Font> mValueFonts;
    ley::UIMenu mMainUI;
    int fontWidth(ley::Font* inFont);
    void reassignKeyButton(std::string keycode, bool addMapping);
    void reassignKeyboard(ley::Command command, SDL_Scancode scancode, bool addMapping);
    void reassignButton(ley::Command command, SDL_GameControllerButton button, bool addMapping);
    std::vector<std::pair<SDL_Scancode, std::string>> findKeysByValue(const KeyBindingsType* bindings, const std::pair<SDL_Keymod, ley::Command>& targetValue);
    std::vector<KeyBindingsType::const_iterator> findIteratorsByValue(const KeyBindingsType* bindings,const std::pair<SDL_Keymod, ley::Command>& targetValue);
    std::vector<PadBindingsType::const_iterator> findButtonIteratorsByValue(const PadBindingsType* bindings,const ley::Command& targetValue);
    void initalizeMenu();
};

}

#endif