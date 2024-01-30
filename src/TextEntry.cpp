#include "../inc/TextEntry.h"
#include "../inc/Screen.h"

const auto TEXTENTRY_WIDTH = 300;

ley::TextEntry::TextEntry()
:
pos{SCREEN_WCENTER - TEXTENTRY_WIDTH/2, SCREEN_HCENTER/3},
value{pos.x, pos.y, 0, 30},
cursor{pos.x, pos.y, 2, 25}
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
        
        if(mHasFocus) {
            SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            SDL_RenderDrawRect(r, &background);

            //draw cursor
            SDL_RenderFillRect(r, &cursor);

            //draw cursor higlight
            SDL_SetRenderDrawColor(r, 238, 50, 83, mCursorFader.alpha());
            SDL_RenderFillRect(r, &cursor);
        }

        value.render(r, d);
    }

    mCursorFader.runFrame();
}

void ley::TextEntry::processInput(std::string s) {

    value.size();

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
    mHasFocus = !mHasFocus;
}

void ley::TextEntry::setPos(SDL_Point p) {
    
    pos = p;
    background.x = p.x;
    background.y = p.y;
    value.setPos({p.x,p.y});
    adjustCursor();
}

void ley::TextEntry::onKeyDown(ley::Character c) {
    
    std::string character;

    switch(c) {
        
        case ley::Character::backspace : character = "backspace";
        break;

        default:
            break;

    }
    
    processInput(character);
    adjustCursor();
}

void ley::TextEntry::onTextInput(const char* cstr) {

    if(value.getMessagePtr()->size() < 19) {
                    
        value.getMessagePtr()->append(cstr);
    }

    adjustCursor();

    SDL_Log("onTextInput %s", cstr);
}

void ley::TextEntry::adjustCursor() {
    std::pair<int, int> size = value.size();
    cursor.x = pos.x + size.first;
    cursor.y = pos.y + 2;
}