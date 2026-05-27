#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

namespace ley {

// TODO should the commands just be simple strings? We use string in the config file.
enum class Command {
    down,
    right,
    up,
    left,
    shiftdown, //used for shifting blocks in the block editor
    shiftright,
    shiftup,
    shiftleft,
    shiftmajup, // shift an entire blocks set of orientations all at the same time.
    shiftmajdown,
    shiftmajright,
    shiftmajleft,
    shiftallup,
    shiftalldown,
    shiftallright,
    shiftallleft,
    restoredefault,
    none,
    pause,
    drop,
    console,
    cclockwise,
    clockwise,
    debugkeystoggle,
    debugcolide,
    debugtexture,
    debugnextlevel,
    debugprevlevel,
    debugclear,
    debugfill,
    debugonlyline,
    enter,
    quit,
    increaseVolume,
    decreaseVolume,
    increaseTransparency,
    decreaseTransparency,
    nextSong,
    previousSong,
    tab,
    backspace,
    fullscreen,
    UI_up,
    UI_down,
    UI_left,
    UI_right,
    UI_enter,
    UI_back,
    UI_add};

typedef struct {
    std::pair<ley::Command, std::vector<Uint8>> down;
    std::pair<ley::Command, std::vector<Uint8>> right;
    std::pair<ley::Command, std::vector<Uint8>> up;
    std::pair<ley::Command, std::vector<Uint8>> left;
    std::pair<ley::Command, std::vector<Uint8>> none;
    std::pair<ley::Command, std::vector<Uint8>> pause;
    std::pair<ley::Command, std::vector<Uint8>> space;
    std::pair<ley::Command, std::vector<Uint8>> cclockwise;
    std::pair<ley::Command, std::vector<Uint8>> clockwise;
    std::pair<ley::Command, std::vector<Uint8>> enter;
    std::pair<ley::Command, std::vector<Uint8>> quit;
} ButtonBindings;

extern const std::unordered_map<ley::Command, std::string> COMMANDTOSTRING;

extern const std::unordered_map<std::string, ley::Command> STRINGTOCOMMAND;

extern const std::unordered_map<SDL_GameControllerButton, std::string> BUTTONTOSTRING;

extern const std::unordered_map<std::string, SDL_GameControllerButton> STRINGTOBUTTON;

extern const std::unordered_map<std::string, SDL_Keymod> STRINGTOKMOD;

extern const std::unordered_map<SDL_Keymod, std::string> KMODTOSTRING;

extern const std::unordered_map<SDL_Scancode, std::string> SCANCODETOSTRING;

extern const std::unordered_map<std::string, SDL_Scancode> STRINGTOSCANCODE;

};

#endif