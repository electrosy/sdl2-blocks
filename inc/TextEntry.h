#include <SDL2/SDL.h>
#include "../Font.h"

namespace ley {

class TextEntry : public Renderable
{

private:
    SDL_Rect background;
    Font label;
    Font value;
    bool visible = true;
    bool hasFocus = false;

public:
    TextEntry();
    void render(SDL_Renderer* r, bool d);
    void processInput(std::string s);
    std::string* getTextBoxField();
    void toggleFocus();
};
}