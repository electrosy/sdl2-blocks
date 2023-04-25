#include "../../inc/State/PlayState.h"

namespace ley {

const std::string PlayState::sPlayID = "PLAY";

PlayState::PlayState(ley::Video * v)
:
videoSystem(v),
firstTimer(v->getRenderer(),3000,{10,300,100,50}),
secondTimer(v->getRenderer(),2500,{10,400,100,25}),
thirdTimer(v->getRenderer(),1000,{10,425,100,30}),
fourthTimer(v->getRenderer(),333,{10,455,100,5}) {

}

void PlayState::update(ley::Command command, ley::GameModel * gm) {
    firstTimer.runFrame();
    secondTimer.runFrame();
    thirdTimer.runFrame();
    fourthTimer.runFrame();
}

void PlayState::loadRenderables() {
    videoSystem->addRenderable(true, &firstTimer);
    videoSystem->addRenderable(true, &secondTimer);
    videoSystem->addRenderable(true, &thirdTimer);
    videoSystem->addRenderable(true, &fourthTimer);
}

bool PlayState::onEnter() {
    SDL_Log("Entering PlayState and loading renderables");
    loadRenderables();
    return true;
}

bool PlayState::onExit() {
    SDL_Log("Exiting Playstate");
    return true;
}

}