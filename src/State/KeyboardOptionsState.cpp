#include "../../inc/State/KeyboardOptionsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string KeyboardOptionsState::sKeyboardOptionsID = "KEYBOARDOPTIONS";


KeyboardOptionsState::KeyboardOptionsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50},
    mButtonTitleFont{20,420,100,50},
    mDirectionsFont{20,420,100,50} {

    mMainUI.setWidth(2);
    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("keyboard inputs", 0, false, capitalizationtype::capitalizeFirst));
    mButtonTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("gamepad inputs", 0, false, capitalizationtype::capitalizeFirst));
    mDirectionsFont.updateMessage("Enter the button for this command");
    mDirectionsFont.bottom(SCREEN_HEIGHT, 1);
    mDirectionsFont.left();
    mDirectionsFont.setVisible(false);

    initalizeMenu();
}

/*
** returns the pixel width value of the font.
*/
//TODO this method needs to go into font or somewhere as a static

void KeyboardOptionsState::initalizeMenu() {

    mMainUI.clear();
    mLabelFonts.clear();
    mValueFonts.clear();

    struct inputOptionsObject {             
        std::string l_elementId;
        std::string l_label;
        ley::Command l_command;
        std::string r_elementId;
        std::string r_label;
        ley::Command r_command;
    };
    std::vector<inputOptionsObject> inputObjects;

    inputObjects.push_back({"K_left", "left", ley::Command::left , "K_nextSong", "Next Song", ley::Command::nextSong});
    inputObjects.push_back({"K_right", "right", ley::Command::right , "K_pause", "Pause Game", ley::Command::pause});
    inputObjects.push_back({"K_down", "down", ley::Command::down , "K_decreaseVolume", "Decrease Volume", ley::Command::decreaseVolume});
    inputObjects.push_back({"K_cclockwise", "counter clockwise", ley::Command::cclockwise, "K_increaseVolume", "Increase Volume", ley::Command::increaseVolume});
    inputObjects.push_back({"K_clockwise", "clockwise", ley::Command::clockwise, "K_drop", "quick drop", ley::Command::drop});

    Uint16 startY = 60;
    Uint16 col2Y = 60;
    Uint16 col2X = 575;
    for(const inputOptionsObject& inputObject : inputObjects) {
        startY+=40;
        col2Y+= 40;
        //create an item in left column
        if(inputObject.l_command != ley::Command::none) {
            mLabelFonts.push_back({20, startY, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(inputObject.l_label, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), startY, 400, 40});
            mValueFonts.back().updateMessage(mGameModel->getKeyInputString(",", inputObject.l_command, mGameModel->getKeyBindingsPtr()));
            mMainUI.pushFont(inputObject.l_elementId, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(inputObject.l_elementId)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(inputObject.l_elementId)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }
        //create an item in the right column
        if(inputObject.r_command != ley::Command::none) {
            mLabelFonts.push_back({col2X, col2Y, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(inputObject.r_label, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), startY, 400, 40});
            mValueFonts.back().updateMessage(mGameModel->getKeyInputString(",", inputObject.r_command, mGameModel->getKeyBindingsPtr()));
            mMainUI.pushFont(inputObject.r_elementId, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(inputObject.r_elementId)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(inputObject.r_elementId)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }
    }

    std::vector<std::pair<std::pair<std::string, ley::Command>,std::pair<std::string, ley::Command>>> buttonStrings2;
    buttonStrings2.push_back(std::make_pair(std::make_pair("left", ley::Command::left),std::make_pair("Counter clockwise", ley::Command::cclockwise)));
    buttonStrings2.push_back(std::make_pair(std::make_pair("right", ley::Command::right),std::make_pair("Clockwise", ley::Command::clockwise)));
    buttonStrings2.push_back(std::make_pair(std::make_pair("down", ley::Command::down),std::make_pair("quick drop", ley::Command::drop)));
    buttonStrings2.push_back(std::make_pair(std::make_pair("quit", ley::Command::quit),std::make_pair("Enter", ley::Command::UI_enter)));
    
    Uint16 colButtonY = 460;
    for(const std::pair<std::pair<std::string, ley::Command>,std::pair<std::string, ley::Command>>& command : buttonStrings2) {
        colButtonY+=40;

        //create an item in left column
        if(command.first.second != ley::Command::none) {
            mLabelFonts.push_back({20, colButtonY, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.first.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), colButtonY, 400, 40});
            mValueFonts.back().updateMessage(mGameModel->getPadInputString(",", command.first.second, mGameModel->getButtonBindingsPtr()));
            mMainUI.pushFont(command.first.first, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(command.first.first)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(command.first.first)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }

        //create an item in the right column
        if(command.second.second != ley::Command::none) {
            mLabelFonts.push_back({500, colButtonY, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.second.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), colButtonY, 400, 40});
            mValueFonts.back().updateMessage(mGameModel->getPadInputString(",", command.second.second, mGameModel->getButtonBindingsPtr()));
            mMainUI.pushFont(command.second.first, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(command.second.first)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(command.second.first)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }
    }
}

int KeyboardOptionsState::fontWidth(ley::Font* inFont) {
    int w = 0;
    int h = 0;

    TTF_SizeUTF8(inFont->getTTFFont(), inFont->getMessage().c_str(), &w, &h);

    return w;
}

void KeyboardOptionsState::update(ley::Command command) {

    switch (command) {
        case ley::Command::UI_back :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::UI_add :
            mAddMapping = true;
        case ley::Command::UI_enter :
            SDL_Log("UI_Enter!!");
            mDirectionsFont.setVisible(true);
            SDL_Log(std::to_string(mGameModel->getLastScancode()).c_str());
            mCaptureNewInput = true;
            SDL_Log("Menu element Id: %s", mMainUI.getCurrentElementPtr()->getLabel().c_str());
        break;
    }

    if(!mCaptureNewInput) {
        mMainUI.runCommand(command);
    }

    if((mGameModel->getLastScancode() != SDL_SCANCODE_RETURN
        && mGameModel->getLastScancode() != SDL_SCANCODE_KP_PLUS)
        && mCaptureNewInput) {

        mDirectionsFont.setVisible(false);
        mCaptureNewInput = false;
        reassignKey(mMainUI.getCurrentElementPtr()->getLabel().c_str(), mAddMapping);
        mAddMapping = false;
        initalizeMenu();
    }
}

void KeyboardOptionsState::reassignKey(std::string keycode, bool addMapping) {
    std::string delimiter = "_";
    size_t delimiter_pos = keycode.find(delimiter);

    std::string prefix, suffix;

    if (delimiter_pos != std::string::npos) {
        // Extract prefix (before delimiter)
        prefix = keycode.substr(0, delimiter_pos);
        // Extract suffix (after delimiter)
        suffix = keycode.substr(delimiter_pos + delimiter.length());
    } else {
        // No delimiter found, treat the whole string as prefix
        prefix = keycode;
        suffix = "";
    }

    SDL_Log("Prefix: %s, Suffix: %s", prefix.c_str(), suffix.c_str());

    // Keyboard input
    if (prefix == "K") {
        reassignKeyboard(STRINGTOCOMMAND.at(suffix), mGameModel->getLastScancode(), addMapping);
        SDL_Log("Key reassign: %s", suffix.c_str());
    }
    // Button input
    else if (prefix == "B") {
        SDL_Log("Button reassign: %s", suffix.c_str());
    }
}
std::vector<KeyBindingsType::const_iterator> KeyboardOptionsState::findIteratorsByValue(
    const KeyBindingsType* bindings,
    const std::pair<SDL_Keymod, ley::Command>& targetValue) {
    
    std::vector<KeyBindingsType::const_iterator> matchingRows;

    for (auto it = bindings->begin(); it != bindings->end(); ++it) {
        if (it->second == targetValue) {            
            matchingRows.push_back(it);
        }
    }

    return matchingRows;
}

std::vector<std::pair<SDL_Scancode, std::string>> KeyboardOptionsState::findKeysByValue(
    const KeyBindingsType* bindings,
    const std::pair<SDL_Keymod, ley::Command>& targetValue) {
    
    std::vector<std::pair<SDL_Scancode, std::string>> matchingKeys;

    for (const auto& entry : (*bindings)) {
        if (entry.second == targetValue) {
            matchingKeys.push_back(entry.first);
        }
    }

    return matchingKeys;
}

void KeyboardOptionsState::reassignKeyboard(ley::Command command, SDL_Scancode scancode, bool addMapping) {

    KeyBindingsType keyBinding;
    KeyBindingValue keyBindingValue;

    //Only delete if we are not adding.
    if(!addMapping) {
        for(KeyBindingsType::const_iterator it : findIteratorsByValue(mGameModel->getKeyBindingsPtr(), {KMOD_NONE, command})) {
            mGameModel->getKeyBindingsPtr()->erase(it);
        }
    }


    bool exists = false;
    if(findIteratorsByValue(mGameModel->getKeyBindingsPtr(), {KMOD_NONE, command}).size() == 0) {
        mGameModel->getKeyBindingsPtr()->emplace(std::make_pair(scancode,"play"), std::make_pair(KMOD_NONE, command));
    }
    else {
        for(KeyBindingsType::const_iterator it : findIteratorsByValue(mGameModel->getKeyBindingsPtr(), {KMOD_NONE, command})) {
            if(it->first.first == scancode) {
                exists = true;
                break;
            }
        }

        if(!exists) {
            mGameModel->getKeyBindingsPtr()->emplace(std::make_pair(scancode,"play"), std::make_pair(KMOD_NONE, command));
        }
    }

    
}

void KeyboardOptionsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), mGameModel->isOverlayOn());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
    
    mMainUI.render(mVideoSystem);
}

void KeyboardOptionsState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mButtonTitleFont);
    mRenderables.push_back(&mDirectionsFont);

    for(auto &font : mLabelFonts) {
        mRenderables.push_back(&font);
    }
}

bool KeyboardOptionsState::onEnter() {
    SDL_Log("Entering KeyboardOptionsState");
    loadRenderables();
    mCurrentInputContext = "ui";

    return true;
}

bool KeyboardOptionsState::onReEnter() {
    SDL_Log("ReEntering KeyboardOptionsState");
    mCurrentInputContext = "ui";

    return true;
}

bool KeyboardOptionsState::onExit() {
    SDL_Log("Exiting KeyboardOptionsState");

    mActiveUIElement = {};
    return true;
}

bool KeyboardOptionsState::onPause() {
    GameState::onPause();
    
    return true;
}

}