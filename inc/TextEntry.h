#include <functional>
#include <regex>

#include <SDL2/SDL.h>

#include "../Font.h"
#include "../Input.h"
#include "Fader.h"
#include "../UIElement.h"


#ifndef TEXTENTRY_H
#define TEXTENTRY_H

namespace ley {

const auto TEXTENTRY_WIDTH = 731;
constexpr auto UNDERLINE_WIDTH = 321;
constexpr auto MAX_CHAR_LENGTH = 18;

enum class Character {
    backspace,enter,none
};

class TextEntry : public Renderable
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
    void render(SDL_Renderer* r, bool d);
    void update(); //handles update part of the lifecycle.
    void processInput(std::string s);
    std::string getTextBoxValue();
    void setTextBoxValue(std::string s);
    std::string* getTextBoxField();
    void toggleFocus();
    bool hasFocus() {return mHasFocus;};
    void setPos(SDL_Point p);
    void onKeyDown(ley::Character c);
    void onTextInput(const char* cstr);
    void adjustCursor();
    void setCharSound(const std::function<void()> &func);
    void setBackspaceSound(const std::function<void()> &func);
    std::string getHelpMessage();
    void setWidth(int width, int underlineWidth, int maxCharLength);
    ley::Font* getErrorFontPtr() { return &mErrorFont; };
    ley::Font* getHelpFontPtr() { return &mHelpFont; };
    std::string getRegEx() { return mRegEx; };
    void setRegEx(std::string regEx) { mRegEx = regEx; };
    ley::Timer* getErrorTimerPtr() { return &mErrorTimer; };
    void setHelpMessages(std::string focusHelp, std::string nonFocusHelp);
    void setErrorMessage(std::string errorMessage);
    void handleFocusChange(TextEntry** activeUIElement, std::string* previousValue);
};

}

#endif