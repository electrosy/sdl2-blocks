/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include <stdio.h>
#include <string>

#include "Video.h"
#include "SimpleShape.h"

const auto START_DELAY = 1; //start delay for the game loop
const auto TARGET_FPS = 144.0; //provide at least this many frames per second.
const auto FPS_ADJUST_RATE = 1000; // adjust fps every second

const std::string APPLICATION_NAME = "Ablockalypse";
const std::string APPLICATION_VER = "0.2.0.2"; //Major, Minor(Set of new features), Features, Bugfix or Refactor
const std::string APPLICATION_PLATFORM = SDL_GetPlatform();
const std::string APPLICATION_REL_TYPE = "Alpha";
const std::string APPLICATION_ENV = "Development";
const std::string SDL_VERSION_COMP_LABEL = "SDL Compiled Version: ";
const std::string SDL_VERSION_LINK_LABEL = "SDL Linked Version: ";

const std::string APPLICATION_STRING = (APPLICATION_NAME + " " + APPLICATION_VER + " " +  APPLICATION_PLATFORM + " " +  APPLICATION_REL_TYPE + " " + APPLICATION_ENV);

ley::Textures* ley::Textures::instance = nullptr;
typedef ley::Textures TextureManager;

// TODO the Video should not have a pointer to the game model. The Controller should update the model and then notify Video of changes.
ley::Video::Video(ley::GameModel* g)
:
window(nullptr),
renderer(nullptr),
video_ready(true),
gm(g),
renderbg(true),
mili_adjust(0),
SDLCompiled{10, 25, 175, 35},
SDLLinked{10, 60, 175, 35},
frame_count{0}
{
    init();
}

ley::Video::~Video() {
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    
}
void ley::Video::addRenderable(bool layer, ley::Renderable * r) {
    if(layer) {
        mDebugRenderables.push_back(r);
    }
    else {
        mRenderables.push_back(r);
    }
}
void ley::Video::frameDelay() {

    if(SDL_GetTicks() % FPS_ADJUST_RATE == 0 && avgFPS() > 0) { // once per second
    
        float fps_ratio = avgFPS() / TARGET_FPS;
        
        SDL_Log("Video::fps_ratio: %f", fps_ratio);

        if(fps_ratio > 2.5) {
            mili_adjust += 5; //add 5 mili seconds
        }
        else if (fps_ratio > 1) {
            mili_adjust += 1; //add 1 mili second
        } 
        else if (fps_ratio < 1 ) {
            mili_adjust -= 1; //subtract 1 mili second
        }
        else if (fps_ratio < 0.7) {
            mili_adjust -= 7;
        }
        else if (fps_ratio < 0.5) {
            mili_adjust -= 10; //subtract 5 mili seconds
        }

        if(mili_adjust < 0) { //mili_adust should never be negative
            mili_adjust = 0;
        }
                
        SDL_Log("Video::miliadjust: %f", mili_adjust);
    }

    SDL_Delay(START_DELAY + mili_adjust);
}

/* RAII */
void ley::Video::createWindow() {

    /* create a window to render to */
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0) {
        // if all good
        window = SDL_CreateWindow(
            APPLICATION_STRING.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN /*SDL_WINDOW_FULLSCREEN*/);
    } else {
        printf("Can't Initialize SDL2 Video");
        video_ready = 0;
    }
}

void ley::Video::createRenderer() {
    /* create a renderer */
    if(window != nullptr) { 
        renderer = SDL_CreateRenderer(window, -1, 0);
    } else {
        printf("Can't Initialize Renderer");
        video_ready = false;
    }
 
}
void ley::Video::init() {
    createWindow();
    createRenderer();
    loadTextures();
    loadSprites();
    //load fonts
    //initialize the fonts for line, level, score
    fontLines = { LINES_POS_X_PX, LINES_POS_Y_PX, 100, 35 };
    fontLvl = { LVL_POS_X_PX, LVL_POS_Y_PX, 100, 35 };
    fontScore = { SCORE_POS_X_PX, SCORE_POS_Y_PX, 100, 35 };
    
    mRenderables.push_back(&fontLines);
    mRenderables.push_back(&fontLvl);
    mRenderables.push_back(&fontScore);
    updateScores(); //TODO the model should call this

    //SimpleShape
    firstSimpleShape(renderer);
    mRenderables.push_back(&firstSimpleShape);
    firstSimpleShape.addShape("nextboundry", {ley::START_X_OFFSET_PX - 145,39,130,130});
    firstSimpleShape.addShape("boardboundry", {ley::START_X_OFFSET_PX-1,39,302,602});

    //debugShape
    debugSimpleShape(renderer);
    mDebugRenderables.push_back(&debugSimpleShape);
    debugSimpleShape.addShape("debugconsole", {0,0,1280,100});


    //Add some fonts for the SDL version.
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    SDLCompiled.updateMessage("SDLCompiled!" + std::to_string(compiled.major) + "!" + std::to_string(compiled.minor) + "!" + std::to_string(compiled.patch));
    SDLLinked.updateMessage("SDLLinked!" + std::to_string(linked.major) + "!" + std::to_string(linked.minor) + "!" + std::to_string(linked.patch));
    mDebugRenderables.push_back(&SDLCompiled);
    mDebugRenderables.push_back(&SDLLinked);

}
/* Accessors */
void ley::Video::setFullScreen(bool f) {

    if(f) {
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
    } else { SDL_SetWindowFullscreen(window,SDL_WINDOW_SHOWN); }
    fs = f;
}
void ley::Video::setRenderBackground(bool inRenderbg) {
    renderbg = inRenderbg;
}

