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
    
    if(initted & (flags != flags)) {
        printf("Mix_Init: Failed to init required mp3 support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    if (!(initted & MIX_INIT_MP3)) {
        printf("SDL_mixer failed to initialize MP3 support: %s\n", Mix_GetError());
    }

    //Configure Audio system
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(2);
    }
    
    //Load music files
    musIntro = Mix_LoadMUS("./assets/audio/MovieTheaterIntro.mp3"); // SOURCE https://opengameart.org/content/movie-theater-intro
    if(!musIntro) {                                                 // AUTHOR Matthew Pablo
        printf("Mix_LoadMUS(musIntro): %s\n", Mix_GetError());
    }

    musMainMenu = Mix_LoadMUS("./assets/audio/8_bit_ooame_lofi__mixed.mp3"); // SOURCE https://opengameart.org/content/ooame
    if(!musMainMenu) {                                                       // AUTHOR TAD
        printf("Mix_LoadMUS(musMainMenu): %s\n", Mix_GetError());
    }

    musMelJazz1 = Mix_LoadMUS("./assets/audio/wild_jazz.mp3"); // SOURCE https://opengameart.org/content/wild-jazz
    if(!musMelJazz1) {                                         // AUTHOR Alex McCulloch
        printf("Mix_LoadMUS(musMelJazz1): %s\n", Mix_GetError());
    }

    musMelJazz2 = Mix_LoadMUS("./assets/audio/jazz.ogg"); // SOURCE https://opengameart.org/content/jazz-1
    if(!musMelJazz2) {                                    // AUTHOR Spring Spring
        printf("Mix_LoadMUS(musMelJazz2): %s\n", Mix_GetError()); 
    }

    musMelJazz3 = Mix_LoadMUS("./assets/audio/Shake and Bake.mp3"); // SOURCE https://opengameart.org/content/shake-and-bake 
    if(!musMelJazz3) {                                              // AUTHOR Matthew Pablo
        printf("Mix_LoadMUS(musMelJazz3): %s\n", Mix_GetError());
    }

    //Set Music Volume
    Mix_VolumeMusic(20);

    //Load Audio SFX
    sfxSwoosh = Mix_LoadWAV("./assets/audio/sfx/swoosh.wav"); // SOURCE https://pixabay.com/sound-effects/sword-movement-single-5-101337/
    sfxPause = Mix_LoadWAV("./assets/audio/sfx/pause.wav"); // SOURCE https://pixabay.com/sound-effects/pause-89443/
    sfxUnPause = Mix_LoadWAV("./assets/audio/sfx/unpause.wav"); // SOURCE https://pixabay.com/sound-effects/unpause-106278/
    sfxSqueek = Mix_LoadWAV("./assets/audio/sfx/squeek.wav"); // SOURCE https://pixabay.com/sound-effects/whiteboard-squeak-90931/
    sfxPiecesFalling = Mix_LoadWAV("./assets/audio/sfx/pieces-falling.wav"); // SOURCE https://pixabay.com/sound-effects/copper-and-metal-pieces-dropping-on-hardwood-surface-98296/
    sfxInPlace = Mix_LoadWAV("./assets/audio/sfx/human-impact.wav"); // SOURCE https://pixabay.com/sound-effects/human-impact-on-ground-6982/
    sfxFallDown = Mix_LoadWAV("./assets/audio/sfx/tick-tock.wav"); // SOURCE https://pixabay.com/sound-effects/mechanical-alarm-clock-14474/
}

ley::Audio::~Audio() {
    
    while(Mix_Init(0)) {
        Mix_Quit();
    }

    Mix_CloseAudio();

    //TODO Mix_FreeMusic & MixFreeChunk???

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

    if(!Mix_PlayingMusic() && !Mix_FadingMusic() == MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMainMenu, -1) == -1) {
                printf("Mix_PlayMusic(musMainMenu): %s\n", Mix_GetError());
            }
    }
    
}
void ley::Audio::playNext() {
    fadeOutMusic();
}

void ley::Audio::playPlaylist() {

    // TODO can !Mix_PlayingMusic be refactored up here with !Mix_FadingMusic() it likely doesn't need to be repeated.
    // we, can likely use a map here for this.
    if (!Mix_FadingMusic() == MIX_FADING_OUT) {

        if(playlistNumber == 0) {

            if(!Mix_PlayingMusic()) {
                if(Mix_PlayMusic(musMelJazz1, 1) == -1) {
                    printf("Mix_PlayMusic(musMelJazz1): %s\n", Mix_GetError());
                } else {
                    playlistNumber++;
                }
            }
        }

        if(playlistNumber == 1) {
            if(!Mix_PlayingMusic()) {
                if(Mix_PlayMusic(musMelJazz2, 1) == -1) {
                    printf("Mix_PlayMusic(musMelJazz2): %s\n", Mix_GetError());
                } else {
                    playlistNumber++;
                }
            }
        }

        if(playlistNumber == 2) {
            if(!Mix_PlayingMusic()) {
                if(Mix_PlayMusic(musMelJazz3, 1) == -1) {
                    printf("Mix_PlayMusic(musMelJazz3): %s\n", Mix_GetError());
                } else {
                    playlistNumber++;
                }
            }
        }
    }

    //restart from the top of the playlist
    if(playlistNumber > (playlistMax-1)) {
        playlistNumber = 0;
    }
}

void ley::Audio::playSfx(ley::sfx sfx) {
    
    Mix_Chunk* sfxToPlay;

    switch (sfx) {
        case ley::sfx::swoosh :
            sfxToPlay = sfxSwoosh;
        break;
        case ley::sfx::pause :
            sfxToPlay = sfxPause;
        break;
        case ley::sfx::unpause :
            sfxToPlay = sfxUnPause;
        break;
        case ley::sfx::squeek :
            sfxToPlay = sfxSqueek;       
        break; 
        case ley::sfx::piecesfalling :
            sfxToPlay = sfxPiecesFalling;
        break;
        case ley::sfx::inplace :
            sfxToPlay = sfxInPlace;
        break;
        case ley::sfx::falldown :
            sfxToPlay = sfxFallDown;
        break;

        default :
        break;
    }

    if(sfxToPlay) {
        Mix_PlayChannel(-1, sfxToPlay, 0);
    }
}


void ley::Audio::fadeOutMusic() {

    Mix_FadeOutMusic(500);
}

void ley::Audio::increaseVolume() {
    Mix_VolumeMusic(Mix_VolumeMusic(-1) + 10);
}

void ley::Audio::decreaseVolume() {

    //If current volume - decrease ammount is less than 0 then set to 0 and exit.
    if(Mix_VolumeMusic(-1) - 10 < 0) {
        Mix_VolumeMusic(0);
        return;
    }

    Mix_VolumeMusic(Mix_VolumeMusic(-1) - 10);
}