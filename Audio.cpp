/* 
sdl2-blocks
Copyright (C) 2020,2021 Steven Philley. All rights reserved.

Purpose: see header.
Date: Dec/2/2021
*/

#include "Audio.h"

ley::Audio::Audio() :
playlistNumber(0), playlistMax(3) {

    SDL_InitSubSystem(SDL_INIT_AUDIO);

    int flags=MIX_INIT_MP3 || MIX_INIT_OGG;
    int initted=Mix_Init(flags);
    
    if(initted & flags != flags) {
        printf("Mix_Init: Failed to init required mp3 support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(2);
    }
    
    musIntro = Mix_LoadMUS("./assets/audio/MovieTheaterIntro.mp3");
    if(!musIntro) {
        printf("Mix_LoadMUS(musIntro): %s\n", Mix_GetError());
    }

    musMainMenu = Mix_LoadMUS("./assets/audio/8_bit_ooame_lofi.mp3");
    if(!musMainMenu) {
        printf("Mix_LoadMUS(musMainMenu): %s\n", Mix_GetError());
    }

    musMelJazz1 = Mix_LoadMUS("./assets/audio/wild_jazz.mp3");
    if(!musMelJazz1) {
        printf("Mix_LoadMUS(musMelJazz1): %s\n", Mix_GetError());
    }

    musMelJazz2 = Mix_LoadMUS("./assets/audio/jazz.ogg");
    if(!musMelJazz2) {
        printf("Mix_LoadMUS(musMelJazz2): %s\n", Mix_GetError());
    }

    musMelJazz3 = Mix_LoadMUS("./assets/audio/Shake and Bake.mp3");
    if(!musMelJazz3) {
        printf("Mix_LoadMUS(musMelJazz3): %s\n", Mix_GetError());
    }
}

ley::Audio::~Audio() {
    
    while(Mix_Init(0)) {
        Mix_Quit();
    }

    Mix_CloseAudio();

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void ley::Audio::playIntro() {

    if(Mix_PlayMusic(musIntro, 1) == -1) {
        printf("Mix_PlayMusic(musIntro): %s\n", Mix_GetError());
    }
}

void ley::Audio::playMainMenu() {

    //always start the playlist at 0 when returning to the main menu
    playlistNumber = 0;

    if(!Mix_PlayingMusic() || Mix_FadingMusic() == MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMainMenu, -1) == -1) {
                printf("Mix_PlayMusic(musMainMenu): %s\n", Mix_GetError());
            }
    }
    
}
void ley::Audio::playNext() {
    fadeOutMusic();
}

void ley::Audio::playPlaylist() {


    if(playlistNumber == 0) {

        if(!Mix_PlayingMusic() || Mix_FadingMusic() == MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMelJazz1, 1) == -1) {
                printf("Mix_PlayMusic(musMelJazz1): %s\n", Mix_GetError());
            } else {
                playlistNumber++;
            }
        }
    }

    if(playlistNumber == 1) {
         if(!Mix_PlayingMusic() || Mix_FadingMusic() == MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMelJazz2, 1) == -1) {
                printf("Mix_PlayMusic(musMelJazz2): %s\n", Mix_GetError());
            } else {
                playlistNumber++;
            }
        }
    }

    if(playlistNumber == 2) {
         if(!Mix_PlayingMusic() || Mix_FadingMusic() == MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMelJazz3, 1) == -1) {
                printf("Mix_PlayMusic(musMelJazz3): %s\n", Mix_GetError());
            } else {
                playlistNumber++;
            }
        }
    }

    //restart from the top of the playlist
    if(playlistNumber > (playlistMax-1)) {
        playlistNumber = 0;
    }
}

void ley::Audio::fadeOutMusic() {

    Mix_FadeOutMusic(0);
}