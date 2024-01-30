/* 
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: see header.
Date: Jan/29/2024
*/
#include "../inc/Fader.h"


ley::Fader::Fader()
:
mFader(1000,{0,0,0,0}) {
    mFader.reset();
}

char ley::Fader::alpha() {

    char alpha;

    if(mFaderControl == 2) {
        alpha = 255;
    }
    else if(mFaderControl == 1) {
        alpha = 255 - (255*mFader.pct());
    }
    else {
        alpha = 255*mFader.pct();
    }
    return alpha;
}

void ley::Fader::runFrame() {
    mFader.runFrame();

    if(mFader.hasExpired()) {
        if(mFaderControl == 0) {
            mFaderControl = 2;
        }
        else if(mFaderControl == 2) {
            mFaderControl = 1;
        }
        else if(mFaderControl == 1) {
            mFaderControl = 0;
        }
        mFader.reset();
    }
}