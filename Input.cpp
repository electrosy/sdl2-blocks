/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include "Input.h"
#include "Video.h"

/* RAII */
ley::Input::Input() {
    
}

ley::Input::~Input() {

}

/* Functions */
ley::Command ley::Input::pollTitleEvents(bool &running) {
    SDL_Event event;
    ley::Command frameDirection = ley::Command::none; //direction for this frame;

    while(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                running = false;
                break;
            case SDL_KEYDOWN:
                //TODO these inputs should manipulate the game controller instead of the game model directly.
                //Full screen mode
                //continue game
                if (state[SDL_SCANCODE_RETURN]) {
                    frameDirection = ley::Command::down;
                }
                //quite game
                if (state[SDL_SCANCODE_Q]) {
                    running = false;
                }
                break;
            default:
                break;
        }
     }

     return frameDirection;
}

ley::Command ley::Input::pollEvents(bool& fullscreen) {
    SDL_Event event;
    ley::Command command = ley::Command::none; //direction for this frame;

    while(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                command = ley::Command::quit;
                break;
            
            case SDL_KEYDOWN:
                //debug clear
                if (state[SDL_SCANCODE_C]) {
                    command = ley::Command::debugclear;
                }
                if (state[SDL_SCANCODE_F]) {
                    command = ley::Command::debugfill;
                }
                //Full screen mode
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    |(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])
                    ) { fullscreen = !fullscreen; }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    command = ley::Command::debugtexture;
                }
                //Output setstate layer on the board
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_S]) {
                    command = ley::Command::debugcolide;                    
                }
                if (state[SDL_SCANCODE_GRAVE]) {
                    command = ley::Command::console;
                }
                //Rotate Block counter clockwise
                if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_E]) {
                    command = ley::Command::cclockwise; // TODO this needs some work, main menu has cclockwise overloaded
                }
                //Rotate Block clockwise
                if (state[SDL_SCANCODE_R]) {
                    command = ley::Command::clockwise;
                }

                //move block down
                if (state[SDL_SCANCODE_DOWN]) {
                    command = ley::Command::down;
                }
                //move block left
                if (state[SDL_SCANCODE_LEFT]) {
                    command = ley::Command::left;
                }
                //move block right
                if (state[SDL_SCANCODE_RIGHT]) {
                    command = ley::Command::right;
                }
                //play next audio music track
                if (state[SDL_SCANCODE_N]) {
                    command = ley::Command::nextSong;
                }

                //quite game
                if (state[SDL_SCANCODE_Q]) {
                    command = ley::Command::quit;
                }

                //debug next level
                if (state[SDL_SCANCODE_I]) {
                    command = ley::Command::debugnextlevel;
                }

                //debug prev level
                if (state[SDL_SCANCODE_U]) {
                    command = ley::Command::debugprevlevel;
                }

                //pause game
                if(state[SDL_SCANCODE_P] || state[SDL_SCANCODE_SLASH]) {
                    command = ley::Command::pause;
                }

                if(state[SDL_SCANCODE_RETURN]) {
                    command = ley::Command::enter;
                }

                if(state[SDL_SCANCODE_MINUS]) {
                    command = ley::Command::decreaseVolume;
                }

                if(state[SDL_SCANCODE_EQUALS]) {
                    command = ley::Command::increaseVolume;
                }

                if(state[SDL_SCANCODE_SPACE]) {
                    command = ley::Command::space;
                }
                
                break;
            default:
                break;
        }
     }

     return command;
}