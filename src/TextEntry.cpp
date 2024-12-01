#include "../inc/TextEntry.h"
#include "../inc/Screen.h"

ley::TextEntry::TextEntry()
:
pos{SCREEN_WCENTER - TEXTENTRY_WIDTH/2, SCREEN_HCENTER/3},
value{pos.x, pos.y, 0, 30},
cursor{pos.x, pos.y, 2, 25},
mUnderLine{pos.x, pos.y, UNDERLINE_WIDTH, 1}
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

            //draw underline
            SDL_RenderFillRect(r, &mUnderLine);

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

    if(s == "backspace") {
        value.updateMessage(value.getMessage().substr(0, value.getMessage().size()-1));
        mBackspaceSound();
        
        return;
    }
}
std::string ley::TextEntry::getTextBoxValue() {
    return value.getMessage();
}

void ley::TextEntry::setTextBoxValue(std::string s) {
    value.updateMessage(s);
}

std::string* ley::TextEntry::getTextBoxField() {
    return value.getMessagePtr();
}

void ley::TextEntry::toggleFocus() {
    mHasFocus = !mHasFocus;
    if(mHasFocus) {
        SDL_StartTextInput();
        adjustCursor();
    }
    else {
        SDL_StopTextInput();
    }
}

void ley::TextEntry::setPos(SDL_Point p) {
    
    pos = p;
    background.x = p.x;
    background.y = p.y;
    mUnderLine.x = p.x;
    mUnderLine.y = p.y + 25;
    value.setPos({p.x,p.y});
    adjustCursor();
}

void ley::TextEntry::setWidth(int width, int underlineWidth, int maxCharLength) { 
    mWidth = width;
    mUnderlineWidth = underlineWidth;
    mMaxCharLength = maxCharLength;

}

void ley::TextEntry::onKeyDown(ley::Character c) {
    
    if(c == ley::Character::none) {
        return;
    }

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

    if(value.getMessage().size() <= MAX_CHAR_LENGTH) {
        value.updateMessage(value.getMessage() + cstr);
    }

    adjustCursor();

    SDL_Log("onTextInput %s", cstr);
    mEnterCharSound();
}

void ley::TextEntry::adjustCursor() {
    std::pair<int, int> size = value.size();
    cursor.x = pos.x + size.first;
    cursor.y = pos.y + 2;
}

void ley::TextEntry::setCharSound(const std::function<void()> &func) {
    mEnterCharSound = func;
}

void ley::TextEntry::setBackspaceSound(const std::function<void()> &func) {
    mBackspaceSound = func;
}

std::string ley::TextEntry::getHelpMessage() {
    if (hasFocus()) {
        return "Enter a number between 8x8 - 25x22, e.g. \"10x20\" and press down." ;
    }
    else {
        return "Scroll here to modify field.";
    }
}