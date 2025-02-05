#include "../../inc/State/LanguageOptionsState.h"


typedef ley::Textures TextureManager;

namespace ley {

const std::string LanguageOptionsState::sLanguageOptionsID = "LANGUAGEOPTIONS";

LanguageOptionsState::LanguageOptionsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50},
    mCurrentLanguageFont{20,70,100,50} {

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeWords));

    mLanguageUI.pushFont("englishLanguage", {29,200,218,63}, mGameModel->getLanguageModel()->getWord("english", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);
    mLanguageUI.pushFont("spanishLanguage", {29,250,218,63}, mGameModel->getLanguageModel()->getWord("spanish", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);
    mLanguageUI.pushFont("testNewElement", {29,300,218,63}, "Testing", v->getRenderer(), 24);

    updateLanguageFont();
}

void LanguageOptionsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    if(command == ley::Command::enter && mLanguageUI.getIndex() == 0) {
        mGameModel->getLanguageModel()->setLanguage("en");
        mGameModel->getLanguageModel()->loadLanguage();
        updateLanguageFont();
    }

    if(command == ley::Command::enter && mLanguageUI.getIndex() == 1) {
        mGameModel->getLanguageModel()->setLanguage("es");
        mGameModel->getLanguageModel()->loadLanguage();
        updateLanguageFont();
    }

    if(command == ley::Command::enter && mLanguageUI.getIndex() == 2) {
        SDL_Log("This is language option 3");
        mLanguageUI.getElementPtr("testNewElement")->setMessage("New Message");
        mLanguageUI.getElementPtr("testNewElement")->setFontSize(50);
    }

    mLanguageUI.runCommand(command);
}

void LanguageOptionsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    mLanguageUI.render(mVideoSystem);
}

void LanguageOptionsState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mCurrentLanguageFont);
}

bool LanguageOptionsState::onEnter() {

    SDL_Log("Entering LanguageOptionsState");
    loadRenderables();
    return true;
}

bool LanguageOptionsState::onReEnter() {
    SDL_Log("ReEntering LanguageOptionsState");
    return true;
}

bool LanguageOptionsState::onExit() {
    SDL_Log("Exiting LanguageOptionsState");
    return true;
}

bool LanguageOptionsState::onPause() {
    GameState::onPause();
    
    return true;
}

void LanguageOptionsState::updateLanguageFont() {
    
    mCurrentLanguageFont.updateMessage(mGameModel->getLanguageModel()->getWord("current language",0,false, capitalizationtype::capitalizeWords) + ": " + mGameModel->getLanguageModel()->getLanguageString());
    
    mLanguageUI.getElementPtr("englishLanguage")->setMessage(mGameModel->getLanguageModel()->getWord("english",0,false, capitalizationtype::capitalizeFirst));
    mLanguageUI.getElementPtr("spanishLanguage")->setMessage(mGameModel->getLanguageModel()->getWord("spanish",0,false, capitalizationtype::capitalizeFirst));

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeWords));
}

}