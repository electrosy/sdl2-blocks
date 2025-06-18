#include <functional>
#include <regex>

#include <SDL2/SDL.h>

#include "../../Font.h"
#include "../../Input.h"
#include "../Fader.h"
#include "../../UIElement.h"
#include "../../Textures.h"
#include "../../inc/UI/UIWidget.h"


#ifndef UI_TILE_H
#define UI_TILE_H

namespace ley {

const auto UI_TILE_WIDTH = 731;
constexpr auto TILE_UNDERLINE_WIDTH = 321;
constexpr auto TILE_MAX_CHAR_LENGTH = 18;

class UI_Tile : public UIWidget
{

private:
    std::string mCurrentTextureName = "d";
    int mWidth = UI_TILE_WIDTH;
    int mUnderlineWidth = TILE_UNDERLINE_WIDTH;
    int mMaxCharLength = TILE_MAX_CHAR_LENGTH;
    SDL_Point pos; //Position of UI_Tile field top/left.
    SDL_Rect background;
    ley::Fader mCursorFader;
    Font value;
    bool visible = true;
    bool mHasFocus = false;
    std::function<void()> mEnterCharSound;
    std::function<void()> mBackspaceSound;
    ley::Font mErrorFont;
    ley::Font mHelpFont;
    std::string mFocusHelp;
    std::string mNonFocusHelp;
    std::string mRegEx;
    ley::Timer mErrorTimer;
    std::string mPreviousValue;
    
public:
    UI_Tile();
    void render(SDL_Renderer* r, bool d) override;
    void update() override; //handles update part of the lifecycle.
    void processInput(std::string s) override;
    std::string getTextBoxValue() override;
    void setTextBoxValue(std::string s) override;
    std::string* getTextBoxField() override;
    void toggleFocus() override;
    bool hasFocus() override {return mHasFocus;};
    void setPos(SDL_Point p) override;
    void onKeyDown(ley::Character c) override;
    void onTextInput(const char* cstr) override;
    void setCharSound(const std::function<void()> &func) override;
    void setBackspaceSound(const std::function<void()> &func) override;
    std::string getHelpMessage() override;
    void setWidth(int width, int underlineWidth, int maxCharLength) override;
    ley::Font* getErrorFontPtr() override { return &mErrorFont; };
    ley::Font* getHelpFontPtr() override { return &mHelpFont; };
    std::string getRegEx() override { return mRegEx; };
    void setRegEx(std::string regEx) override { mRegEx = regEx; };
    ley::Timer* getErrorTimerPtr() override { return &mErrorTimer; };
    void setHelpMessages(std::string focusHelp, std::string nonFocusHelp) override;
    void setErrorMessage(std::string errorMessage) override;
    void handleFocusChange(UIWidget** activeUIElement, std::string* previousValue) override;
    void setWidthByChar(int maxCharLength) override;
    std::string* getPreviousValuePtr() {return &mPreviousValue;};
    
    void commit();
};

}

#endif