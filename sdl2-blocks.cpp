/*
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A fun game and Mini Game Framework.
Date: Feb/15/2020
*/
#include <stdio.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

// ** TODO s Edge Cases
//      1.) Test case where there are more than one full lines but not on consecutive rows. This case hasen't been coded for.

// ** New features
//      1.) 

auto const TARGET_FPS = 60; //provide at least this many frames per second.
auto const DELAY_MILI = 1.3f; //start delay for the game loop
auto const MILI_ADJ_MAX = 1000;

auto const SCORE_POS_X_PX = 680;
auto const SCORE_POS_Y_PX = 20;

auto const LVL_POS_X_PX = 680;
auto const LVL_POS_Y_PX = 40;



enum class menuitems {start,highscores,options,exit};

ley::Textures* ley::Textures::instance = nullptr;
typedef ley::Textures TextureManager;

void runIntroScreen(ley::Video* v, ley::Input* i, ley::GameModel* m, bool fs, std::string t, SDL_Rect r, double fpsDelay) {
    /**** Intro Screen Loop ****/
    bool intro = true;
    SDL_Texture* test = nullptr;
    test = TextureManager::Instance()->getTexture(t);
    SDL_SetTextureBlendMode(test,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(test, 255);

    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = 0; src_rect.y = 0; src_rect.h = r.h ; src_rect.w = r.w;
    dest_rect.x = r.x; dest_rect.y = r.y; dest_rect.h = r.h; dest_rect.w = r.w;

    unsigned int alphaFrameIndex = 0;
    bool faddedin = false;
    char fadespeed = 8;
    while(intro == true) {
        SDL_Delay(fpsDelay);

        if(!faddedin) {
            if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                if(alphaFrameIndex < 255) {
                    alphaFrameIndex++;
                }
                else {
                    faddedin = true;
                }
            }
        } else {
             if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                 if(alphaFrameIndex != 0) {  
                    alphaFrameIndex--;
                 }
            }
        }
        SDL_SetTextureAlphaMod(test, alphaFrameIndex);
        SDL_RenderCopy(v->getRenderer(), test, &src_rect, &dest_rect);
        v->render();
        if(i->pollTitleEvents(intro,fs,(*m)) == ley::Direction::down 
            || (alphaFrameIndex < 10 && faddedin)) {
            intro = false;
        }
        v->clear();
    }
}
void runOptionMenu(ley::Video* v, ley::Input* i, ley::GameModel* m, bool fs, std::string t, SDL_Rect r, double fpsDelay) {
    bool running = true;
    SDL_Texture* startButton = TextureManager::Instance()->getTexture("optionsmenu");

    while(running) {
        ley::Direction frameDirection = i->pollMainMenuEvents(running,fs,(*m));
    }
}

