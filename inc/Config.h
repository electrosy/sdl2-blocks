/* 
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: Config loads and saves user selected options
Date: Mar/27/2024
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>

namespace ley {

const auto BOARDWIDTH_DEFAULT = 10;
const auto BOARDHEIGHT_DEFAULT = 20;
const auto BOARDHEIGHT_MIN = 8;
const auto BOARDWIDTH_MIN = 8;
const auto BOARDWIDTH_MAX = 25;
const auto BOARDHEIGHT_MAX = 22;

class Config {

private:
    int mBoardHeight = 0;
    int mBoardWidth = 0;

public:
    void read();
    int height() const { return mBoardHeight; };
    int width()  const { return mBoardWidth; };
};

}

#endif