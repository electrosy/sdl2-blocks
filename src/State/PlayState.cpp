#include "../../inc/State/PlayState.h"

namespace ley {

const std::string PlayState::sPlayID = "PLAY";

PlayState::PlayState(ley::Video * v, ley::GameModel * gm)
:
mVideoSystem(v),
mGameModel(gm),
firstTimer(v->getRenderer(),3000,{10,300,100,50}),
secondTimer(v->getRenderer(),2500,{10,400,100,25}),
thirdTimer(v->getRenderer(),1000,{10,425,100,30}),
fourthTimer(v->getRenderer(),333,{10,455,100,5}),
fallTimer(v->getRenderer(),1000,{ley::START_X_OFFSET_PX-1,641,302,2}) {

}

void PlayState::update(ley::Command command) {

    switch (command) {
        case ley::Command::pause :
            mGameModel->pauseGame(!mGameModel->isPaused());
            fallTimer.pause(!fallTimer.isPaused());
        break;
        case ley::Command::console :
            mGameModel->overlayToggle();
        break;
        case ley::Command::cclockwise :
            mGameModel->rotateBlock(false);
            SDL_Log("Rotate counter clockwise");
        break;
        case ley::Command::clockwise :
            mGameModel->rotateBlock(true);
            SDL_Log("Rotate clockwise");
        break;
        case ley::Command::down :
            mGameModel->moveBlock(ley::Command::down);
            fallTimer.reset();
        break;
        case ley::Command::left :
            mGameModel->moveBlock(ley::Command::left);
        break;
        case ley::Command::right :
            mGameModel->moveBlock(ley::Command::right);
        break;
    }

    /**** UPDATE ****/
    fallTimer.runFrame(false, blockFallSpeed);
    firstTimer.runFrame();
    secondTimer.runFrame();
    thirdTimer.runFrame();
    fourthTimer.runFrame();

    //Check to see if we need to move the block down.
    if(fallTimer.hasExpired()) {
        blockFallSpeed = mGameModel->moveBlock(ley::Command::down);
        fallTimer.reset();
    }
}

void PlayState::loadRenderables() {
    mVideoSystem->addRenderable(false, &fallTimer);
    mVideoSystem->addRenderable(true, &firstTimer);
    mVideoSystem->addRenderable(true, &secondTimer);
    mVideoSystem->addRenderable(true, &thirdTimer);
    mVideoSystem->addRenderable(true, &fourthTimer);
}

bool PlayState::onEnter() {
    
    fallTimer.reset();

    //unpause game if it is already paused.
    if(mGameModel->isPaused()) {
        mGameModel->pauseGame(false);
        fallTimer.pause(false);
    }

    mVideoSystem->resetClock(); //restart the clock for the main game loop AVG FPS calculation. 
    SDL_Log("Entering PlayState and loading renderables");
    loadRenderables();
    return true;
}

bool PlayState::onExit() {
    SDL_Log("Exiting Playstate");
    return true;
}

}