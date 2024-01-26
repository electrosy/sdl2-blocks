#include <SDL2/SDL.h>
#include "../Font.h"
#include "../Input.h"

#ifndef TEXTENTRY_H
#define TEXTENTRY_H

namespace ley {

class TextEntry : public Renderable
{

private:
    SDL_Point pos;
    SDL_Rect background;
    Font value;
    bool visible = true;
    bool mHasFocus = false;

public:
    TextEntry();
    void render(SDL_Renderer* r, bool d);
    void processInput(std::string s);
    std::string getTextBoxValue();
    std::string* getTextBoxField();
    void toggleFocus();
    bool hasFocus() {return mHasFocus;};
    void setPos(SDL_Point p);
    void processTextBox(ley::Character c);
};

}

#endif