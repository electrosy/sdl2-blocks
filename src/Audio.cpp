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
        printf("Mix_Init: Failed to init required MP3/OGG support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
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



    #ifndef FULL_ASSETS
        mMusicList.push_back(Mix_LoadMUS("./assets/audio/wild_jazz.mp3")); // SOURCE https://opengameart.org/content/wild-jazz
        if(!mMusicList.back()) {                                           // AUTHOR Alex McCulloch
            printf("Mix_LoadMUS(wild_jazz.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/jazz.ogg")); // SOURCE https://opengameart.org/content/jazz-1
        if(!mMusicList.back()) {                                      // AUTHOR Spring Spring
            printf("Mix_LoadMUS(jazz.ogg): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/Shake and Bake.mp3")); // SOURCE https://opengameart.org/content/shake-and-bake
        if(!mMusicList.back()) {                                                 // AUTHOR Matthew Pablo
            printf("Mix_LoadMUS(Shake and Bake.mp3): %s\n", Mix_GetError());
        }
    #elif FULL_ASSETS
        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/come-with-me-tonight-239958.mp3")); // SOURCE https://pixabay.com/music/traditional-jazz-come-with-me-tonight-239958/ 
        if(!mMusicList[0]) {                                                                       // AUTHOR Music by u_0tyyfec3hz from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/come-with-me-tonight-239958.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/moonlit-groove-248491.mp3"));       // SOURCE https://pixabay.com/music/traditional-jazz-moonlit-groove-248491/
        if(!mMusicList[1]) {                                                                       // AUTHOR Music by John Schofield from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/moonlit-groove-248491.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/jazz-elevator-musik-278566.mp3"));  // SOURCE https://pixabay.com/music/traditional-jazz-jazz-elevator-musik-278566/
        if(!mMusicList[2]) {                                                                       // AUTHOR Music by Tommsel from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/jazz-elevator-musik-278566.mp3): %s\n", Mix_GetError());
        }
        
        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/jazz-only-good-vibes-275341.mp3"));  // SOURCE https://pixabay.com/music/traditional-jazz-jazz-only-good-vibes-275341/
        if(!mMusicList[3]) {                                                                        // AUTHOR Music by NiKneT_Art from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/jazz-only-good-vibes-275341.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/guitar-jazz-song-300816.mp3"));      // SOURCE https://pixabay.com/music/smooth-jazz-guitar-jazz-song-300816/
        if(!mMusicList[4]) {                                                                        // AUTHOR Music by Mircea Iancu from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/guitar-jazz-song-300816.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/sebuah-kenangan-226035.mp3"));       // SOURCE https://pixabay.com/music/traditional-jazz-sebuah-kenangan-226035/
        if(!mMusicList[5]) {                                                                        // AUTHOR Music by Wahyu Music from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/sebuah-kenangan-226035.mp3): %s\n", Mix_GetError());
        }
        
        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/good-morning-cafe-jazz-227888.mp3")); // SOURCE https://pixabay.com/music/traditional-jazz-good-morning-cafe-jazz-227888/
        if(!mMusicList[6]) {                                                                         // AUTHOR Music by Sleep Volume from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/good-morning-cafe-jazz-227888.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/rhythmic-jazz-289915.mp3"));          // SOURCE https://pixabay.com/music/funk-rhythmic-jazz-289915/
        if(!mMusicList[7]) {                                                                         // AUTHOR Music by Silviu on the street from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/rhythmic-jazz-289915.mp3): %s\n", Mix_GetError());
        }

        mMusicList.push_back(Mix_LoadMUS("./assets/audio/music/to-my-precious-person-291357.mp3"));  // SOURCE https://pixabay.com/music/beats-to-my-precious-person-291357/
        if(!mMusicList[8]) {                                                                         // AUTHOR Music by EDDIE LEE from Pixabay
            printf("Mix_LoadMUS(./assets/audio/music/to-my-precious-person-291357.mp3): %s\n", Mix_GetError());
        }
    #endif

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
        printf("Mix_PlayMusic(musIntro): %s\n", Mix_GetError());
    }
}

void ley::Audio::playMainMenu() {

    //always start the playlist at 0 when returning to the main menu
    playlistNumber = 0;

    if (!Mix_PlayingMusic() && Mix_FadingMusic() != MIX_FADING_OUT) {
            if(Mix_PlayMusic(musMainMenu, -1) == -1) {
                printf("Mix_PlayMusic(musMainMenu): %s\n", Mix_GetError());
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
                printf("Mix_PlayMusic(mMusicList[%d]): %s\n", playlistNumber, Mix_GetError());
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