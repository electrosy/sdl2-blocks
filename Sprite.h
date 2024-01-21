/*
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Representation of a sprite object on the screen. 
         This is the most basic game object that is 
         displayed on the screen
Date: Feb/17/2020
*/
#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Timer.h"
#include "Renderable.h"
#include "./inc/Screen.h"

namespace ley {

class Sprite : public Renderable {

private:
    std::pair<unsigned int,unsigned int> pos;
    unsigned int animSpeed;
    Timer fader;
    bool startFader = false;
    char mFaderControl = 0; //0 = fade in, 1 = fade out, 2 = hold //Run fade in reverse from in to out instead of out to in.
    double mScale = 1; //scale texture using percent
    int mWidth = -1; //queried from texture
    int mHeight = -1; //queried from texture
protected:
    SDL_Texture* texture = nullptr;
    std::vector<SDL_Rect> frames;
    SDL_Rect dest_rect;
public:
    Sprite(); //TODO try to remove default constructor for Sprite() as it should not be needed.
    Sprite(SDL_Texture * t, unsigned int, std::vector<SDL_Rect>, std::pair<int, SDL_Rect> f);
    ~Sprite();
    void resetFader();
    bool faderFinished();
    bool faderFinishedMessage();
    void reverseFader();
    void holdFader();
    void fadeTime(float m); //change fade time.
    void render(SDL_Renderer * r, bool d);
    void setPos(unsigned int, unsigned int);
    unsigned int getX() {return pos.first;}; unsigned int getY() {return pos.second;};
    void center(); //center this sprite on the screen
    void bottomLeft(); //put sprite at bottom left
    void bottomRight(); //put sprite at bottom right
    void scale(double s); //set the scale of the sprite using a percent
};

}
#endif