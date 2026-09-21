#include "../../inc/State/LanguageOptionsState.h"
#include "../../inc/LanguageModel.h"


typedef ley::Textures TextureManager;

namespace {

constexpr int LANGUAGE_LIST_START_Y = 140;
constexpr int LANGUAGE_LIST_SPACING = 52;

std::string languageElementId(const std::string& code) {
    return code + "Language";
}

}

namespace ley {


LanguageOptionsState::LanguageOptionsState(ley::Video * v, ley::GameModel * gm):
    BaseState(v, gm),
    mTitleFont{20,20,100,50},
    mCurrentLanguageFont{20,70,100,50} {

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeWords));

    const auto& codes = mGameModel->getLanguageModel()->getLanguageCodes();
    int y = LANGUAGE_LIST_START_Y;
    for (const auto& code : codes) {
        const std::string labelKey = mGameModel->getLanguageModel()->getLanguageLabelKey(code);
        mLanguageUI.pushFont(languageElementId(code), {29, y},
            mGameModel->getLanguageModel()->getWord(labelKey, 0, false, capitalizationtype::capitalizeFirst),
            v->getRenderer(), 24);
        y += LANGUAGE_LIST_SPACING;
    }

    updateLanguageFont();

    mActiveUIElement = {};
}

void LanguageOptionsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::UI_back :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    if (command == ley::Command::UI_enter) {
        const auto& codes = mGameModel->getLanguageModel()->getLanguageCodes();
        const int index = mLanguageUI.getIndex();
        if (index >= 0 && index < static_cast<int>(codes.size())) {
            mGameModel->getLanguageModel()->setLanguage(codes[index]);
            mGameModel->getLanguageModel()->loadLanguage();
            updateLanguageFont();
        }
    }

    mLanguageUI.runCommand(command);
}

void LanguageOptionsState::render() {
    BaseState::render();
    mLanguageUI.render(mVideoSystem);
}

void LanguageOptionsState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mCurrentLanguageFont);
}

bool LanguageOptionsState::onEnter() {

    SDL_Log("Entering LanguageOptionsState");
    loadRenderables();
    mCurrentInputContext = "ui";

    return true;
}

bool LanguageOptionsState::resume() {
    SDL_Log("ReEntering LanguageOptionsState");
    mCurrentInputContext = "ui";

    return true;
}


void LanguageOptionsState::updateLanguageFont() {
    
    mCurrentLanguageFont.updateMessage(mGameModel->getLanguageModel()->getWord("current language",0,false, capitalizationtype::capitalizeWords) + ": " + mGameModel->getLanguageModel()->getLanguageString());
    
    const auto& codes = mGameModel->getLanguageModel()->getLanguageCodes();
    for (const auto& code : codes) {
        const std::string labelKey = mGameModel->getLanguageModel()->getLanguageLabelKey(code);
        ley::UIElement* element = mLanguageUI.getElementPtr(languageElementId(code));
        if (element) {
            element->setMessage(mGameModel->getLanguageModel()->getWord(labelKey, 0, false, capitalizationtype::capitalizeFirst));
        }
    }

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeWords));
}

}
