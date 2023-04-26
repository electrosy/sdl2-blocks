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
fourthTimer(v->getRenderer(),333,{10,455,100,5}) {

}

void PlayState::update(ley::Command command) {
    firstTimer.runFrame();
    secondTimer.runFrame();
    thirdTimer.runFrame();
    fourthTimer.runFrame();
}

void PlayState::loadRenderables() {
    mVideoSystem->addRenderable(true, &firstTimer);
    mVideoSystem->addRenderable(true, &secondTimer);
    mVideoSystem->addRenderable(true, &thirdTimer);
    mVideoSystem->addRenderable(true, &fourthTimer);
}

bool PlayState::onEnter() {
    
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