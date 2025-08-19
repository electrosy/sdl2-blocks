#include "../../inc/State/CreditsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string CreditsState::sCreditsID = "CREDITS";

CreditsState::CreditsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("credits"), 0, {}, {1000,{0,0,0,0}})),
//    mCreditsPanel(ley::Sprite(TextureManager::Instance()->getTexture("credits-panel"), 0, {}, {0,{0,0,0,0}})),
    mNameFont{310,200,100,50},
    mMainRolesFont{310,235,100,50},
    mMusicByFont{645,200,100,50},
    mMusicByNamesFont{645,235,100,50},
    mSpecialThanksFont{500,495,100,50},
    mSpecialThanksNamesFont{500,530,100,50}, 
    mSoundEffectsFont{645,370,100,50},
    mSoundEffectsNamesFont{645,405,100,50},
    mAblockalypseUsesFont{310,370,100,50},
    mAblockalypseUsesNamesFont{310,405,100,50} {

//    mCreditsPanel.center();

    mNameFont.updateMessage("Steven Philley");
    mMainRolesFont.setFontSize(14);

    std::string mainCredits;
    mainCredits = mGameModel->getLanguageModel()->getWord("programming", 0, false, capitalizationtype::capitalizeWords);
    mainCredits += "\n" + mGameModel->getLanguageModel()->getWord("photography", 0, false, capitalizationtype::capitalizeWords);
    mainCredits += "\n" + mGameModel->getLanguageModel()->getWord("3d modeling", 0, false, capitalizationtype::capitalizeWords);
    mainCredits += "\n" + mGameModel->getLanguageModel()->getWord("artwork", 0, false, capitalizationtype::capitalizeWords);
    mainCredits += "\n" + mGameModel->getLanguageModel()->getWord("sound engineering", 0, false, capitalizationtype::capitalizeWords);
    mMainRolesFont.updateMessage(mainCredits);

    mMusicByFont.updateMessage(mGameModel->getLanguageModel()->getWord("featuring music by", 0, false, capitalizationtype::capitalizeWords));
    
    std::string musicByNames;
    musicByNames = "Matthew Pablo (matthewpablo.com)\nTAD\nAlex McCulloch\nSpring Spring";
    mMusicByNamesFont.setFontSize(14);
    mMusicByNamesFont.updateMessage(musicByNames);

    mSpecialThanksFont.updateMessage(mGameModel->getLanguageModel()->getWord("special thanks", 0, false, capitalizationtype::capitalizeWords));

    mSpecialThanksNamesFont.setFontSize(14);
    mSpecialThanksNamesFont.updateMessage("Lazy Foo' (lazyfoo.net)\nopengameart.org\nSPRITELIB (widgetworx.com)");

    mSoundEffectsFont.updateMessage(mGameModel->getLanguageModel()->getWord("sound effects", 0, false, capitalizationtype::capitalizeWords));

    mSoundEffectsNamesFont.setFontSize(14);
    mSoundEffectsNamesFont.updateMessage("pixabay");

    mAblockalypseUsesFont.updateMessage("Ablockalypse " + mGameModel->getLanguageModel()->getWord("uses", 0, false, capitalizationtype::capitalizeWords));

    mAblockalypseUsesNamesFont.setFontSize(14);
    mAblockalypseUsesNamesFont.updateMessage("SDL (libsdl.org)");

    mActiveUIElement = {};
}

void CreditsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    optionUI.runCommand(command);
}

void CreditsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    optionUI.render(mVideoSystem);
}

void CreditsState::loadRenderables() {
    mRenderables.push_back(&mBackground);
    //mRenderables.push_back(&mCreditsPanel);
    mRenderables.push_back(&mNameFont);
    mRenderables.push_back(&mMainRolesFont);
    mRenderables.push_back(&mMusicByFont);
    mRenderables.push_back(&mMusicByNamesFont);
    mRenderables.push_back(&mSpecialThanksFont);
    mRenderables.push_back(&mSpecialThanksNamesFont);
    mRenderables.push_back(&mSoundEffectsFont);
    mRenderables.push_back(&mSoundEffectsNamesFont);
    mRenderables.push_back(&mAblockalypseUsesFont);
    mRenderables.push_back(&mAblockalypseUsesNamesFont);
}

bool CreditsState::onEnter() {
    SDL_Log("Entering CreditsState");
    mCurrentInputContext = "play";
    loadRenderables();
    return true;
}

bool CreditsState::onReEnter() {
    SDL_Log("ReEntering CreditsState");
    mCurrentInputContext = "play";
    return true;
}

bool CreditsState::onExit() {
    SDL_Log("Exiting CreditsState");
    return true;
}

bool CreditsState::onPause() {
    GameState::onPause();
    
    return true;
}

}