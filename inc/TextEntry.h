#include <functional>
#include <regex>

#include <SDL2/SDL.h>

#include "./gfx/Font.h"
#include "Input.h" // TODO circurlar Input.h has TextEntry and TextEntry also has Input
#include "Fader.h"
#include "./UI/UIElement.h" // TODO this is also in TextEntry.h
#include "./UI/UIWidget.h"


#ifndef TEXTENTRY_H
#define TEXTENTRY_H

namespace ley {

const auto TEXTENTRY_WIDTH = 731;
constexpr auto UNDERLINE_WIDTH = 321;
constexpr auto MAX_CHAR_LENGTH = 18;

// TODO TextEntry goes into the UI folder
class TextEntry : public UIWidget
{

private:
    int mWidth = TEXTENTRY_WIDTH;
    int mUnderlineWidth = UNDERLINE_WIDTH;
    int mMaxCharLength = MAX_CHAR_LENGTH;
    SDL_Point pos; //Position of TextEntry field top/left.
    SDL_Rect background;
    SDL_Rect cursor;
    SDL_Rect mUnderLine;
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
    
public:
    TextEntry();
    void render(SDL_Renderer* r, bool d) override;
    void update() override; //handles update part of the lifecycle.
    void processInput(std::string s) override;
    void toggleFocus() override;
    bool hasFocus() override {return mHasFocus;};
    void setPos(SDL_Point p) override;
    void onKeyDown(ley::Character c) override;
    void onTextInput(const char* cstr) override;
    void adjustCursor();
    void setCharSound(const std::function<void()> &func) override;
    void setBackspaceSound(const std::function<void()> &func) override;
    void setWidth(int width, int underlineWidth, int maxCharLength) override;
    ley::Font* getHelpFontPtr() { return &mHelpFont; };
    std::string getRegEx() override { return mRegEx; };
    void setRegEx(std::string regEx) override { mRegEx = regEx; };
    void handleFocusChange(UIWidget** activeUIElement, std::string* previousValue) override;
    void setWidthByChar(int maxCharLength);

    std::string getTextBoxValue();
    void setTextBoxValue(std::string s);
    std::string* getTextBoxField();
    std::string getHelpMessage();
    void setHelpMessages(std::string focusHelp, std::string nonFocusHelp);
    ley::Timer* getErrorTimerPtr() { return &mErrorTimer; };
    ley::Font* getErrorFontPtr() { return &mErrorFont; };
    void setErrorMessage(std::string errorMessage);
};

}

#endif