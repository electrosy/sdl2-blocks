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
ley::Direction ley::Input::pollMainMenuEvents(bool &running, bool& fullscreen, GameModel& gm) {
    SDL_Event event;
    ley::Direction frameDirection = ley::Direction::none; //direction for this frame;

    if(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                running = false;
                break;
            case SDL_KEYDOWN:
                SDL_Log("Keydown Detected!");
                //TODO these inputs should manipulate the game controller instead of the game model directly.
                //Full screen mode
                //continue game
                if (state[SDL_SCANCODE_DOWN]) {
                    frameDirection = ley::Direction::down;
                }
                //quite game
                if ( state[SDL_SCANCODE_Q] || state[SDL_SCANCODE_ESCAPE] || state[SDL_SCANCODE_RETURN]) {
                    running = false;
                }
                

                break;
            default:
                break;
        }
     }

     return frameDirection;
}

ley::Direction ley::Input::pollTitleEvents(bool &running, bool& fullscreen, GameModel& gm) {
    SDL_Event event;
    ley::Direction frameDirection = ley::Direction::none; //direction for this frame;

    if(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                running = false;
                break;
            case SDL_KEYDOWN:
                SDL_Log("Keydown Detected!");
                //TODO these inputs should manipulate the game controller instead of the game model directly.
                //Full screen mode
                //continue game
                if (state[SDL_SCANCODE_RETURN]) {
                    frameDirection = ley::Direction::down;
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
ley::Direction ley::Input::pollEndEvents(bool &running, bool& fullscreen, GameModel& gm) {
    SDL_Event event;
    ley::Direction frameDirection = ley::Direction::none; //direction for this frame;

    if(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {
            case SDL_QUIT:         
                running = false;
                break;
            
            case SDL_KEYDOWN:
                SDL_Log("Keydown Detected!");
                //TODO these inputs should manipulate the game controller instead of the game model directly.
                //Full screen mode
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    |(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])
                    ) { fullscreen = !fullscreen; }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    gm.debugBoard(false);
                }
                //Output setstate layer on the board
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_S]) {
                    gm.debugBoard(true);
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
ley::Direction ley::Input::pollEvents(bool &running, bool& fullscreen, GameModel& gm) {
    SDL_Event event;
    ley::Direction frameDirection = ley::Direction::none; //direction for this frame;

    if(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                running = false;
                break;
            
            case SDL_KEYDOWN:
                SDL_Log("Keydown Detected!");
                //TODO these inputs should manipulate the game controller instead of the game model directly.
                //Full screen mode
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    |(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])
                    ) { fullscreen = !fullscreen; }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    gm.debugBoard(false);
                }
                //Output setstate layer on the board
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_S]) {
                    gm.debugBoard(true);
                }
                //Rotate Block counter clockwise
                if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_E]) {
                    gm.rotateBlock(false);
                    SDL_Log("Rotate counter clockwise");
                }
                //Rotate Block clockwise
                if (state[SDL_SCANCODE_R]) {
                    gm.rotateBlock(true);
                    SDL_Log("Rotate clockwise");
                }

                //move block down
                if (state[SDL_SCANCODE_DOWN]) {
                    frameDirection = ley::Direction::down;
                    gm.moveBlock(ley::Direction::down);
                }
                //move block left
                if (state[SDL_SCANCODE_LEFT]) {
                    frameDirection = ley::Direction::left;
                    gm.moveBlock(ley::Direction::left);
                }
                //move block right
                if (state[SDL_SCANCODE_RIGHT]) {
                    frameDirection = ley::Direction::right;
                    gm.moveBlock(ley::Direction::right);
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