int runMenu(ley::Video* v, ley::Input* i, ley::GameModel* m, bool fs, std::string t, SDL_Rect r, double fpsDelay) {
    bool runmain = true;
    
    SDL_Texture* background = TextureManager::Instance()->getTexture(t); 
    
    SDL_Texture* btnStart = TextureManager::Instance()->getTexture("btnStart");
    SDL_Texture* startButton = TextureManager::Instance()->getTexture("start-white");
    SDL_Texture* startButtonHot = TextureManager::Instance()->getTexture("start-hot-red");

    SDL_Texture* btnHighScores = TextureManager::Instance()->getTexture("btnHighScores");
    SDL_Texture* hsButton = TextureManager::Instance()->getTexture("highscores-white");
    SDL_Texture* hsButtonHot = TextureManager::Instance()->getTexture("highscores-hot-red");

    SDL_Texture* btnOptions = TextureManager::Instance()->getTexture("btnOptions");
    SDL_Texture* options = TextureManager::Instance()->getTexture("options-white");
    SDL_Texture* optionsHot = TextureManager::Instance()->getTexture("options-hot-red");

    SDL_Texture* btnExit = TextureManager::Instance()->getTexture("btnExit");
    SDL_Texture* exit = TextureManager::Instance()->getTexture("exit-white");
    SDL_Texture* exitHot = TextureManager::Instance()->getTexture("exit-hot-red");

    SDL_Texture* btnCurrent = btnStart;
    SDL_Texture* currentButton = startButton;
    SDL_Texture* currentButtonHot = startButtonHot;

    SDL_SetTextureBlendMode(background,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(background, 255);

    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = 0; src_rect.y = 0; src_rect.h = r.h ; src_rect.w = r.w;
    dest_rect.x = r.x; dest_rect.y = r.y; dest_rect.h = r.h; dest_rect.w = r.w;

    //default - start
    SDL_Rect current_rect = {0,0,150,60};
    SDL_Rect current_dest_rect = {29,199,150,60};

    ley::UIMenu mainmenu;
    mainmenu.push("start",{0,0,139,46},{25,199,139,46},btnStart,startButton,startButtonHot);
    mainmenu.push("highscore",{0,0,323,64},{29,282,323,64},btnHighScores,hsButton,hsButtonHot);
    mainmenu.push("options",{0,0,218,63},{29,365,218,63},btnOptions,options,optionsHot);
    mainmenu.push("exit",{0,0,100,49},{30,451,100,49},btnExit,exit,exitHot);
    
    unsigned int alphaFrameIndex = 0;
    bool faddedin = false;
    char fadespeed = 5;
    while(runmain == true) {
        SDL_Delay(fpsDelay);

        if(!faddedin) {
            if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                if(alphaFrameIndex < 255) {
                    alphaFrameIndex++;
                }
                else {
                    faddedin = true;
                }
            }
        }
        SDL_SetTextureAlphaMod(background, alphaFrameIndex);
        SDL_SetTextureAlphaMod(currentButton, alphaFrameIndex);
        SDL_RenderCopy(v->getRenderer(), background, &src_rect, &dest_rect);

        //Display all the base menu elements
        std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*>> baseElements;
        mainmenu.getBaseElements(&baseElements); 
        for(int i = 0; i < baseElements.size() && !baseElements.empty(); ++i) {
            SDL_Rect source = std::get<0>(baseElements.at(i));
            SDL_Rect destination = std::get<1>(baseElements.at(i));
            SDL_Texture* baseTexture = std::get<2>(baseElements.at(i));


            SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
        }

        //Display either the hot or flicker depending on the current flag
        if((SDL_GetTicks() % 50) % 10) {
            if(faddedin) {
                mainmenu.setHot(true);
                SDL_RenderCopy(v->getRenderer(), mainmenu.currentTex(), &current_rect, &current_dest_rect);
            }
        } else {
            if(faddedin) {
                mainmenu.setHot(false);
                SDL_RenderCopy(v->getRenderer(), mainmenu.currentTex(), &current_rect, &current_dest_rect);
            }
        }
        v->render();

        ley::Direction frameDirection = i->pollMainMenuEvents(runmain,fs,(*m));
        
        if(frameDirection == ley::Direction::down || frameDirection == ley::Direction::right) {
            mainmenu.next();
        }
        
        if(frameDirection == ley::Direction::up || frameDirection == ley::Direction::left) {
            mainmenu.previous();
        }

        current_rect = mainmenu.currentSrc();
        current_dest_rect = mainmenu.currentDest();
        mainmenu.setHot(false);
        currentButton = mainmenu.currentTex();
        mainmenu.setHot(true);
        currentButtonHot = mainmenu.currentTex();

        v->clear();
    }

    return mainmenu.getIndex();
}