/* functions */
void ley::Video::setBackgroundTexture() {
    SDL_Texture *bg_west_1 = nullptr;
    SDL_Rect start_rect;
    start_rect.x = 0;
    start_rect.y = 0;
    start_rect.w = 1280;
    start_rect.h = 720;
 
    std::string background_level;
    if(gm->getLevel() <= 9) {
        background_level = "BG_WEST_0" + std::to_string(gm->getLevel()); //background based on current level.
    } else {
        background_level = "BG_WEST_09";
    }
    
    spriteBackground = ley::Sprite(TextureManager::Instance()->getTexture(background_level.c_str()), 0, {start_rect});
}

void ley::Video::render() {

    setRenderBackground(gm->isGameRunning());

    //Render background
    if(renderbg) {
        setBackgroundTexture();
        spriteBackground.render(renderer);
    }
    
    //Then render sprites
    renderSprites();

    ++frame_count; //TODO - every 10 minutes or so we should 0 (zero) the frame_count and 
                                   // seconds_from_start, so there is no chance of a memory overrun
}
void ley::Video::renderSprites() {
    updateScores(); // TODO the controller should call this only when the scores are updated.
    mRenderables.renderAll(renderer);
    if(gm->isOverlayOn()) {
        mDebugRenderables.renderAll(renderer);
    }
}

void ley::Video::present() {

    if(video_ready) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
    }
}

void ley::Video::clear() {
    SDL_RenderClear(renderer);
}

void ley::Video::loadTextures() {
    TextureManager::Instance()->setRenderer(renderer);
    //Block pieces
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_gr_30x30.bmp", "d");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_bl_30x30.bmp", "e");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_or_30x30.bmp", "f");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_pr_30x30.bmp", "g");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_rd_30x30.bmp", "h");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_yl_30x30.bmp", "i");
    TextureManager::Instance()->loadTexture("assets/graphic/ph/glass_cy_30x30.bmp", "j");
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

    //Cat
    TextureManager::Instance()->loadTexture("assets/cat-trans.png", "cat");

    //Game controls
    TextureManager::Instance()->loadTexture("assets/graphic/game_controls.png", "game-controls");

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
}

void ley::Video::loadSprites() {
    catSprite = ley::Sprite(TextureManager::Instance()->getTexture("cat"), 75, cat_frames);
    catSprite.setPos(25,650);
    catSprite2 = ley::Sprite(TextureManager::Instance()->getTexture("cat"), 175, cat_frames);
    catSprite2.setPos(1150,650);

    mRenderables.push_back(&catSprite);
    mRenderables.push_back(&catSprite2);
}

void ley::Video::updateScores() {
    fontLines.updateMessage("Lines  " + std::to_string(int(gm->getLines())));
    fontLvl.updateMessage("Level  " + std::to_string(int(gm->getLevel())));
    fontScore.updateMessage("Score  " + std::to_string(int(gm->getScore())));
}

Uint32 ley::Video::avgFPS() {

    if(SDL_GetTicks() % 1000 == 0) { //recalculate the AVG fps only once per second.
        avg_fps = mainClock.secondsFromStart() == 0 ? mainClock.secondsFromStart() : frame_count / mainClock.secondsFromStart();
        SDL_Log("Video::frameCount(): %u", frame_count);
        SDL_Log("Video::secondsFromStart(): %u", mainClock.secondsFromStart());
        SDL_Log("Video::avgFPS(): %u", avg_fps);
    }

    return avg_fps;
}

void ley::Video::resetClock() {
    mainClock.reset();
}


