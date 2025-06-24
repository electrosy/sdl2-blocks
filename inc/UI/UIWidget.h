#include <regex>

#include "../../Renderable.h"
#include "../../UIElement.h"
#include "../../Timer.h"

#ifndef UIWIDGET_H
#define UIWIDGET_H

namespace ley {

class UIWidget : public Renderable {

protected:
    static std::string mLastCharPressed;

public:
    virtual void render(SDL_Renderer* r, bool d) = 0;
    virtual void update() = 0; //handles update part of the lifecycle.
    virtual void processInput(std::string s) = 0;
    virtual void handleFocusChange(UIWidget** activeUIElement, std::string* previousValue) = 0;
    virtual void toggleFocus() = 0;
    virtual bool hasFocus()  = 0;
    virtual void onKeyDown(ley::Character c) = 0;
    virtual void onTextInput(const char* cstr) = 0;
    virtual std::string getRegEx() = 0;
    virtual void setPos(SDL_Point p) = 0;
    virtual void setCharSound(const std::function<void()> &func) = 0;
    virtual void setBackspaceSound(const std::function<void()> &func) = 0;
    virtual void setRegEx(std::string regEx)  = 0;
    virtual void setWidth(int width, int underlineWidth, int maxCharLength) = 0;

    std::string getLastChar(); // The most recently captured character from SDL_TEXTINPUT
};

}

#endif