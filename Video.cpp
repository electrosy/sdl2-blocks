/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include <stdio.h>
#include <string>

#include "Video.h"

//Screen dimensions
const auto SCREEN_WIDTH = 1280;
const auto SCREEN_HEIGHT = 720;

const std::string APPLICATION_NAME = "Ablockalypse";
const std::string APPLICATION_VER = "0.1.6";
const std::string APPLICATION_PLATFORM = SDL_GetPlatform();
const std::string APPLICATION_REL_TYPE = "Alpha";
const std::string APPLICATION_ENV = "Development";

const std::string APPLICATION_STRING = (APPLICATION_NAME + " " + APPLICATION_VER + " " +  APPLICATION_PLATFORM + " " +  APPLICATION_REL_TYPE + " " + APPLICATION_ENV);


/* RAII */
void ley::Video::createWindow() {
    /* create a window to render to */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0) {
        // if all good
        window = SDL_CreateWindow(
            APPLICATION_STRING.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN /*SDL_WINDOW_FULLSCREEN*/);
    } else {
        printf("Can't Initialize SDL2");
        sdl_ready = 0;
    }
}

void ley::Video::createRenderer() {
    /* create a renderer */
    if(window != 0) { 
        renderer = SDL_CreateRenderer(window, -1, 0);
    } else {
        printf("Can't Initialize Renderer");
        sdl_ready = 0;
    }
 
}
void ley::Video::init() {
    sdl_ready = 1; //default to 1, this will get set to 0 if there is a problem.
    createWindow();
    createRenderer();
}

ley::Video::Video() {
    window = nullptr;
    renderer = nullptr;
    init();
}

ley::Video::~Video() {
    auto sdl_ready = 0;
    sdl_ready = !SDL_Init(0);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    if(sdl_ready && SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
   
    if(getReady()) {
        atexit(SDL_Quit);
    }
}
/* Accessors */
void ley::Video::setFullScreen(bool fs) {
    sdl_fullscreen = fs;

    if(fs) {
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
    } else { SDL_SetWindowFullscreen(window,SDL_WINDOW_SHOWN); }
}

/* functions */
void ley::Video::renderFrame() {
    
}

void ley::Video::render() {
    if(sdl_ready) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);        
        SDL_RenderPresent(renderer);
    }
}

void ley::Video::clear() {
    SDL_RenderClear(renderer);
}

