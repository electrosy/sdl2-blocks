/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h"

ley::Sprite::Sprite()
:
Renderable(),
fader(1000,{0,0,0,0}),
texture{nullptr} {
    SDL_Log("Sprite() called");
}

ley::Sprite::Sprite(SDL_Texture * t, unsigned int s, std::vector<SDL_Rect> v, std::pair<int, SDL_Rect> f)
: 
Renderable(),
animSpeed(s),
texture(t),
fader{f.first,f.second} {

    SDL_Log("Sprite(SDL_Texture * t, unsigned int s, std::vector<SDL_Rect> v) called");

    if (!texture) {
        return;//EARLY EXIT
    }
    
    SDL_QueryTexture(
            texture,
            nullptr,
            nullptr,
            &mWidth,
            &mHeight);

    bool multiframe = false;
    
    fader.pause(true); //start paused and resume on the first render
    if(v.size() > 0) {
        fader(f.first,{v.at(0)}); // fader is a Timer
    }
    else {
        fader(f.first,{0,0,mWidth,mHeight});
    }

    multiframe = v.size() > 1;
    
    SDL_Rect source_rect;
    dest_rect.y = source_rect.y = 0;
    if(multiframe) {
        for(auto rect : v) {
            source_rect.x = rect.x;
            source_rect.y = rect.y;
            source_rect.w = dest_rect.w = rect.w;
            source_rect.h = dest_rect.h = rect.h;
            frames.push_back(source_rect);
        }

        //Set width and height of multiframe sprites to last frame. NOTE may cause unexpected behavior if frame sizes are different
        mWidth = source_rect.w; 
        mHeight = source_rect.h;
    } else {
        dest_rect.w = mWidth;
        dest_rect.h = mHeight;
        dest_rect.x = source_rect.x = 0;
    }
}

ley::Sprite::~Sprite() {

    SDL_Log("Sprite::~Sprite() dtor called");
}

/* Accessors */
void ley::Sprite::center() {
    SDL_Rect tempRect;
    //Query for width and heigh of texture.

    tempRect.x = SCREEN_WCENTER - (mWidth*mScale / 2);
    tempRect.y = SCREEN_HCENTER - (mHeight*mScale / 2);

    setPos(tempRect.x,tempRect.y);
}

void ley::Sprite::bottomLeft() {
    setPos(0, SCREEN_HEIGHT - mHeight*mScale);
}

void ley::Sprite::bottomRight() {
    setPos(SCREEN_WIDTH - mWidth*mScale, SCREEN_HEIGHT - mHeight*mScale);
}

void ley::Sprite::scale(double s) {
    mScale = s;
}

void ley::Sprite::setPos(unsigned int x, unsigned int y) {
    pos.first = x; pos.second = y;
}
void ley::Sprite::resetFader() {
    startFader = false;
    fader.reset();
    fader.pause(true);
}
void ley::Sprite::holdFader() {
    mFaderControl = 2;
}

void ley::Sprite::reverseFader() {
    mFaderControl = 1;
}

bool ley::Sprite::faderFinished() {

    return fader.hasExpired();
}

bool ley::Sprite::faderFinishedMessage() {
    return fader.expiredMessage();
}

void ley::Sprite::fadeTime(float m) {
    fader.setTime(m);
}

void ley::Sprite::render(SDL_Renderer * r, bool d) {
    if(!texture || !isVisible()) {
        return; // assume this is a null sprite or not visible
    }
    
    unsigned int frameIndex = frames.size() > 1 ? 
        (SDL_GetTicks() / animSpeed) % frames.size() : 0;

    dest_rect.x = pos.first; 
    dest_rect.y = pos.second;
    dest_rect.h = mHeight*mScale;
    dest_rect.w = mWidth*mScale;

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    
    char alpha;
    if(mFaderControl == 2) {
        alpha = 255;
    }
    else if(mFaderControl == 1) {
        alpha = 255 - (255*fader.pct());
    }
    else {
        alpha = 255*fader.pct();
    }
    SDL_SetTextureAlphaMod(texture, alpha);
    SDL_RenderCopy(r, texture, &frames[frameIndex], &dest_rect);

    if(!startFader) {
        //Start the fader on the first render call
        fader.pause(false);
        startFader = true;
    }

    fader.runFrame(false, 0.0);
    
    if(d) {    
        //If debug also render the fader timer.
        fader.render(r, d);
    }
}

/* Functions */
