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
bool ley::Input::pollEvents() {
   
    auto running = true;
    SDL_Event event;   
    if(SDL_PollEvent(&event))   {     
        switch (event.type)     {       
            case SDL_QUIT:         
             running = false;
                break;        
            default:
                break;
                }
     }
     return running;
}