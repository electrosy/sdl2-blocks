#include <SDL2/SDL.h>
#include "../Font.h"

namespace ley {

class TextEntry : public Renderable
{

private:
    SDL_Point pos;
    SDL_Rect background;
    Font value;
    bool visible = true;
    bool hasFocus = false;

public:
    TextEntry();
    void render(SDL_Renderer* r, bool d);
    void processInput(std::string s);
    std::string getTextBoxValue();
    std::string* getTextBoxField();
    void toggleFocus();
    void setPos(SDL_Point p);
};
}