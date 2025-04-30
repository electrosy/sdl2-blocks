#include "../inc/TextEntry.h"
#include "../inc/Screen.h"

ley::TextEntry::TextEntry()
:
pos{SCREEN_WCENTER - TEXTENTRY_WIDTH/2, SCREEN_HCENTER/3},
value{pos.x, pos.y, 0, 30},
cursor{pos.x, pos.y, 2, 25},
mUnderLine{pos.x, pos.y, UNDERLINE_WIDTH, 1},
mErrorTimer{2500, {0,0,0,0}},
mHelpFont{20,600,100,100}
{
    background.x = pos.x;
    background.y = pos.y;
    background.w = TEXTENTRY_WIDTH;
    background.h = TTF_FontHeight(value.getTTFFont());
    mErrorFont.updateMessage("Must be two numbers seperated by an 'x' between 8x8 and 25x22");
    mErrorFont.setVisible(false);
}

void ley::TextEntry::render(SDL_Renderer * r, bool d) {
    
    if(isVisible()) {

        SDL_SetRenderDrawColor(r, 125,125,125,50);
        //SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND); // TODO we don't need this because blendmode is set in video.
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

    mHelpFont.render(r, d);

    if(mErrorFont.isVisible()) {
        mErrorFont.render(r, d);
    }
} 

void ley::TextEntry::update() {
    mErrorTimer.runFrame(false, 0.0);
    mCursorFader.runFrame();
    
    if(mErrorTimer.hasExpired()) {
        mErrorFont.setVisible(false);
    }
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
        // help message is based on focus
        mHelpFont.updateMessage(getHelpMessage());
    }
    else {
        // help message is based on focus
        mHelpFont.updateMessage(getHelpMessage());
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

    background.w = mWidth;
    mUnderLine.w = mUnderlineWidth;

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

    if(value.getMessage().size() <= mMaxCharLength - 1) {
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
        return mFocusHelp;
    }
    else {
        return mNonFocusHelp;
    }
}

void ley::TextEntry::setHelpMessages(std::string focusHelp, std::string nonFocusHelp)  { 
    
    mFocusHelp = focusHelp;
    mNonFocusHelp = nonFocusHelp;
    mHelpFont.updateMessage(getHelpMessage());
}

void ley::TextEntry::setErrorMessage(std::string errorMessage) {

    mErrorFont.updateMessage(errorMessage);

}

void ley::TextEntry::handleFocusChange(TextEntry** activeUIElement, std::string* previousValue) {

    if(!hasFocus()){
        (*activeUIElement) = this;
        (*previousValue) = getTextBoxValue();
    }
    else {
        activeUIElement = {};
    }

    toggleFocus();


    /*
    if(!mLocalTextEntry.hasFocus()){
        mActiveUIElement = &mLocalTextEntry;
        mPreviousOptionsValue = mLocalTextEntry.getTextBoxValue();
    }
    else {
        mActiveUIElement = {};
    }

    mLocalTextEntry.toggleFocus();
    */
}