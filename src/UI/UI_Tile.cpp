#include "../../inc/UI/UI_Tile.h"
#include "../../inc/Screen.h"

// TODO clean up all the left overs from copying from TextEntry.

typedef ley::Textures TextureManager;

ley::UI_Tile::UI_Tile()
:
mPosPx{SCREEN_WCENTER - UI_TILE_WIDTH /2, SCREEN_HCENTER/3},
value{mPosPx.x, mPosPx.y, 0, 30},
mErrorTimer{2500, {0,0,0,0}},
mHelpFont{1,SCREEN_HEIGHT-30,100,100}
{
    background.x = mPosPx.x;
    background.y = mPosPx.y;
    background.w = UI_TILE_WIDTH;
    background.h = TTF_FontHeight(value.getTTFFont());
    mErrorFont.updateMessage("Must be two numbers seperated by an 'x' between 8x8 and 25x22");
    mErrorFont.setVisible(false);
}

void ley::UI_Tile::render(SDL_Renderer * r, bool d) {
    
    if(isVisible()) {

        SDL_SetRenderDrawColor(r, 125,125,125,50);
        //SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND); // TODO we don't need this because blendmode is set in video.
        SDL_RenderFillRect(r, &background);
                
        SDL_Rect start_rect;
        start_rect = {0,0,BLOCKSIZE_PX,BLOCKSIZE_PX};
        if(mCurrentTextureName != "" && mCurrentTextureName != "O") {
            SDL_RenderCopy(r, TextureManager::Instance()->getTexture(mCurrentTextureName), &start_rect, &background);
        }

        // Draw rect around the widget by using the dimensions of the backgorund.   
        if(mHasFocus) {
            SDL_SetRenderDrawColor(r, CWHITE.r, CWHITE.g, CWHITE.b, CWHITE.a);
            SDL_RenderDrawRect(r, &background);
        }

        value.render(r, d);
    }

    mHelpFont.render(r, d);

    if(mErrorFont.isVisible()) {
        mErrorFont.render(r, d);
    }
} 

void ley::UI_Tile::update() {
    mErrorTimer.runFrame(false, 0.0);
    mCursorFader.runFrame();
    
    if(mErrorTimer.hasExpired()) {
        mErrorFont.setVisible(false);
    }
}

void ley::UI_Tile::processInput(std::string s) {

    if(s == "backspace") {
        mCurrentTextureName = "O";
        mBackspaceSound();
        
        return;
    }
    else if(s == "space") {
        SDL_Log("Space pressed for UI_Tile");
        return;
    }
}
std::string ley::UI_Tile::getTextBoxValue() {
    return value.getMessage();
}

void ley::UI_Tile::setTextBoxValue(std::string s) {
    value.updateMessage(s);
}

std::string* ley::UI_Tile::getTextBoxField() {
    return value.getMessagePtr();
}

void ley::UI_Tile::toggleFocus() {
    mHasFocus = !mHasFocus;
    if(mHasFocus) {
        SDL_StartTextInput();
        // help message is based on focus
        mHelpFont.updateMessage(getHelpMessage());
    }
    else {
        // help message is based on focus
        mHelpFont.updateMessage(getHelpMessage());
        SDL_StopTextInput();
    }
}

void ley::UI_Tile::setPos(SDL_Point p) {
    
    mPosPx = p;
    background.x = p.x;
    background.y = p.y;
    value.setPos({p.x,p.y});
}

void ley::UI_Tile::setWidth(int width, int underlineWidth, int maxCharLength) { 
    mWidth = width;
    mUnderlineWidth = underlineWidth;
    mMaxCharLength = maxCharLength;

    background.w = mWidth;
}

void ley::UI_Tile::onKeyDown(ley::Character c) {
    
    if(c == ley::Character::none) {
        return;
    }

    std::string character;

    switch(c) {
        
        case ley::Character::backspace : character = "backspace";
        break;

        case ley::Character::space : character = "space";
        break;

        default:
            break;

    }
    
    processInput(character);
}

void ley::UI_Tile::onTextInput(const char* cstr) {

    SDL_Log("onTextInput %s", cstr);
    if (std::regex_match(cstr, std::regex(getRegEx()))) {
    
        mCurrentTextureName = cstr;
        mLastCharPressed = cstr;
    }

    mEnterCharSound();
}

void ley::UI_Tile::setCharSound(const std::function<void()> &func) {
    mEnterCharSound = func;
}

void ley::UI_Tile::setBackspaceSound(const std::function<void()> &func) {
    mBackspaceSound = func;
}

std::string ley::UI_Tile::getHelpMessage() {
    if (hasFocus()) {
        return mFocusHelp;
    }
    else {
        return mNonFocusHelp;
    }
}

void ley::UI_Tile::setHelpMessages(std::string focusHelp, std::string nonFocusHelp)  { 
    
    mFocusHelp = focusHelp;
    mNonFocusHelp = nonFocusHelp;
    mHelpFont.updateMessage(getHelpMessage());
}

void ley::UI_Tile::setErrorMessage(std::string errorMessage) {

    mErrorFont.updateMessage(errorMessage);

}

void ley::UI_Tile::handleFocusChange(UIWidget** activeUIElement, std::string* previousValue) {

    if(!hasFocus()){
        (*activeUIElement) = this;
        (*previousValue) = getTextBoxValue();
    }
    else {
        activeUIElement = {};
    }

    toggleFocus();
}

void ley::UI_Tile::commit() {
    if(hasFocus()) {
        toggleFocus();
    }
}

void  ley::UI_Tile::setTextureName(std::string name) {
    mCurrentTextureName = name;
}