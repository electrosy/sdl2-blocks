/*
sdl2-blocks
Copyright (C) 2020-2021 Steven Philley. All rights reserved.

Purpose: A fun game and Mini Game Framework.
Date: Feb/15/2020

Notes: Intended to be built around the MVC philosophy. GameModel.cpp(Model), Video.cpp(View), GameController.cpp(Controller)
*/
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GameController.h"
#include "GameModel.h"
#include "Video.h"

enum class mainmenu {start,highscores,options,exit};  //TODO this probably goes in the game model
enum class optionsmenu {debug,sound_volume,back};  //TODO this probably goes in the game model

int main(int argv, char** args) {
    ley::GameModel mainGameModel;
    mainGameModel.debugBoard(false); 
    ley::Video mainVideo(&mainGameModel);
    ley::GameController mainGameController(&mainVideo,&mainGameModel);
    mainGameController.runIntros();
    mainGameController.runGameLoop();

    return 1;
}