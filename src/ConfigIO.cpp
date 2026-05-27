/*
sdl2-blocks
Copyright (C) 2020-2025 Steven Philley

Purpose: see header.
*/
#include <fstream>
#include <filesystem>
#include <functional>
#include <sstream>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "../inc/ConfigIO.h"
#include "../inc/GameModel.h"
// Board-size constants are now in ConfigIO.h

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

/* ── Block data write ───────────────────────────────────────────────────── */

void ley::ConfigIO::writeBlockData(const std::vector<std::string>& rows) {

    std::ofstream f("blocks.csv");
    for (const std::string& row : rows) {
        SDL_Log("ConfigIO::writeBlockData row: %s", row.c_str());
        f << row << '\n';
    }
}

/* ── Restore default blocks ─────────────────────────────────────────────── */

bool ley::ConfigIO::restoreDefaultBlocks() {

    namespace fs = std::filesystem;

    fs::path src  = fs::absolute("blocks-default.csv");
    fs::path dest = fs::absolute("blocks.csv");

    SDL_Log("ConfigIO::restoreDefaultBlocks: %s → %s",
            src.string().c_str(), dest.string().c_str());

    if (!fs::exists(src)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Source file does not exist: %s", src.string().c_str());
        return false;
    }

    // Ensure dest is writable if it already exists
    if (fs::exists(dest)) {
        std::error_code permEc;
        fs::permissions(dest,
                        fs::status(dest).permissions() | fs::perms::owner_write,
                        fs::perm_options::replace, permEc);
        if (permEc) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Cannot set write permissions on dest: %s",
                         permEc.message().c_str());
            return false;
        }

        std::error_code rmEc;
        fs::remove(dest, rmEc);
        if (rmEc) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Cannot remove existing dest: %s", rmEc.message().c_str());
            return false;
        }
    }

    std::error_code ec;
    fs::copy(src, dest, fs::copy_options::overwrite_existing, ec);
    if (ec) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "File copy failed: %s (code %d)", ec.message().c_str(), ec.value());
        return false;
    }

    SDL_Log("ConfigIO::restoreDefaultBlocks: copy succeeded.");
    return true;
}

/* ── UI context default bindings ────────────────────────────────────────── */
// These are always-on hardcoded bindings for the "ui" context (menus,
// overlays, block editor). Play-context bindings come from CSV via
// readKeyboardBindings / readGamepadBindings.

void ley::ConfigIO::applyUiKeyDefaults(KeyBindingsType* out) {
    // Each entry: { scancode, modifier, command }
    static const struct { SDL_Scancode sc; SDL_Keymod mod; Command cmd; } TABLE[] = {
        { SDL_SCANCODE_LEFT,      KMOD_NONE,  Command::UI_left        },
        { SDL_SCANCODE_LEFT,      KMOD_ALT,   Command::shiftleft      },
        { SDL_SCANCODE_LEFT,      KMOD_SHIFT, Command::shiftmajleft   },
        { SDL_SCANCODE_LEFT,      KMOD_CTRL,  Command::shiftallleft   },
        { SDL_SCANCODE_RIGHT,     KMOD_NONE,  Command::UI_right       },
        { SDL_SCANCODE_RIGHT,     KMOD_ALT,   Command::shiftright     },
        { SDL_SCANCODE_RIGHT,     KMOD_SHIFT, Command::shiftmajright  },
        { SDL_SCANCODE_RIGHT,     KMOD_CTRL,  Command::shiftallright  },
        { SDL_SCANCODE_DOWN,      KMOD_NONE,  Command::UI_down        },
        { SDL_SCANCODE_DOWN,      KMOD_SHIFT, Command::shiftmajdown   },
        { SDL_SCANCODE_DOWN,      KMOD_ALT,   Command::shiftdown      },
        { SDL_SCANCODE_DOWN,      KMOD_CTRL,  Command::shiftalldown   },
        { SDL_SCANCODE_UP,        KMOD_NONE,  Command::UI_up          },
        { SDL_SCANCODE_UP,        KMOD_ALT,   Command::shiftup        },
        { SDL_SCANCODE_UP,        KMOD_SHIFT, Command::shiftmajup     },
        { SDL_SCANCODE_UP,        KMOD_CTRL,  Command::shiftallup     },
        { SDL_SCANCODE_ESCAPE,    KMOD_NONE,  Command::UI_back        },
        { SDL_SCANCODE_RETURN,    KMOD_NONE,  Command::UI_enter       },
        { SDL_SCANCODE_RETURN,    KMOD_ALT,   Command::fullscreen     },
        { SDL_SCANCODE_BACKSPACE, KMOD_NONE,  Command::backspace      },
        { SDL_SCANCODE_DELETE,    KMOD_NONE,  Command::backspace      },
        { SDL_SCANCODE_D,         KMOD_RCTRL, Command::restoredefault },
        { SDL_SCANCODE_KP_PLUS,   KMOD_NONE,  Command::UI_add         },
    };
    for (const auto& e : TABLE)
        out->emplace(std::make_pair(e.sc, "ui"), std::make_pair(e.mod, e.cmd));
}

void ley::ConfigIO::applyUiButtonDefaults(PadBindingsType* out) {
    static const struct { SDL_GameControllerButton btn; Command cmd; } TABLE[] = {
        { SDL_CONTROLLER_BUTTON_DPAD_UP,       Command::UI_up    },
        { SDL_CONTROLLER_BUTTON_DPAD_DOWN,     Command::UI_down  },
        { SDL_CONTROLLER_BUTTON_DPAD_LEFT,     Command::UI_left  },
        { SDL_CONTROLLER_BUTTON_DPAD_RIGHT,    Command::UI_right },
        { SDL_CONTROLLER_BUTTON_A,             Command::UI_enter },
        { SDL_CONTROLLER_BUTTON_B,             Command::UI_enter },
        { SDL_CONTROLLER_BUTTON_Y,             Command::UI_enter },
        { SDL_CONTROLLER_BUTTON_X,             Command::UI_enter },
        { SDL_CONTROLLER_BUTTON_START,         Command::UI_enter },
        { SDL_CONTROLLER_BUTTON_BACK,          Command::UI_back  },
        { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Command::UI_add   },
    };
    for (const auto& e : TABLE)
        out->insert({{e.btn, "ui"}, e.cmd});
}
