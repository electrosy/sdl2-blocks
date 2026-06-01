/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: GameModel setup, configuration, and binding methods.
         Split from GameModel.cpp — see that file for the gameplay core.
         All functions here are called during initialisation, reset, or
         in response to config changes; none run on the per-frame hot path.
*/
#ifdef DEBUG_LOGGING
#include <iostream>
#endif
#include <unordered_set>

#include <SDL2/SDL.h>
#include "../inc/GameModel.h"
#include "../inc/Board.h"
#include "../inc/ConfigIO.h"

void ley::GameModel::readConfigOther() {
    auto bs = ConfigIO::readBoardSize();
    if(bs.height >= BOARDHEIGHT_MIN && bs.width >= BOARDWIDTH_MIN) {
        resizeBoard(bs.width, bs.height);
    }
}

void ley::GameModel::resizeBoard(int width, int height) {

    mBoard->setSize(width, height + BOARDSIZE_BUFFER);
    mPts_Line = mBoard->width() * (PTS_DROP * 2);
    mBoard->putBlock(mActiveBlock);
}

void ley::GameModel::loadKeyBindings() {
    ConfigIO::applyUiKeyDefaults(&mKeyBindings);

    std::vector<KeyBindingRow> keyMappingData;
    ConfigIO::readKeyboardBindings(&keyMappingData);
    for (const KeyBindingRow& row : keyMappingData)
        mKeyBindings.insert({{row.first, "play"}, {row.second.first, row.second.second}});
}

void ley::GameModel::loadButtonBindings() {
    ConfigIO::applyUiButtonDefaults(&mButtonBindings);

    std::vector<ControllerButtonRow> buttonMappingData;
    ConfigIO::readGamepadBindings(&buttonMappingData);
    for (const ControllerButtonRow& row : buttonMappingData)
        mButtonBindings.insert({{row.first, "play"}, row.second});
}

ley::KeyBindingsType* ley::GameModel::getKeyBindingsPtr() {
    return &mKeyBindings;
}

ley::PadBindingsType* ley::GameModel::getButtonBindingsPtr() {
    return &mButtonBindings;
}

std::string ley::GameModel::getKeyInputString(const std::string& seperator, ley::Command command, KeyBindingsType* bindings) {

    std::string output = "";
    for(const auto& kv : *bindings) {
        if (kv.second.second == command) {
            if(output != "")
                output += seperator + " ";
            output += SDL_GetScancodeName(kv.first.first);
        }
    }
    return output;
}

std::string ley::GameModel::getPadInputString(const std::string& seperator, ley::Command command, PadBindingsType* bindings) {

    std::string output = "";
    for(const auto& kv : *bindings) {
        if (kv.second == command) {
            if(output != "")
                output += seperator + " ";
            // TODO can we do this without casting?
            output += (std::string)SDL_GameControllerGetStringForButton((SDL_GameControllerButton)kv.first.first);
        }
    }
    return output;
}

void ley::GameModel::setGuideGridOn(const std::string& inOn) {
    mGuideGrid = stringToGridGuide(inOn);
}

void ley::GameModel::setWallKickOn(bool on) {
    mWallKickOn = on;
}

void ley::GameModel::readBlockData() {
    // Delegate file I/O to ConfigIO, then run in-memory processing steps.
    ConfigIO::readBlockData(&mBlockMapData);
    addCanRotateToBlockData();
#ifdef DEBUG_LOGGING
    logBlockData();
#endif
}

void ley::GameModel::addCanRotateToBlockData() {

    // Extract single-letter block prefixes from keys like "a-0-3"
    std::unordered_set<std::string> blockLetters;
    for (const auto& [key, val] : mBlockMapData) {
        const auto dash = key.find('-');
        if (dash != std::string::npos && dash > 0)
            blockLetters.insert(key.substr(0, dash));
    }

    for (const std::string& prefix : blockLetters) {
        bool canRotate = false;
        for (int row = 0; row < BLOCK_SIZE && !canRotate; ++row) {
            const std::string base = prefix + "-0-" + std::to_string(row);
            for (int orient = 1; orient <= 3 && !canRotate; ++orient) {
                const std::string key = prefix + "-" + std::to_string(orient) + "-" + std::to_string(row);
                if (mBlockMapData[base] != mBlockMapData[key])
                    canRotate = true;
            }
        }
        mBlockMapData[prefix + "-*"] = canRotate ? "yes" : "no";
    }
}

void ley::GameModel::logBlockData() {
#ifdef DEBUG_LOGGING
    SDL_Log("Log Block Data:");
    for (const auto& pair : mBlockMapData) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
#endif
}

ley::BlockFileDataMapType* ley::GameModel::getFileDataPtr() {
    return &mBlockMapData;
}

void ley::GameModel::initGame() {
    mActiveBlock = getRandomBlock();
    mOldBlock = mActiveBlock;
    mOldBlock.setClear(true);
    mNextBlock = getRandomBlock();
}

bool ley::GameModel::waitForKeyDown() {
    
    if(!mWaitForKeydown) {
        mWaitForKeydown = true;
        mKeyDownEvent = false;
    }

    if(mKeyDownEvent) {
        mWaitForKeydown = false;
    }
    
    return mKeyDownEvent;
}

bool ley::GameModel::waitForButtonPress() {
    
    if(!mWaitForButtonPress) {
        mWaitForButtonPress = true;
        mButtonPressEvent = false;
    }

    if(mButtonPressEvent) {
        mWaitForButtonPress = false;
    }
    
    return mButtonPressEvent;
}
