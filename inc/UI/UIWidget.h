#include "../../Renderable.h"

#ifndef UIWIDGET_H
#define UIWIDGET_H

namespace ley {

class UIWidget : public Renderable {

public:
    virtual void render(SDL_Renderer* r, bool d) = 0;
    virtual void update() = 0; //handles update part of the lifecycle.
    virtual void processInput(std::string s) = 0;
    virtual std::string getTextBoxValue() = 0;
    virtual void setTextBoxValue(std::string s) = 0;
    virtual std::string* getTextBoxField() = 0;
    virtual void toggleFocus() = 0;
    virtual bool hasFocus()  = 0;
    virtual void setPos(SDL_Point p) = 0;
    virtual void onKeyDown(ley::Character c) = 0;
    virtual void onTextInput(const char* cstr) = 0;
    virtual void adjustCursor() = 0;
    virtual void setCharSound(const std::function<void()> &func) = 0;
    virtual void setBackspaceSound(const std::function<void()> &func) = 0;
    virtual std::string getHelpMessage() = 0;
    virtual void setWidth(int width, int underlineWidth, int maxCharLength) = 0;
    virtual ley::Font* getErrorFontPtr()  = 0;
    virtual ley::Font* getHelpFontPtr()  = 0;
    virtual std::string getRegEx() = 0;
    virtual void setRegEx(std::string regEx)  = 0;
    virtual ley::Timer* getErrorTimerPtr() = 0;
    virtual void setHelpMessages(std::string focusHelp, std::string nonFocusHelp) = 0;
    virtual void setErrorMessage(std::string errorMessage) = 0;
    virtual void handleFocusChange(UIWidget** activeUIElement, std::string* previousValue) = 0;
    virtual void setWidthByChar(int maxCharLength) = 0;

};

}

#endif