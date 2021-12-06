/*
sdl2-blocks
Copyright (C) 2020,2021 Steven Philley. All rights reserved.

Purpose: A fun game and Mini Game Framework.
Date: Feb/15/2020

Notes: Intended to be built around the MVC philosophy. GameModel.cpp(Model), Video.cpp(View), GameController.cpp(Controller)
*/
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Audio.h"
#include "Video.h"
#include "Input.h"
#include "Sprite.h"
#include "Block.h"
#include "GameModel.h"
#include "Clock.h"
#include "Winlet.h"
#include "Renderables.h"
#include "GameController.h"
#include "SimpleShape.h"
#include "Timer.h"
#include "Textures.h"
#include "Font.h"
#include "UIMenu.h"
#include "UIElement.h"
#include "HighScores.h"

auto const TARGET_FPS = 60; //provide at least this many frames per second.
auto const DELAY_MILI = 1.3f; //start delay for the game loop
auto const MILI_ADJ_MAX = 1000;

auto const LINES_POS_X_PX = 800; //Score text
auto const LINES_POS_Y_PX = 40;

auto const LVL_POS_X_PX = 800; //Level text
auto const LVL_POS_Y_PX = 60;

auto const SCORE_POS_X_PX = 800;
auto const SCORE_POS_Y_PX = 80;

enum class mainmenu {start,highscores,options,exit};
enum class optionsmenu {debug,sound_volume,back};

ley::Textures* ley::Textures::instance = nullptr;
typedef ley::Textures TextureManager;

