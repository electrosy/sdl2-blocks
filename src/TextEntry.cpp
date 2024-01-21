#include "../inc/TextEntry.h"
#include "../inc/Screen.h"

const auto TEXTENTRY_WIDTH = 300;

ley::TextEntry::TextEntry()
:
pos{SCREEN_WCENTER - TEXTENTRY_WIDTH/2, SCREEN_HCENTER/3},
value{pos.x, pos.y, 0, 30}
{
    background.x = pos.x;
    background.y = pos.y;
    background.w = TEXTENTRY_WIDTH;
    background.h = TTF_FontHeight(value.getTTFFont());
}

void ley::TextEntry::render(SDL_Renderer * r, bool d) {
    
    if(isVisible()) {

        SDL_SetRenderDrawColor(r, 125,125,125,50);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(r, &background);
        
        if(hasFocus) {
            SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            SDL_RenderDrawRect(r, &background);
        }

        value.render(r, d);
    }
}

void ley::TextEntry::processInput(std::string s) {

    if(s == "backspace") {
        value.updateMessage(value.getMessage().substr(0, value.getMessage().size()-1));
        
        return;
    }
}
std::string ley::TextEntry::getTextBoxValue() {
    return value.getMessage();
}

std::string* ley::TextEntry::getTextBoxField() {
    return value.getMessagePtr();
}

void ley::TextEntry::toggleFocus() {
    hasFocus = !hasFocus;
}

void ley::TextEntry::setPos(SDL_Point p) {
    
    background.x = p.x;
    background.y = p.y;
    value.setPos({p.x,p.y});
}