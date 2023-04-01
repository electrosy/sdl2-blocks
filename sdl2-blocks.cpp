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
    ley::GameModel mainGameModel; //instantiating the Game Model also starts the audio system.
    mainGameModel.debugBoard(false);

    ley::Video mainVideo(&mainGameModel);
    ley::Input mainInput; //create the input system object.

    ley::Renderables renderables;
    ley::Renderables debugRenderables;
    
    //Fall down timer - TODO timers should go into the controller
    ley::Timer fallTimer(mainVideo.getRenderer(),1000,{ley::START_X_OFFSET_PX-1,641,302,2}); //Time to force the blockdown.
    renderables.push_back(&fallTimer);

    mainGameModel.setTimer(&fallTimer);
    ley::GameController mainGameController(&mainVideo,&mainGameModel);

    bool fs = false; //full screen
    Uint32 avgFPS = 0;

    ley::UIMenu mainUI;

    //TODO runIntroScreens need better FPS throttling.
    mainUI.runIntroScreen(&mainVideo, &mainInput, &mainGameModel, fs, "sdl", {400,170,414,240}, 1); //display the sdl logo
    mainUI.runIntroScreen(&mainVideo, &mainInput, &mainGameModel, fs, "itlogo", {400,155,400,400}, 1); //display the color it company logo
    
    mainGameModel.fadeMusic();

    /**** UI/UX ****/
    int menuItem; //Store the option selected from the main menu.
    int optionItem; //Store the option selected from the options menu.

    ley::Font fontGameOver(255, 190, 100, 35);
    fontGameOver.updateMessage("");
    ley::Font* ptrFontGameOver = &fontGameOver; //grab a pointer so we can update the text.
    renderables.push_back(ptrFontGameOver);

    //Gather elements for the menus
    mainUI.push("start",{0,0,139,46},{25,199,139,46},"btnStart","start-white","start-hot-red");
    mainUI.push("highscore",{0,0,323,64},{29,282,323,64},"btnHighScores","highscores-white","highscores-hot-red");
    mainUI.push("options",{0,0,218,63},{29,365,218,63},"btnOptions","options-white","options-hot-red");
    mainUI.push("exit",{0,0,100,49},{30,451,100,49},"btnExit","exit-white","exit-hot-red");

    ley::UIMenu optionUI;
    optionUI.push("options",{0,0,218,63},{29,270,218,63},"btnOptions","options-white","options-hot-red");
    optionUI.push("options1",{0,0,218,63},{29,365,218,63},"btnOptions","options-white","options-hot-red");
    optionUI.push("back",{0,0,100,49},{30,451,100,49},"opt-back","opt-white","opt-hot");
    
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "yes", "yes-white", "yes-hot");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "no", "no-white", "no-hot");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "no", "no-hot", "no-white");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "btnExit","exit-white","exit-hot-red");

    optionUI.addSelector("back", {0,0,100,49}, {300,451,100,49}, "yes", "yes-white", "yes-hot");
    optionUI.addSelector("back", {0,0,100,49}, {300,451,100,49}, "no", "no-white", "no-hot");

    ley::UIMenu highscoresmenu;

    std::vector<ley::Font*> fonts;
    ley::Font fontHighScores0(450, 190, 400, 40);
    ley::Font fontHighScores1(450, 225, 400, 40);
    ley::Font fontHighScores2(450, 260, 400, 40);
    ley::Font fontHighScores3(450, 295, 400, 40);
    ley::Font fontHighScores4(450, 330, 400, 40);
    ley::Font fontHighScores5(450, 365, 400, 40);
    ley::Font fontHighScores6(450, 400, 400, 40);
    ley::Font fontHighScores7(450, 435, 400, 40);
    ley::Font fontHighScores8(450, 470, 400, 40);
    ley::Font fontHighScores9(450, 505, 400, 40);
    ley::Font fontHighScores10(450, 540, 400, 40);

    fonts.push_back(&fontHighScores0);
    fonts.push_back(&fontHighScores1);
    fonts.push_back(&fontHighScores2);
    fonts.push_back(&fontHighScores3);
    fonts.push_back(&fontHighScores4);
    fonts.push_back(&fontHighScores5);
    fonts.push_back(&fontHighScores6);
    fonts.push_back(&fontHighScores7);
    fonts.push_back(&fontHighScores8);
    fonts.push_back(&fontHighScores9);
    fonts.push_back(&fontHighScores10);

    ley::HighScores highscores;

    ley::Renderables highScoreRenderables;
    //Load highscore data initially.
    highscores.read();
    highscores.renderScoreFonts(mainVideo.getRenderer(), &highScoreRenderables, fonts);
    highscoresmenu.addRenderables(highScoreRenderables);

    bool masterloop = true; //Starts the main menu.
    bool runInitialUI = true;

    /*** Start Main Menu ***/
    Uint32 frame_start, frame_time;
    while(masterloop && runInitialUI) {
        while(runInitialUI) {
            //read high scores at the main menu.
            if(!highscores.isClean()) {//check if we have new highscore data to read.
                highscores.read();
                highscores.renderScoreFonts(mainVideo.getRenderer(), &highScoreRenderables, fonts);
                highscoresmenu.addRenderables(highScoreRenderables);
                highscores.setClean(true);
            }

            mainGameModel.playMainMenu(); // starts playing the music for the main menu
            menuItem = mainUI.runMenu(&mainVideo, &mainInput, &mainGameModel, fs, "mainmenu", {0,0,1280,720}, 1, menutypes::main);
                  
            if(menuItem == 0) {
                runInitialUI = false;
            }
            else if(menuItem == 1) {
                highscoresmenu.runMenu(&mainVideo, &mainInput, &mainGameModel, fs, "highscores", {0,0,1280,720}, 1, menutypes::highscores);
            }
            else if(menuItem == 2) {
                optionItem = optionUI.runMenu(&mainVideo, &mainInput, &mainGameModel, fs, "optionsmenu", {0,0,800,600}, 1, menutypes::options);
            } else if(menuItem == 3) {
                runInitialUI = false;
                mainGameModel.stopProgram(true);
                masterloop = false;
            }
        }
        runInitialUI = true;

        fallTimer.reset();
        
        mainGameModel.fadeMusic(); // finish up the intro music

        //unpause game if it is already paused.
        if(mainGameModel.isPaused()) {
            mainGameModel.pauseGame(false);
            fallTimer.pause(false);
        }

        mainGameModel.resetClock(); //restart the clock for the main game loop AVG FPS calculation.
        
        bool fs_changed = false;
        double newTime = 1000;
        
        fontGameOver.updateMessage("");
    
        /**** Main Game Loop ****/ 
        SDL_Log("Starting Game loop!");
        while(mainGameModel.programRunning() && !mainGameModel.isGameOver()) {
            frame_start = SDL_GetTicks();
            /**** MUSIC ****/
            mainGameModel.startPlayList(); //start the main playlist for game play

            /**** RENDER ****/
            mainVideo.render(); // renders background
            mainVideo.renderSprites();
            renderables.renderAll(mainVideo.getRenderer()); // render all sprites
            if(mainGameModel.isOverlayOn()) { //render debug renderables
                debugRenderables.renderAll(mainVideo.getRenderer());
            }
            mainGameController.renderBoard();

            if (SDL_GetTicks() % 1000 == 0 ) { // log once per second
                SDL_Log("frame_count: %u", mainGameModel.frameCount());
                SDL_Log("mainGameModel.Clock.secondsFromStart(): %u", mainGameModel.secondsFromStart());
            }

            mainVideo.present(); // output to the video system.
           
            /**** GET INPUT ****/
            //pollEtimervents updates running and full screen flags
            ley::Direction eventDirection = mainInput.pollEvents(fs,mainGameModel);
            if(fs != fs_changed) {
                mainVideo.setFullScreen(fs);
                fs_changed = !fs_changed;
            }
            /**** INPUT PROCESSING ****/
            //TODO this stuff should probably go in the controller
            if(eventDirection == ley::Direction::down) {
                fallTimer.reset();
                eventDirection = ley::Direction::none;
            } else if(eventDirection == ley::Direction::pause) {
                mainGameModel.pauseGame(!mainGameModel.isPaused());
                fallTimer.pause(!fallTimer.isPaused());
            }

            /**** UPDATE ****/
            fallTimer.runFrame(false, newTime);

            mainGameController.runFrame();
            mainGameModel.frameCountInc(); //TODO - every 10 minutes or so we should 0 (zero) the frame_count and  
                        // seconds_from_start, so there is no chance of a memory overrun

            /* additional control */
            //TODO this bit of code should probably go in the controller.
            if(fallTimer.hasExpired() == true) {
                newTime = mainGameModel.moveBlock(ley::Direction::down);
                fallTimer.reset();
            }

            /**** CLEAR ****/
            mainVideo.clear(); //clear the backbuffer

            /**** LOCK FRAME RATE ****/
            frame_time = SDL_GetTicks() - frame_start;
            if(SDL_GetTicks() % 500 == 0) { // every half second
                SDL_Log("frame_time: %u", frame_time);
            }

            mainVideo.frameDelay();
        }

        //continue to render graphics and get keyboard input after game is over.
        if(mainGameModel.isGameOver()) {
            //push on the new high score
            highscores.push(mainGameModel.getScore(), "Steve", mainGameModel.getLevel(), mainGameModel.getLines());
            highscores.write();
            highscores.setClean(false);

            if(fontGameOver.getMessage().empty()) {
            fontGameOver.updateMessage("Game Over!!!");
            }

            while(mainGameModel.programRunning()) {
                //This loop runs the gameover animations and should not run until the game is actually over.
                //For now just run at the frame rate that was set at the end of the game.
                
                mainGameController.renderBoard();
                renderables.renderAll(mainVideo.getRenderer());
                mainVideo.renderSprites(); 
                if(mainGameModel.isOverlayOn()) {
                    debugRenderables.renderAll(mainVideo.getRenderer());
                }
                mainVideo.present();
                ley::Direction eventDirection = mainInput.pollEndEvents(fs,mainGameModel);

                mainVideo.clear(); //SDL_RenderClear()

                mainVideo.frameDelay();
            }
        }

        /**** CLEAN UP ****/
        mainGameModel.resetGame();

    }//EXIT THE GAME

    return 1;
}