/* 
Author: Steven Philley
Purpose: Input System for the game.
Date: Feb/14/2020
*/

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

namespace ley {

class Input {

private:
  
public:
    Input();
    ~Input();

    void pollEvents(bool&, bool&);
};

}
#endif
