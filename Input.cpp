/* 
Author: Steven Philley
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
void ley::Input::pollEvents(bool &running, bool& fullscreen, GameModel& gm) {
    SDL_Event event;
    if(SDL_PollEvent(&event))   {    //SDL_PollEvent calls pumpevents.
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        switch (event.type)     {       
            case SDL_QUIT:         
                running = false;
                break;
            
            case SDL_KEYDOWN:
                
                SDL_Log("Keydown Detected!");
                
                if (state[SDL_SCANCODE_RETURN]) {
                  printf("<RETURN> is pressed.\n");
                }
                if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) {
                    printf("Right and Up Keys Pressed.\n");
                }
                if ((state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN])
                    |(state[SDL_SCANCODE_RALT] && state[SDL_SCANCODE_RETURN])
                    ) {
                    printf("LALT + Return key pressed.\n");
                    fullscreen = !fullscreen;
                }
                if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_D]) {
                    gm.debugBoard();
                }
                if (state[SDL_SCANCODE_Q]) {
                    running = false;
                }
                break;
            
            default:
                break;
        }
     }
}