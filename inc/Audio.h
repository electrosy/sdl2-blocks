/* 
sdl2-blocks
Copyright (C) 2020,2021 Steven Philley. All rights reserved.

Purpose: Contains audio subsystem
Date: Dec/2/2021
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Theme.h"

namespace ley {

enum class sfx {swoosh, pause, unpause, squeek, piecesfalling, inplace, falldown};

class Audio {

    private:
        Mix_Music *musIntro;
        Mix_Music *musMainMenu;

        std::vector<Mix_Music*> mMusicList;

        Mix_Chunk* sfxSwoosh;
        Mix_Chunk* sfxPause;
        Mix_Chunk* sfxUnPause;
        Mix_Chunk* sfxSqueek;
        Mix_Chunk* sfxPiecesFalling;
        Mix_Chunk* sfxInPlace;
        Mix_Chunk* sfxFallDown;

        int playlistMax;

        int playlistNumber;

        ley::Theme mPlaylistTheme = ley::THEME_DEFAULT;
        bool mPlaylistLoaded = false;
        void freePlaylist();

    public:
        Audio();
        ~Audio();

        void playIntro();
        void playMainMenu();
        void playNext();
        void playPlaylist();
        void playSfx(ley::sfx sfx);
        // (Re)load the gameplay playlist for this theme. Returns false, leaving the
        // current playlist untouched, if none of the theme's tracks could be loaded.
        bool setTheme(ley::Theme theme);

        void fadeOutMusic();
        void increaseVolume();
        void decreaseVolume();
};

}

#endif