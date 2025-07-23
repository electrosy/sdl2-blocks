/*
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: A fancy timer that reports an alpha value based on different types of fades.
Date: Jan/29/2024
*/
#ifndef FADER_H
#define FADER_H

#include "Timer.h"

namespace ley {

class Fader {

private:

    ley::Timer mFader;
    char mFaderControl = 0; //0 = fade in, 1 = fade out, 2 = hold //Run fade in reverse from in to out instead of out to in.

public:

    Fader();
    char alpha();
    void runFrame();
};

}

#endif