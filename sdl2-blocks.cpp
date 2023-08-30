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

#include "Video.h"
#include "Input.h"
#include "Sprite.h"
#include "Block.h"
#include "GameModel.h"
#include "Renderables.h"  // TODO move all renderables into the View(video.cpp)
#include "GameController.h"
#include "Timer.h"
#include "Font.h"
#include "UIMenu.h"
#include "UIElement.h"
#include "HighScores.h"

enum class mainmenu {start,highscores,options,exit};  //TODO this probably goes in the game model
enum class optionsmenu {debug,sound_volume,back};  //TODO this probably goes in the game model

int main(int argv, char** args) {
    ley::GameModel mainGameModel; //instantiating the Game Model
    mainGameModel.debugBoard(false); 
    ley::Video mainVideo(&mainGameModel);
    ley::GameController mainGameController(&mainVideo,&mainGameModel);
    mainGameController.runIntros();
    mainGameController.runGameLoop(/*highscores*/);

    return 1;
}