int main(int argv, char** args) {
    ley::Video mainVideo;
    TextureManager::Instance()->setRenderer(mainVideo.getRenderer());
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_gr_30x30.bmp", "d");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_bl_30x30.bmp", "e");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_or_30x30.bmp", "f");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_pr_30x30.bmp", "g");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_rd_30x30.bmp", "h");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_yl_30x30.bmp", "i");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/block_cy_30x30.bmp", "j");

    TextureManager::Instance()->loadTexture("assets/sdllogo.png", "sdl");
    TextureManager::Instance()->loadTexture("assets/coloritlogo.png", "itlogo");
    TextureManager::Instance()->loadTexture("assets/mainmenu.png", "mainmenu");

    TextureManager::Instance()->loadTexture("assets/optionsmenu.png", "optionsmenu");
    
    TextureManager::Instance()->loadTexture("assets/btnStart.png", "btnStart");
    TextureManager::Instance()->loadTexture("assets/start-white.png", "start-white");
    TextureManager::Instance()->loadTexture("assets/start-hot-red.png", "start-hot-red");

    TextureManager::Instance()->loadTexture("assets/btnHighScores.png", "btnHighScores");
    TextureManager::Instance()->loadTexture("assets/highscores-white.png", "highscores-white");
    TextureManager::Instance()->loadTexture("assets/highscores-hot-red.png", "highscores-hot-red");

    TextureManager::Instance()->loadTexture("assets/btnOptions.png", "btnOptions");
    TextureManager::Instance()->loadTexture("assets/options.png", "options-white");
    TextureManager::Instance()->loadTexture("assets/options-hot-red.png", "options-hot-red");

    TextureManager::Instance()->loadTexture("assets/btnExit.png", "btnExit");
    TextureManager::Instance()->loadTexture("assets/exit-white.png", "exit-white");
    TextureManager::Instance()->loadTexture("assets/exit-hot-red.png", "exit-hot-red");

    TextureManager::Instance()->loadTexture("assets/background/1280x720/Wested/WEST01_0440_V1.JPG", "BG_WEST_01");

    bool masterloop = true;
    bool programRunning = true;

    ley::Input mainInput;

    ley::Renderables renderables;
    //Create the rendertable font object
    ley::Font fontOne(mainVideo.getRenderer(), SCORE_POS_X_PX, SCORE_POS_Y_PX);
    ley::Font* ptrFont = &fontOne; //grab a pointer so we can update the text.
    renderables.push_back(&fontOne);

    //Create the font for the level ouput
    ley::Font fontLvl(mainVideo.getRenderer(), LVL_POS_X_PX, LVL_POS_Y_PX);
    ley::Font* ptrFontLvl = &fontLvl;
    renderables.push_back(&fontLvl);

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
    catSprite2.setPos(900,650);
    renderables.push_back(&catSprite2);
    
    // test Winlet
    SDL_Rect debugBounds;
    debugBounds.x = 200; debugBounds.y = 200;
    debugBounds.h = 300; debugBounds.h = 300;
    SDL_Color debugBoundsColor = {100,100,100,100};
    ley::Winlet debugWinlet(debugBounds,debugBoundsColor);

    

    //SimpleShape
    ////std::vector<SDL_Rect> rects;

    ley::SimpleShape firstSimpleShape(mainVideo.getRenderer());
    renderables.push_back(&firstSimpleShape);
    firstSimpleShape.addShape("nextboundry",{10,39,130,130});
    firstSimpleShape.addShape("boardboundry", {361,39,302,602});
    //Test Timer
    ley::Timer firstTimer(mainVideo.getRenderer(),3000,{10,300,100,50}); // a 3 second timer
    renderables.push_back(&firstTimer);
    //Test Timer
    ley::Timer secondTimer(mainVideo.getRenderer(),2500,{10,400,100,25}); // a 2 second timer
    renderables.push_back(&secondTimer);
    //Test Timer
    ley::Timer thirdTimer(mainVideo.getRenderer(),1000,{10,425,100,30}); // a 2 second timer
    renderables.push_back(&thirdTimer);
    //Test Timer
    ley::Timer fourthTimer(mainVideo.getRenderer(),333,{10,455,100,5}); // a 2 second timer
    renderables.push_back(&fourthTimer);
    
    //Fall down timer - TODO timers should go into the controller
    ley::Timer fallTimer(mainVideo.getRenderer(),1000,{361,641,302,2}); //Time to force the blockdown.
    renderables.push_back(&fallTimer);

    ley::GameModel mainGameModel(&fallTimer);
    mainGameModel.debugBoard(false);
    ley::GameController mainGameController(mainVideo.getRenderer(),&mainGameModel);

    bool fs = false; //full screen
    auto avgFPS = 0;

    runIntroScreen(&mainVideo, &mainInput, &mainGameModel, fs, "sdl", {193,170,414,240}, 1);
    runIntroScreen(&mainVideo, &mainInput, &mainGameModel, fs, "itlogo", {200,155,400,400}, 1);

    /**** UI/UX ****/
    bool runInitialUI = true;
    int menuItem;
    int optionItem;

    ley::Font fontGameOver(mainVideo.getRenderer(), 255, 190);
    fontGameOver.updateMessage("");
    ley::Font* ptrFontGameOver = &fontGameOver; //grab a pointer so we can update the text.
    renderables.push_back(ptrFontGameOver);

    while(masterloop && runInitialUI) {

        while(runInitialUI) {
        
            menuItem = runMenu(&mainVideo, &mainInput, &mainGameModel, fs, "mainmenu", {0,0,800,600}, 1);

            if(menuItem == 0) {
                runInitialUI = false;
            }
            else if(menuItem == 2) {
                optionItem = runMenu(&mainVideo, &mainInput, &mainGameModel, fs, "optionsmenu", {0,0,800,600}, 1);
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
        while(programRunning && !mainGameModel.isGameOver()) {
            SDL_Delay(DELAY_MILI + fpsAdjustMili);
        /**** RENDER ****/
            avgFPS = 0;
            mainGameController.renderBackground();
            renderables.renderAll(); // render all sprites
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
            ley::Direction eventDirection = mainInput.pollEvents(programRunning,fs,mainGameModel);
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

            mainGameController.runFrame(ptrFont, ptrFontLvl);
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
        }

        //continue to render graphics and get keyboard input after game is over.
        // TODO add FPS throttling to this part as well.

        if(fontGameOver.getMessage().empty() && mainGameModel.isGameOver()) {
            fontGameOver.updateMessage("Game Over!!!");
        }

        while(programRunning) {

            //For now just run at the frame rate that was set at the end of the game.
            SDL_Delay(DELAY_MILI + fpsAdjustMili);
            
            mainGameController.renderBoard();
            renderables.renderAll();
            mainVideo.render();
            ley::Direction eventDirection = mainInput.pollEndEvents(programRunning,fs,mainGameModel);

            mainVideo.clear();
        }

        /**** CLEAN UP ****/
        mainGameModel.resetGame();
        programRunning = true;
    }

    
    

    return 1;
}