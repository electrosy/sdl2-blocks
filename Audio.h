/* 
sdl2-blocks
Copyright (C) 2020,2021 Steven Philley. All rights reserved.

Purpose: Contains audio subsystem
Date: Dec/2/2021
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace ley {

class Audio {

    private:
        Mix_Music *musIntro;
        Mix_Music *musMainMenu;

        Mix_Music *musMelJazz1;
        Mix_Music *musMelJazz2;
        Mix_Music *musMelJazz3;

        int playlistMax;

        int playlistNumber;

    public:
        Audio();
        ~Audio();

        void playIntro();
        void playMainMenu();
        void playNext();
        void playPlaylist();

        void fadeOutMusic();
};

}

#endif