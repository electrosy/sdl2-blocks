#include <functional>

#include <SDL2/SDL.h>

#include "../Font.h"
#include "../Input.h"
#include "Fader.h"


#ifndef TEXTENTRY_H
#define TEXTENTRY_H

namespace ley {

enum class Character {
    backspace,enter,none
};

class TextEntry : public Renderable
{

private:
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

public:
    TextEntry();
    void render(SDL_Renderer* r, bool d);
    void processInput(std::string s);
    std::string getTextBoxValue();
    std::string* getTextBoxField();
    void toggleFocus();
    bool hasFocus() {return mHasFocus;};
    void setPos(SDL_Point p);
    void onKeyDown(ley::Character c);
    void onTextInput(const char* cstr);
    void adjustCursor();
    void setCharSound(const std::function<void()> &func);
    void setBackspaceSound(const std::function<void()> &func);
};

}

#endif