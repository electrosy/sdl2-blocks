/* 
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: Config loads and saves user selected options
Date: Mar/27/2024
*/
#include <SDL2/SDL.h>

#ifndef CONFIG_H
#define CONFIG_H

namespace ley {

class Config {

private:
    int mBoardHeight = 0;
    int mBoardWidth = 0;

public:
    void read();
};

}

#endif