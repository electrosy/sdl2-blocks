#include "../../inc/State/LanguageOptionsState.h"
#include "../../inc/LanguageModel.h"
#include "../../inc/ConfigIO.h"
#include "../../inc/gfx/Font.h"


typedef ley::Textures TextureManager;

namespace ley {


LanguageOptionsState::LanguageOptionsState(ley::Video * v, ley::GameModel * gm):
    BaseState(v, gm),
    mTitleFont{20,20,100,50},
    mCurrentLanguageFont{20,70,100,50} {

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeWords));

    // Data-driven list from LanguageModel ordered codes (not hardcoded en/es indices).
    const auto& codes = mGameModel->getLanguageModel()->getLanguageCodes();
    // 14 languages on 720p: tighter row spacing + slightly smaller font so all fit.
    const int startY = 130;
    const int rowSpacing = 28;
    const int rowFontSize = 20;
    for (size_t i = 0; i < codes.size(); ++i) {
        const std::string& code = codes[i];
        // Native names on the picker (Français, 日本語, …) — not translated labels.
        const std::string label = mGameModel->getLanguageModel()->getNativeLanguageName(code);
        const int y = startY + static_cast<int>(i) * rowSpacing;
        mLanguageUI.pushFont("lang_" + code, {29, y}, label, v->getRenderer(), rowFontSize);
        // Native names are not in the active UI font. MartianMono has no CJK, and
        // Noto JP/SC/KR do not cover each other (KR-only Hangul, SC-only 简).
        // Pin each row to its own script so all 14 names render in every UI language.
        // ru/uk stay on MartianMono; Noto CJK is missing Ukrainian ї.
        if (ley::UIElement* el = mLanguageUI.getElementPtr("lang_" + code)) {
            el->pinFontsToLanguage(code);
        }
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
        const int idx = mLanguageUI.getIndex();
        const auto& codes = mGameModel->getLanguageModel()->getLanguageCodes();
        if (idx >= 0 && idx < static_cast<int>(codes.size())) {
            const std::string& code = codes[static_cast<size_t>(idx)];
            mGameModel->getLanguageModel()->setLanguage(code);
            mGameModel->getLanguageModel()->loadLanguage();
            // Reload title/list fonts for CJK switch and persist selection.
            mTitleFont.reloadForActiveLanguage();
            mCurrentLanguageFont.reloadForActiveLanguage();
            updateLanguageFont();
            ConfigIO::writeMainConfig(mGameModel);
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
    
    mCurrentLanguageFont.updateMessage(
        mGameModel->getLanguageModel()->getWord("current language", 0, false, capitalizationtype::capitalizeWords)
        + ": " + mGameModel->getLanguageModel()->getLanguageString());

    const auto& codes = mGameModel->getLanguageModel()->getLanguageCodes();
    for (const auto& code : codes) {
        const std::string elementId = "lang_" + code;
        ley::UIElement* el = mLanguageUI.getElementPtr(elementId);
        if (!el) {
            continue;
        }
        el->setMessage(mGameModel->getLanguageModel()->getNativeLanguageName(code));
        // Re-pin after a language switch. reloadForActiveLanguage() would put every
        // row on the UI font and turn the other scripts back into tofu.
        el->pinFontsToLanguage(code);
    }

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeWords));
}

}
