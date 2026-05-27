#ifndef COMMAND_H
#define COMMAND_H

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>

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

// Lookup tables declared here; defined once in Command.cpp.
extern const std::unordered_map<ley::Command, std::string>          COMMANDTOSTRING;
extern const std::unordered_map<std::string,  ley::Command>          STRINGTOCOMMAND;
extern const std::unordered_map<SDL_GameControllerButton, std::string> BUTTONTOSTRING;
extern const std::unordered_map<std::string,  SDL_GameControllerButton> STRINGTOBUTTON;
extern const std::unordered_map<std::string,  SDL_Keymod>             STRINGTOKMOD;
extern const std::unordered_map<SDL_Keymod,   std::string>            KMODTOSTRING;
extern const std::unordered_map<SDL_Scancode, std::string>            SCANCODETOSTRING;
extern const std::unordered_map<std::string,  SDL_Scancode>           STRINGTOSCANCODE;

} // namespace ley

#endif
