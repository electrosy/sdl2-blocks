#include "../../inc/UI/UI_Tile.h"
#include "../../inc/Screen.h"

// TODO clean up all the left overs from copying from TextEntry.

typedef ley::Textures TextureManager;

ley::UI_Tile::UI_Tile()
:
mPosPx{SCREEN_WCENTER - UI_TILE_WIDTH /2, SCREEN_HCENTER/3}
{
    background.x = mPosPx.x;
    background.y = mPosPx.y;
    background.w = BLOCKSIZE_PX;
    // TODO this shouldn't be set based on the font.
    background.h = BLOCKSIZE_PX - 1;
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
    }
} 

void ley::UI_Tile::update() {
    mCursorFader.runFrame();
    
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
/*
std::string ley::UI_Tile::getTextBoxValue() {
    return value.getMessage();
}

void ley::UI_Tile::setTextBoxValue(std::string s) {
    value.updateMessage(s);
}

std::string* ley::UI_Tile::getTextBoxField() {
    return value.getMessagePtr();
}
*/

void ley::UI_Tile::toggleFocus() {
    mHasFocus = !mHasFocus;
    if(mHasFocus) {
        SDL_StartTextInput();
    }
    else {
        SDL_StopTextInput();
    }
}

void ley::UI_Tile::setPos(SDL_Point p) {
    
    mPosPx = p;
    background.x = p.x;
    background.y = p.y;
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

/*
std::string ley::UI_Tile::getHelpMessage() {
    if (hasFocus()) {
        return mFocusHelp;
    }
    else {
        return mNonFocusHelp;
    }
}
*/

/*
void ley::UI_Tile::setHelpMessages(std::string focusHelp, std::string nonFocusHelp)  { 
    
    mFocusHelp = focusHelp;
    mNonFocusHelp = nonFocusHelp;    
}
*/

void ley::UI_Tile::handleFocusChange(UIWidget** activeUIElement, std::string* previousValue) {

    if(!hasFocus()){
        (*activeUIElement) = this;
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