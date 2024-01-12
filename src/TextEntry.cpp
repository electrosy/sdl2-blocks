#include "../inc/TextEntry.h"

ley::TextEntry::TextEntry()
:
background{20,20,300,100},
label{20, 25, 150, 30},
value{20, 85, 0, 30} {

    label.updateMessage("Hello,");
}

void ley::TextEntry::render(SDL_Renderer * r, bool d) {
    
    SDL_SetRenderDrawColor(r, 125,125,125,50);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(r, &background);
    
    if(hasFocus) {
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDrawRect(r, &background);
    }
    
    label.render(r, d);
    value.render(r, d);
}

void ley::TextEntry::processInput(std::string s) {

    if(s == "backspace") {
        value.updateMessage(value.getMessage().substr(0, value.getMessage().size()-1));
        
        return;
    }
}
std::string* ley::TextEntry::getTextBoxField() {
    return value.getMessagePtr();
}

void ley::TextEntry::toggleFocus() {
    hasFocus = !hasFocus;
}