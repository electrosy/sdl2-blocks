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
#include "GameModel.h"
#include "Video.h"

enum class mainmenu {start,highscores,options,exit};  //TODO this probably goes in the game model
enum class optionsmenu {debug,sound_volume,back};  //TODO this probably goes in the game model

int main(int argv, char** args) {
    ley::GameModel mainGameModel;

    //old board
    //mainGameModel.debugBoard(false); 

    ley::Video mainVideo(&mainGameModel);
    ley::GameController mainGameController(&mainVideo,&mainGameModel);
    mainGameController.runGameLoop();

    return 1;
}