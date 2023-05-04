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
ley::Command ley::Input::pollMainMenuEvents(bool &running, GameModel& gm) {
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
                if (state[SDL_SCANCODE_UP]) {
                    frameDirection = ley::Command::up;
                }

                if (state[SDL_SCANCODE_DOWN]) {
                    frameDirection = ley::Command::down;
                }

                if (state[SDL_SCANCODE_RIGHT]) {
                    frameDirection = ley::Command::right;
                }

                if (state[SDL_SCANCODE_LEFT]) {
                    frameDirection = ley::Command::left;
                }

                if (state[SDL_SCANCODE_SPACE]) {
                    frameDirection = ley::Command::space;
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

ley::Command ley::Input::pollTitleEvents(bool &running, GameModel& gm) {
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
ley::Command ley::Input::pollEndEvents(bool& fullscreen, GameModel& gm) {
    SDL_Event event;
    ley::Command frameDirection = ley::Command::none; //direction for this frame;

    while(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {
            case SDL_QUIT:         
                gm.stopProgram(true);
                break;
            
            case SDL_KEYDOWN:
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
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_A]) {
                    gm.overlayToggle();
                }
                //quite game
                if (state[SDL_SCANCODE_Q]) {
                    gm.stopProgram(true);
                }
                break;
            default:
                break;
        }
     }

     return frameDirection;
}
ley::Command ley::Input::pollEvents(bool& fullscreen, GameModel* gm, bool &playnext) {
    SDL_Event event;
    ley::Command command = ley::Command::none; //direction for this frame;

    while(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                gm->stopProgram(true);
                break;
            
            case SDL_KEYDOWN:
                //TODO these inputs should manipulate the game controller instead of the game model directly.
                //Full screen mode
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    |(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])
                    ) { fullscreen = !fullscreen; }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    gm->debugBoard(false);
                }
                //Output setstate layer on the board
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_S]) {
                    gm->debugBoard(true);
                }
                if (state[SDL_SCANCODE_GRAVE]) {
                    command = ley::Command::console;
                }
                //Rotate Block counter clockwise
                if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_E]) {
                    command = ley::Command::cclockwise;
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
                    playnext = true;
                }

                //quite game
                if (state[SDL_SCANCODE_Q]) {
                    gm->stopProgram(true);
                    gm->setGameRunning(false);
                }

                //pause game
                if(state[SDL_SCANCODE_P]) {
                    command = ley::Command::pause;
                }
                break;
            default:
                break;
        }
     }

     return command;
}