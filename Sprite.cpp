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
fader(1000,{0,0,0,0}) {
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

    bool multiframe = false;
    
    fader.pause(true); //start paused and resume on the first render
    if(v.size() > 0) {
        fader(f.first,{v.at(0)}); // fader is a Timer
    }
    else {
        int width;
        int height;

        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        fader(f.first,{0,0,width,height});
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
    } else {
        SDL_QueryTexture(texture, NULL, NULL, &source_rect.w, &source_rect.h);
        dest_rect.w = source_rect.w;
        dest_rect.h = source_rect.h;
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
    SDL_QueryTexture(
            texture,
            nullptr,
            nullptr,
            &tempRect.w,
            &tempRect.h);

    tempRect.x = SCREEN_WCENTER - (tempRect.w / 2);
    tempRect.y = SCREEN_HCENTER - (tempRect.h / 2);

    setPos(tempRect.x,tempRect.y);
}

void ley::Sprite::setPos(unsigned int x, unsigned int y) {
    pos.first = x; pos.second = y;
}
void ley::Sprite::resetFader() {
    startFader = false;
    fader.reset();
    fader.pause(true);
}

void ley::Sprite::render(SDL_Renderer * r, bool d) {
    unsigned int frameIndex = frames.size() > 1 ? 
        (SDL_GetTicks() / animSpeed) % frames.size() : 0;

    dest_rect.x = pos.first; dest_rect.y = pos.second;

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, 255*fader.pct());
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
