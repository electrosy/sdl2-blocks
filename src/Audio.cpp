/* 
sdl2-blocks
Copyright (C) 2020,2021 Steven Philley. All rights reserved.

Purpose: see header.
Date: Dec/2/2021
*/

#include "../inc/Audio.h"

ley::Audio::Audio() :
playlistNumber(0), playlistMax(0),
musIntro(nullptr), musMainMenu(nullptr),
sfxSwoosh(nullptr), sfxPause(nullptr), sfxUnPause(nullptr),
sfxSqueek(nullptr), sfxPiecesFalling(nullptr), sfxInPlace(nullptr), sfxFallDown(nullptr) {

    SDL_InitSubSystem(SDL_INIT_AUDIO);

    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    int initted = Mix_Init(flags);

    if ((initted & flags) != flags) {
        SDL_Log("Mix_Init: Failed to init required MP3/OGG support!");
        SDL_Log("Mix_Init: %s", Mix_GetError());
    }

    //Configure Audio system
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        SDL_Log("Mix_OpenAudio: %s", Mix_GetError());
        exit(2);
    }
    
    //Load music files
    musIntro = Mix_LoadMUS("./assets/audio/MovieTheaterIntro.mp3"); // SOURCE https://opengameart.org/content/movie-theater-intro
    if(!musIntro) {                                                 // AUTHOR Matthew Pablo
        SDL_Log("Mix_LoadMUS(musIntro): %s", Mix_GetError());
    }

    musMainMenu = Mix_LoadMUS("./assets/audio/8_bit_ooame_lofi__mixed.mp3"); // SOURCE https://opengameart.org/content/ooame
    if(!musMainMenu) {                                                       // AUTHOR TAD
        SDL_Log("Mix_LoadMUS(musMainMenu): %s", Mix_GetError());
    }



    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/adream.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(adream.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/bbsintro.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(bbsintro.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/fm-scrap.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(fm-scrap.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/hardcore.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(hardcore.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/inc.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(inc.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/iz-mih.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(iz-mih.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/oasis.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(oasis.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/stevee.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(stevee.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/unitech.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(unitech.s3m): %s", Mix_GetError());
    }

    mMusicList.push_back(Mix_LoadMUS("./assets/s3m/vouge-me.s3m"));
    if(!mMusicList.back()) {
        SDL_Log("Mix_LoadMUS(vouge-me.s3m): %s", Mix_GetError());
    }

    playlistMax = static_cast<int>(mMusicList.size());

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

    // Free all music tracks
    for (Mix_Music* m : mMusicList) {
        if (m) Mix_FreeMusic(m);
    }
    if (musIntro)     Mix_FreeMusic(musIntro);
    if (musMainMenu)  Mix_FreeMusic(musMainMenu);

    // Free all sound effects
    if (sfxSwoosh)        Mix_FreeChunk(sfxSwoosh);
    if (sfxPause)         Mix_FreeChunk(sfxPause);
    if (sfxUnPause)       Mix_FreeChunk(sfxUnPause);
    if (sfxSqueek)        Mix_FreeChunk(sfxSqueek);
    if (sfxPiecesFalling) Mix_FreeChunk(sfxPiecesFalling);
    if (sfxInPlace)       Mix_FreeChunk(sfxInPlace);
    if (sfxFallDown)      Mix_FreeChunk(sfxFallDown);

    while (Mix_Init(0)) {
        Mix_Quit();
    }

    Mix_CloseAudio();

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void ley::Audio::playIntro() {

    if(Mix_PlayMusic(musIntro, 1) == -1) {
        SDL_Log("Mix_PlayMusic(musIntro): %s", Mix_GetError());
    }
}

void ley::Audio::playMainMenu() {

    //always start the playlist at 0 when returning to the main menu
    playlistNumber = 0;

    if (!Mix_PlayingMusic() && Mix_FadingMusic() != MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMainMenu, -1) == -1) {
                SDL_Log("Mix_PlayMusic(musMainMenu): %s", Mix_GetError());
            }
    }
    
}
void ley::Audio::playNext() {
    fadeOutMusic();
}

void ley::Audio::playPlaylist() {

    if (Mix_FadingMusic() != MIX_FADING_OUT) {
        if (!Mix_PlayingMusic() && playlistNumber < static_cast<int>(mMusicList.size())) {
            if (Mix_PlayMusic(mMusicList[playlistNumber], 1) == -1) {
                SDL_Log("Mix_PlayMusic(mMusicList[%d]): %s", playlistNumber, Mix_GetError());
            } else {
                playlistNumber++;
            }
        }
    }

    // Restart from the top of the playlist.
    if (playlistNumber > playlistMax - 1) {
        playlistNumber = 0;
    }
}

void ley::Audio::playSfx(ley::sfx sfx) {
    
    Mix_Chunk* sfxToPlay = nullptr;

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