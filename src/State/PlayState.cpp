#include "../../inc/State/PlayState.h"

namespace ley {

const std::string PlayState::sPlayID = "PLAY";

PlayState::PlayState(ley::Video * v, ley::GameModel * gm)
:
mVideoSystem(v),
mGameModel(gm),
firstTimer(3000,{10,300,100,50}),
secondTimer(2500,{10,400,100,25}),
thirdTimer(1000,{10,425,100,30}),
fourthTimer(333,{10,455,100,5}),
fallTimer(1000,{ley::BLOCK_START_POS_X_PX-1,641,302,2}),
statusTimer(1000,{10,500,100,5}),
statusFont(VOLUME_POS_X_PX, VOLUME_POS_Y_PX, 100, 20) {

    statusFont.updateMessage("Start Game");

}

void PlayState::update(ley::Command command) {

    switch (command) {
        case ley::Command::pause :
            mGameModel->pauseGame(!mGameModel->isPaused());
            fallTimer.pause(!fallTimer.isPaused());
        break;
        case ley::Command::cclockwise :
            if (mGameModel->rotateBlock(false)) {
                mGameModel->audio()->playSfx(ley::sfx::swoosh);
            }
        break;
        case ley::Command::clockwise :
            if (mGameModel->rotateBlock(true)) {
                mGameModel->audio()->playSfx(ley::sfx::swoosh);
            }
        break;
        case ley::Command::down :
            if(mGameModel->moveBlock(ley::Command::down)) {
                mGameModel->audio()->playSfx(ley::sfx::squeek);
            }

            fallTimer.reset();
        break;
        case ley::Command::left :
            if(mGameModel->moveBlock(ley::Command::left)) {
                mGameModel->audio()->playSfx(ley::sfx::squeek);
            }
        break;
        case ley::Command::right :
            if(mGameModel->moveBlock(ley::Command::right)) {
                mGameModel->audio()->playSfx(ley::sfx::squeek);
            }
        break;
        case ley::Command::debugcolide :
            mGameModel->debugBoard(true);
        break;
        case ley::Command::debugtexture :
            mGameModel->debugBoard(false);
        break;
        case ley::Command::quit :
            mGameModel->setGameRunning(false);
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::decreaseVolume :
            statusFont.updateMessage("Volume down");
            statusTimer.reset();
        break;
        case ley::Command::increaseVolume :
            statusFont.updateMessage("Volume up");
            statusTimer.reset();
        break;
        case ley::Command::nextSong :
            statusFont.updateMessage("Next song");
            statusTimer.reset();
        break;
        case ley::Command::space :
            mGameModel->quickDrop();
        break;
        
        defaut:
        break;
    }

    /**** UPDATE ****/
    fallTimer.runFrame(false, mGameModel->speed());
    firstTimer.runFrame();
    secondTimer.runFrame();
    thirdTimer.runFrame();
    fourthTimer.runFrame();
    statusTimer.runFrame(false, 0.0);
    if(statusTimer.hasExpired()) {
        statusFont.updateMessage("");
    }

    //Check to see if we need to move the block down.
    if(fallTimer.hasExpired()) {
        mGameModel->moveBlock(ley::Command::down);
        fallTimer.reset();
        mGameModel->audio()->playSfx(ley::sfx::falldown);
    }

    mGameModel->audio()->playPlaylist();
}

void PlayState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), mGameModel->isOverlayOn());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

void PlayState::loadRenderables() {
    
    mRenderables.push_back(&statusFont);
    mRenderables.push_back(&fallTimer);

    mDebugRenderables.push_back(&firstTimer);
    mDebugRenderables.push_back(&secondTimer);
    mDebugRenderables.push_back(&thirdTimer);
    mDebugRenderables.push_back(&fourthTimer);
    mDebugRenderables.push_back(&statusTimer);
}

bool PlayState::onEnter() {
    resetGame();
    SDL_Log("Entering PlayState and loading renderables");
    mVideoSystem->resetBackgroundFader();
    loadRenderables();

    return true;
}

bool PlayState::onReEnter() {
    if(fallTimer.isPaused()) {
        fallTimer.pause(false);
    }
    SDL_Log("ReEntering PlayState");

    return true;
}

bool PlayState::onExit() {    
    SDL_Log("Exiting Playstate");

    mGameModel->audio()->fadeOutMusic();
    
    return true;
}

bool PlayState::onPause() {
    GameState::onPause();
    
    return true;
}

void PlayState::resetGame() {
    fallTimer.reset();

    //unpause game if it is already paused.
    if(mGameModel->isPaused()) {
        mGameModel->pauseGame(false);
        fallTimer.pause(false);
    }

    mGameModel->resetGame();
}

}