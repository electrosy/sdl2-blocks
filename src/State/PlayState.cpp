#include "../../inc/State/PlayState.h"

namespace ley {

const std::string PlayState::sPlayID = "PLAY";

PlayState::PlayState(ley::Video * v, ley::GameModel * gm)
:
mVideoSystem(v),
mGameModel(gm),
statusTimer(2000,{10,500,100,5}),
fallTimer(1000,{}),
mStatusFont(STATUSMESSAGE_POS_X_PX, STATUSMESSAGE_POS_Y_PX, 100, 20),
mLastHardDrop(gm->getDropCoolDown(),{0,0,0,0}) { 

    std::string statusString = mGameModel->getLanguageModel()->getWord("start game", 0, false, capitalizationtype::capitalizeWords);
    statusString += " - " + mGameModel->getLanguageModel()->getWord("press '?' for help", 0, false, capitalizationtype::capitalizeFirst);
    mStatusFont.updateMessage(statusString);
    mActiveUIElement = {};
    mLastHardDrop.expire(); // We want the last hard drop to start out expired because hard drop hasen't been activated yet.
    
}

void PlayState::update(ley::Command command) {

    switch (command) {
        case ley::Command::pause :
            mGameModel->pauseGame(!mGameModel->isPaused());
            fallTimer.pause(!fallTimer.isPaused());
        break;
        case ley::Command::cclockwise :
            {
                bool rotated = false;
                if(mGameModel->getWallKickOn() == "on") {
                    rotated = mGameModel->rotateWithKick(false);
                }
                else {
                    rotated = mGameModel->rotateBlock(false).first;
                }

                if(rotated) {
                    mGameModel->audio()->playSfx(ley::sfx::swoosh);
                }
            }

        break;
        case ley::Command::clockwise :
             {
                bool rotated = false;
                if(mGameModel->getWallKickOn() == "off") {
                    rotated = mGameModel->rotateWithKick(true);
                }
                else {
                    rotated = mGameModel->rotateBlock(true).first;
                }

                if(rotated) {
                    mGameModel->audio()->playSfx(ley::sfx::swoosh);
                }
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
            //mGameModel->debugBoard(true);
            mGameModel->getNewBoard()->debugOutput(true);
        break;
        case ley::Command::debugtexture :
            //mGameModel->debugBoard(false);
            mGameModel->getNewBoard()->debugOutput(false);
        break;
        case ley::Command::quit :
            mGameModel->setGameRunning(false);
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::decreaseVolume :
            mStatusFont.updateMessage(mGameModel->getLanguageModel()->getWord("volume down", 0, false, capitalizationtype::capitalizeFirst));
            statusTimer.reset();
        break;
        case ley::Command::increaseVolume :
            mStatusFont.updateMessage(mGameModel->getLanguageModel()->getWord("volume up", 0, false, capitalizationtype::capitalizeFirst));
            statusTimer.reset();
        break;
        case ley::Command::nextSong :
            mStatusFont.updateMessage("Next song");
            statusTimer.reset();
        break;
        case ley::Command::drop :
            if(mLastHardDrop.hasExpired()) {
                mGameModel->quickDrop();
                mLastHardDrop.reset();
            }
            fallTimer.reset();
        break;
        
        defaut:
        break;
    }

    /**** UPDATE ****/
    fallTimer.runFrame(false, mGameModel->speed());
    statusTimer.runFrame(false, 0.0);
    mLastHardDrop.runFrame(false, 0.0);
    if(statusTimer.hasExpired()) {
        mStatusFont.updateMessage("");
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
    
    mRenderables.push_back(&mStatusFont);
    mRenderables.push_back(&fallTimer);

    mDebugRenderables.push_back(&statusTimer);
}

bool PlayState::onEnter() {
    resetGame();
    SDL_Log("Entering PlayState and loading renderables");
    mVideoSystem->resetBackgroundFader();

    SDL_Log("Loading PlayState renderables");
    loadRenderables();

    //make sure the board reflects the current config
    SDL_Log("Resizing Board in PlayState");
    // TODO maybe this should move to where the value changes, so we call it only if needed.
    mGameModel->readConfigOther();
    

    SDL_Log("Resizing videosystem board in PlayState");
    mVideoSystem->videoResizeBoard();

    //make sure the falltimer width reflects the correct boardsize.
    SDL_Log("Resizing the falltimer.");
    fallTimer(1000,{mGameModel->getBoard()->boardPosXPx()-1,BOARD_POS_Y_PX+mGameModel->getBoard()->heightpx()-(BOARDSIZE_BUFFER*BLOCKSIZE_PX)+1,mGameModel->getBoard()->widthpx()+2,2});

    #ifdef FULL_ASSETS
        SDL_Log("Full Assets loaded!");
    #elif !FULL_ASSETS
        SDL_Log("Minimal Assets loaded!");
    #endif

    mCurrentInputContext = "play";

    return true;
}

bool PlayState::onReEnter() {
    if(fallTimer.isPaused()) {
        fallTimer.pause(false);
    }
    SDL_Log("ReEntering PlayState");

    mCurrentInputContext = "play";

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