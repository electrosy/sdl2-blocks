/*
sdl2-blocks
Copyright (C) 2020-2025 Steven Philley

Purpose: see header.
*/
#include <fstream>
#include <functional>
#include <sstream>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "../inc/ConfigIO.h"
#include "../inc/GameModel.h"
#include "../inc/Config.h"

/* ── Main config ────────────────────────────────────────────────────────── */

void ley::ConfigIO::readMainConfig(ley::GameModel* gm) {

    const std::unordered_map<std::string, std::function<void(const std::string&)>> handlers = {
        {"language",        [gm](const std::string& v){ gm->getLanguageModel()->setLanguage(v); }},
        {"keydelay",        [gm](const std::string& v){ gm->setKeyDelay(std::stoi(v)); }},
        {"keyrepeat",       [gm](const std::string& v){ gm->setKeyRepeat(std::stoi(v)); }},
        {"guidegridon",     [gm](const std::string& v){ gm->setGuideGridOn(v); }},
        {"wallkickon",      [gm](const std::string& v){ gm->setWallKickOn(v == "on"); }},
        {"dropcooldown",    [gm](const std::string& v){ gm->setDropCoolDown(std::stoi(v)); }},
        {"showprogressbar", [gm](const std::string& v){ gm->setShowProgressBar(v == "on"); }},
        {"startlevel",      [gm](const std::string& v){ gm->setStartLevel(std::stoi(v)); }},
    };

    std::ifstream inFile("mainconfig.csv");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string key, value;
            std::getline(ss, key, ',');
            std::getline(ss, value, ',');
            if (!key.empty() && !value.empty()) {
                auto it = handlers.find(key);
                if (it != handlers.end()) {
                    it->second(value);
                }
            }
        }
    }
}

void ley::ConfigIO::writeMainConfig(const ley::GameModel* gm) {

    std::ofstream f("mainconfig.csv");
    f << "language"        << ',' << gm->getLanguageModel()->getLanguage()       << '\n';
    f << "keydelay"        << ',' << gm->getKeyDelay()                           << '\n';
    f << "keyrepeat"       << ',' << gm->getKeyRepeat()                          << '\n';
    f << "guidegridon"     << ',' << gm->getGuideGridOnString()                  << '\n';
    f << "wallkickon"      << ',' << (gm->getWallKickOn() ? "on" : "off")        << '\n';
    f << "dropcooldown"    << ',' << std::to_string(gm->getDropCoolDown())       << '\n';
    f << "showprogressbar" << ',' << (gm->getShowProgressBar() ? "on" : "off")   << '\n';
    f << "startlevel"      << ',' << gm->getStartLevel()                         << '\n';
}

/* ── Keyboard bindings ──────────────────────────────────────────────────── */

void ley::ConfigIO::readKeyboardBindings(std::vector<ley::KeyBindingRow>* out) {

    std::ifstream inFile("keyboard-config.csv");
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string sScanCode, sModifier, sCommand;
        std::getline(ss, sScanCode,  ',');
        std::getline(ss, sModifier,  ',');
        std::getline(ss, sCommand);

        SDL_Log((sScanCode + "," + sCommand + "," + sModifier).c_str());
        if (!sScanCode.empty() && !sCommand.empty()) {
            out->push_back({ STRINGTOSCANCODE.at(sScanCode),
                             { STRINGTOKMOD.at(sModifier), STRINGTOCOMMAND.at(sCommand) } });
        }
    }
}

void ley::ConfigIO::writeKeyboardBindings(const ley::KeyBindingsType& bindings) {

    std::ofstream f("keyboard-config.csv");
    for (const auto& binding : bindings) {
        if (binding.first.second != "play") continue;
        f << SCANCODETOSTRING.at(binding.first.first)  << ','
          << KMODTOSTRING.at(binding.second.first)      << ','
          << COMMANDTOSTRING.at(binding.second.second)  << '\n';
    }
}

/* ── Gamepad bindings ───────────────────────────────────────────────────── */

void ley::ConfigIO::readGamepadBindings(std::vector<ley::ControllerButtonRow>* out) {

    std::ifstream inFile("gamepad-config.csv");
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string sButton, sCommand;
        std::getline(ss, sButton,  ',');
        std::getline(ss, sCommand, ',');

        SDL_Log((sButton + "," + sCommand).c_str());
        if (!sButton.empty() && !sCommand.empty()) {
            out->push_back({ STRINGTOBUTTON.at(sButton), STRINGTOCOMMAND.at(sCommand) });
        }
    }
}

void ley::ConfigIO::writeGamepadBindings(const ley::PadBindingsType& bindings) {

    std::ofstream f("gamepad-config.csv");
    for (const auto& binding : bindings) {
        if (binding.first.second != "play") continue;
        f << BUTTONTOSTRING.at(binding.first.first)  << ','
          << COMMANDTOSTRING.at(binding.second)       << '\n';
    }
}

/* ── Block data ─────────────────────────────────────────────────────────── */

void ley::ConfigIO::readBlockData(ley::BlockFileDataMapType* out) {

    SDL_Log("ley::ConfigIO::readBlockData()");
    out->clear();

    std::ifstream inFile("blocks.csv");
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string key, value;
        std::getline(ss, key,   ',');
        std::getline(ss, value, ',');
        if (!key.empty() && !value.empty()) {
            out->emplace(key, value);
        }
    }
}

/* ── Board size (config.csv) ────────────────────────────────────────────── */

ley::ConfigIO::BoardSize ley::ConfigIO::readBoardSize() {

    BoardSize result{BOARDWIDTH_DEFAULT, BOARDHEIGHT_DEFAULT};

    std::ifstream inFile("config.csv");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string sWidth, sHeight;
            std::getline(ss, sWidth,  'x');
            std::getline(ss, sHeight, 'x');
            if (!sWidth.empty())  result.width  = std::stoi(sWidth);
            if (!sHeight.empty()) result.height = std::stoi(sHeight);
        }
    }

    if (result.width  == 0) result.width  = BOARDWIDTH_DEFAULT;
    if (result.height == 0) result.height = BOARDHEIGHT_DEFAULT;

    if (result.width  > BOARDWIDTH_MAX  || result.width  < BOARDWIDTH_MIN)  result.width  = BOARDWIDTH_DEFAULT;
    if (result.height > BOARDHEIGHT_MAX || result.height < BOARDHEIGHT_MIN) result.height = BOARDHEIGHT_DEFAULT;

    SDL_Log("ConfigIO::readBoardSize() → %d x %d", result.width, result.height);
    return result;
}