int main(int argv, char** args) {
    ley::Video mainVideo;
    TextureManager::Instance()->setRenderer(mainVideo.getRenderer());
    //Block pieces
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_gr_30x30.bmp", "d");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_bl_30x30.bmp", "e");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_or_30x30.bmp", "f");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_pr_30x30.bmp", "g");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_rd_30x30.bmp", "h");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_yl_30x30.bmp", "i");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_cy_30x30.bmp", "j");
    //Logos
    TextureManager::Instance()->loadTexture("assets/sdllogo.png", "sdl");
    TextureManager::Instance()->loadTexture("assets/coloritlogo.png", "itlogo");
    TextureManager::Instance()->loadTexture("assets/mainmenu.png", "mainmenu");
    //Buttons for main menu
    //Start
    TextureManager::Instance()->loadTexture("assets/btnStart.png", "btnStart");
    TextureManager::Instance()->loadTexture("assets/start-white.png", "start-white");
    TextureManager::Instance()->loadTexture("assets/start-hot-red.png", "start-hot-red");
    //High Scores
    TextureManager::Instance()->loadTexture("assets/btnHighScores.png", "btnHighScores");
    TextureManager::Instance()->loadTexture("assets/highscores-white.png", "highscores-white");
    TextureManager::Instance()->loadTexture("assets/highscores-hot-red.png", "highscores-hot-red");
    //Options
    TextureManager::Instance()->loadTexture("assets/btnOptions.png", "btnOptions");
    TextureManager::Instance()->loadTexture("assets/options.png", "options-white");
    TextureManager::Instance()->loadTexture("assets/options-hot-red.png", "options-hot-red");
    //Exit
    TextureManager::Instance()->loadTexture("assets/btnExit.png", "btnExit");
    TextureManager::Instance()->loadTexture("assets/exit-white.png", "exit-white");
    TextureManager::Instance()->loadTexture("assets/exit-hot-red.png", "exit-hot-red");
    //Options Background
    TextureManager::Instance()->loadTexture("assets/optionsmenu.png", "optionsmenu");
    //Highscores Background
    TextureManager::Instance()->loadTexture("assets/highscores.png", "highscores");
    //Buttons for options menu.
    TextureManager::Instance()->loadTexture("assets/graphic/btn/back.png", "opt-back");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/back-hot.png", "opt-hot");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/back-white.png", "opt-white");
    //Yes and No selectors
    TextureManager::Instance()->loadTexture("assets/graphic/btn/yes.png", "yes");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/yes-hot.png", "yes-hot");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/yes-white.png", "yes-white");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/no.png", "no");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/no-hot.png", "no-hot");
    TextureManager::Instance()->loadTexture("assets/graphic/btn/no-white.png", "no-white");

    //Backgrounds.
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST01_0440_V1.JPG", "BG_WEST_00");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST09_0489_V1.JPG", "BG_WEST_01");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST10_0528_V1.JPG", "BG_WEST_02");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST04_0495_V1.JPG", "BG_WEST_03");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST05_0447_V1.JPG", "BG_WEST_04");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST06_0465_V1.JPG", "BG_WEST_05");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST07_0570_V1.JPG", "BG_WEST_06");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST08_0442_V1.JPG", "BG_WEST_07");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST02_0520_V1.JPG", "BG_WEST_08");
    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST03_0457_V1.JPG", "BG_WEST_09");

    bool programRunning = true; //If the program is running, this could also be in the game over state, to restart play.

    ley::Input mainInput; //create the input system object.

    ley::Renderables renderables;
    ley::Renderables debugRenderables;
    //Create the rendertable font object for lines
    ley::Font fontOne(mainVideo.getRenderer(), LINES_POS_X_PX, LINES_POS_Y_PX, 100, 35);
    ley::Font* ptrFont = &fontOne; //grab a pointer so we can update the text.
    renderables.push_back(&fontOne);

    //Create the font for the level ouput
    ley::Font fontLvl(mainVideo.getRenderer(), LVL_POS_X_PX, LVL_POS_Y_PX, 100, 35);
    ley::Font* ptrFontLvl = &fontLvl;
    renderables.push_back(&fontLvl);

    //Create the font for the score output
    ley::Font fontScore(mainVideo.getRenderer(), SCORE_POS_X_PX, SCORE_POS_Y_PX, 100, 35);
    ley::Font* ptrFontScore = &fontScore;
    renderables.push_back(&fontScore);

    std::vector<SDL_Rect> catFrames;
    SDL_Rect catFrame1;
    catFrame1.x = 0; catFrame1.y = 75;
    catFrame1.h = 100; catFrame1.w = 100;
    catFrames.push_back(catFrame1);

    SDL_Rect catFrame2;
    catFrame2.x = 132; catFrame2.y = 75;
    catFrame2.h = 100; catFrame2.w = 100;
    catFrames.push_back(catFrame2);

    SDL_Rect catFrame3;
    catFrame3.x = 261; catFrame3.y = 75;
    catFrame3.h = 100; catFrame3.w = 100;
    catFrames.push_back(catFrame3);

    SDL_Rect catFrame4; 
    catFrame4.x = 400; catFrame4.y = 75;
    catFrame4.h = 100; catFrame4.w = 100;
    catFrames.push_back(catFrame4);

    SDL_Rect catFrame5; 
    catFrame5.x = 8; catFrame5.y = 207;
    catFrame5.h = 100; catFrame5.w = 100;
    catFrames.push_back(catFrame5);

    SDL_Rect catFrame6; 
    catFrame6.x = 138; catFrame6.y = 207;
    catFrame6.h = 100; catFrame6.w = 100;
    catFrames.push_back(catFrame6);

    ley::Sprite catSprite(mainVideo.getRenderer(), "assets/cat-trans.png", 100, &catFrames);
    catSprite.setPos(25,650);
    renderables.push_back(&catSprite);
    ley::Sprite catSprite2(mainVideo.getRenderer(), "assets/cat-trans.png", 175, &catFrames);
    catSprite2.setPos(1150,650);
    renderables.push_back(&catSprite2);
    
    // test Winlet
    SDL_Rect debugBounds;
    debugBounds.x = 200; debugBounds.y = 200;
    debugBounds.h = 300; debugBounds.h = 300;
    SDL_Color debugBoundsColor = {100,100,100,100};
    ley::Winlet debugWinlet(debugBounds,debugBoundsColor);

    //SimpleShape
    ley::SimpleShape firstSimpleShape(mainVideo.getRenderer());
    renderables.push_back(&firstSimpleShape);
    firstSimpleShape.addShape("nextboundry", {ley::START_X_OFFSET_PX - 145,39,130,130});
    firstSimpleShape.addShape("boardboundry", {ley::START_X_OFFSET_PX-1,39,302,602});
    //Test Timer
    ley::Timer firstTimer(mainVideo.getRenderer(),3000,{10,300,100,50}); // a 3 second timer
    debugRenderables.push_back(&firstTimer);
    //Test Timer
    ley::Timer secondTimer(mainVideo.getRenderer(),2500,{10,400,100,25}); // a 2 second timer
    debugRenderables.push_back(&secondTimer);
    //Test Timer
    ley::Timer thirdTimer(mainVideo.getRenderer(),1000,{10,425,100,30}); // a 2 second timer
    debugRenderables.push_back(&thirdTimer);
    //Test Timer
    ley::Timer fourthTimer(mainVideo.getRenderer(),333,{10,455,100,5}); // a 2 second timer
    debugRenderables.push_back(&fourthTimer);
    
    //Fall down timer - TODO timers should go into the controller
    ley::Timer fallTimer(mainVideo.getRenderer(),1000,{ley::START_X_OFFSET_PX-1,641,302,2}); //Time to force the blockdown.
    renderables.push_back(&fallTimer);

    ley::GameModel mainGameModel(&fallTimer);
    mainGameModel.debugBoard(false);
    ley::GameController mainGameController(mainVideo.getRenderer(),&mainGameModel);

    bool fs = false; //full screen
    auto avgFPS = 0;

    //Start intro Music
    ley::Audio audSystem;
    audSystem.playIntro();

    ley::UIMenu mainUI;
    mainUI.runIntroScreen(&mainVideo, &mainInput, &mainGameModel, fs, "sdl", {400,170,414,240}, 1); //display the sdl logo
    mainUI.runIntroScreen(&mainVideo, &mainInput, &mainGameModel, fs, "itlogo", {400,155,400,400}, 1); //display the color it company logo
    
    audSystem.fadeOutMusic();

    /**** UI/UX ****/
    int menuItem; //Store the option selected from the main menu.
    int optionItem; //Store the option selected from the options menu.

    ley::Font fontGameOver(mainVideo.getRenderer(), 255, 190, 100, 35);
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
    ley::Font fontHighScores0(mainVideo.getRenderer(), 450, 190, 400, 40);
    ley::Font fontHighScores1(mainVideo.getRenderer(), 450, 225, 400, 40);
    ley::Font fontHighScores2(mainVideo.getRenderer(), 450, 260, 400, 40);
    ley::Font fontHighScores3(mainVideo.getRenderer(), 450, 295, 400, 40);
    ley::Font fontHighScores4(mainVideo.getRenderer(), 450, 330, 400, 40);
    ley::Font fontHighScores5(mainVideo.getRenderer(), 450, 365, 400, 40);
    ley::Font fontHighScores6(mainVideo.getRenderer(), 450, 400, 400, 40);
    ley::Font fontHighScores7(mainVideo.getRenderer(), 450, 435, 400, 40);
    ley::Font fontHighScores8(mainVideo.getRenderer(), 450, 470, 400, 40);
    ley::Font fontHighScores9(mainVideo.getRenderer(), 450, 505, 400, 40);
    ley::Font fontHighScores10(mainVideo.getRenderer(), 450, 540, 400, 40);

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

    while(masterloop && runInitialUI) {

        while(runInitialUI) {
            //read high scores at the main menu.
            
            if(!highscores.isClean()) { //check if we have new highscore data to read.
                highscores.read();
                highscores.renderScoreFonts(mainVideo.getRenderer(), &highScoreRenderables, fonts);
                highscoresmenu.addRenderables(highScoreRenderables);
                highscores.setClean(true);
            }

            audSystem.playMainMenu();
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
                programRunning = false;
                masterloop = false;
            }
        }
        runInitialUI = true;

        fallTimer.reset();
        
        /**** Main Game Loop ****/
        SDL_Log("Starting Game loop!");
        audSystem.fadeOutMusic();

        //unpause game if it is already paused.
        if(mainGameModel.isPaused()) {
            mainGameModel.pauseGame(false);
            fallTimer.pause(false);
        }

        ley::Clock mainClock;
        size_t frame_count = 0;
        long fpsAdjustMili = 0;
        bool fs_changed = false;
        double newTime = 1000;
        
        fontGameOver.updateMessage("");
        bool hitnext; //if player its next music button
        while(programRunning && !mainGameModel.isGameOver()) {
            SDL_Delay(DELAY_MILI + fpsAdjustMili);

        /**** MUSIC ****/
        audSystem.playPlaylist();

        
        /**** RENDER ****/
            avgFPS = 0;
            mainGameController.renderBackground();
            renderables.renderAll(); // render all sprites
            if(mainGameModel.isOverlayOn()) { //render debug renderables
                debugRenderables.renderAll();
            }
            mainGameController.renderBoard();
            if (SDL_GetTicks() % 1000 == 0 ) {
                auto seconds_from_start = mainClock.secondsFromStart();
                avgFPS = seconds_from_start == 0 
                    ? seconds_from_start : frame_count/seconds_from_start;
                SDL_Log("Seconds From Start:%s", std::to_string(seconds_from_start).c_str());
                SDL_Log("AVG FPS: %s",std::to_string(avgFPS).c_str());
            }
            mainVideo.render(); // output to the video system.

        /**** GET INPUT ****/
            //pollEtimervents updates running and full screen flags
            ley::Direction eventDirection = mainInput.pollEvents(programRunning,fs,mainGameModel, hitnext);
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
            /* Calculate Frame Rate and output to log */
            //adjust for target fps.
            if(avgFPS != 0) {
                auto avg_target_ratio = TARGET_FPS / avgFPS; //greater than 1 too slow, less than one too fast.
                if(avg_target_ratio > 0.954445f) { /*then speed up*/  /* TODO The point at which to speed up may need to 
                                                                be veriable. To ensure we dont dip below, we 
                                                                should probably adjust right before hitting 1
                                                                This adjustment may be very system specific, 
                                                                may be best to save the adjustment off to a file*/
                    
                    fpsAdjustMili -= 5;
                    if(fpsAdjustMili < 0) {fpsAdjustMili = 0;}
                } else if(avg_target_ratio < 0.7999f) { /* then slow down */ /* NOTE slowing down seems easier   
                                                                                because you dont notice when its too fast. */
                    
                    if(fpsAdjustMili < MILI_ADJ_MAX) {
                        ++fpsAdjustMili;    
                    }
                } else { fpsAdjustMili = 0; }
            }

            if (SDL_GetTicks() % 1000 == 0 ) {
                SDL_Log("FpsMiliAdjust:%s",std::to_string(fpsAdjustMili).c_str());
            }

            firstTimer.runFrame();
            secondTimer.runFrame();
            thirdTimer.runFrame();
            fourthTimer.runFrame();

            fallTimer.runFrame(false, newTime);

            mainGameController.runFrame(ptrFont, ptrFontLvl, ptrFontScore);
            ++frame_count; //TODO - every 10 minutes or so we should 0 (zero) the frame_count and  
                        // seconds_from_start, so there is no chance of a memory overrun

            /* additional control */
            //TODO this bit of code should probably go in the controller.
            if(fallTimer.hasExpired() == true) {
                newTime = mainGameModel.moveBlock(ley::Direction::down);
                fallTimer.reset();
            }

        /**** CLEAR ****/
            mainVideo.clear();
            
            //check if player hits the next track
            if(hitnext) {
                audSystem.playNext();
                hitnext = false;
            }
        }

        

        //continue to render graphics and get keyboard input after game is over.
        // TODO add FPS throttling to this part as well.
        //push on the new high score
        if(mainGameModel.isGameOver()) {
            highscores.push(mainGameModel.getScore(), "Steve", mainGameModel.getLevel(), mainGameModel.getLines());
            highscores.write();
            highscores.setClean(false);

            if(fontGameOver.getMessage().empty()) {
            fontGameOver.updateMessage("Game Over!!!");
            }
        }

        while(programRunning) {

            //For now just run at the frame rate that was set at the end of the game.
            SDL_Delay(DELAY_MILI + fpsAdjustMili);
            
            mainGameController.renderBoard();
            renderables.renderAll();
            if(mainGameModel.isOverlayOn()) {
                debugRenderables.renderAll();
            }
            mainVideo.render();
            ley::Direction eventDirection = mainInput.pollEndEvents(programRunning,fs,mainGameModel);

            
            mainVideo.clear(); //SDL_RenderClear()
        }

        /**** CLEAN UP ****/
        mainGameModel.resetGame();
        audSystem.fadeOutMusic();
        programRunning = true;

    }//EXIT THE GAME

    return 1;
}