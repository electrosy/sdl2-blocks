/*
sdl2-blocks
Copyright (C) 2020-2021 Steven Philley. All rights reserved.

Purpose: A fun game and Mini Game Framework.
Date: Feb/15/2020

Notes: Intended to be built around the MVC philosophy.
        GameModel.cpp(Model) -> Contains the game state and game specific logic (Business Logic)
        Video.cpp(View) -> Handles Windows and Rendering contexts
        GameController.cpp(Controller) -> Handles User input and controlling the game state.

        Controller has a pointer to Video and Model
        Video has a pointer to Model so it can just render the current state.

                    |-------|
            |-------| Video |<-----|
            |       |-------|      |
            |                    |------------|
            |                    | Controller |
            v                    |------------|
        |-------|                  |
        | Model |<-----------------|
        |-------|
*/
#include <SDL2/SDL.h>

#include "GameController.h"
#include "inc/GameModel.h"
#include "inc/gfx/Video.h"

enum class mainmenu {start,highscores,options,exit};  //TODO this probably goes in the game model
enum class optionsmenu {debug,sound_volume,back};  //TODO this probably goes in the game model

int main(int argc, char *argv[]) {
    ley::GameModel mainGameModel;
    ley::Video mainVideo(&mainGameModel);
    ley::GameController mainGameController(&mainVideo, &mainGameModel);
    mainGameController.runGameLoop();

    return 0; // Return 0 for success
}