#ifndef COMMAND_H
#define COMMAND_H

#include <unordered_map>

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
    UI_right}; //these include directions and other inputs, this is overloaded maybe a couple enums would work better?

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

const std::unordered_map<std::string, ley::Command> STRINGTOCOMMAND = {
    {"down", ley::Command::down},
    {"right", ley::Command::right},
    {"up", ley::Command::up},
    {"left", ley::Command::left},
    {"none", ley::Command::none},
    {"pause", ley::Command::pause},
    {"drop", ley::Command::drop},
    {"cclockwise", ley::Command::cclockwise},
    {"clockwise", ley::Command::clockwise},
    {"enter", ley::Command::enter},
    {"quit", ley::Command::quit},
    {"right", ley::Command::right}
};

const std::unordered_map<std::string, SDL_GameControllerButton> STRINGTOBUTTON = {
   
    {"SDL_CONTROLLER_BUTTON_A",SDL_CONTROLLER_BUTTON_A},
    {"SDL_CONTROLLER_BUTTON_B",SDL_CONTROLLER_BUTTON_B},
    {"SDL_CONTROLLER_BUTTON_X",SDL_CONTROLLER_BUTTON_X},
    {"SDL_CONTROLLER_BUTTON_Y",SDL_CONTROLLER_BUTTON_Y},
    {"SDL_CONTROLLER_BUTTON_BACK",SDL_CONTROLLER_BUTTON_BACK},
    {"SDL_CONTROLLER_BUTTON_GUIDE",SDL_CONTROLLER_BUTTON_GUIDE},
    {"SDL_CONTROLLER_BUTTON_START",SDL_CONTROLLER_BUTTON_START},
    {"SDL_CONTROLLER_BUTTON_LEFTSTICK",SDL_CONTROLLER_BUTTON_LEFTSTICK},
    {"SDL_CONTROLLER_BUTTON_RIGHTSTICK",SDL_CONTROLLER_BUTTON_RIGHTSTICK},
    {"SDL_CONTROLLER_BUTTON_LEFTSHOULDER",SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {"SDL_CONTROLLER_BUTTON_RIGHTSHOULDER",SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    {"SDL_CONTROLLER_BUTTON_DPAD_UP",SDL_CONTROLLER_BUTTON_DPAD_UP},
    {"SDL_CONTROLLER_BUTTON_DPAD_DOWN",SDL_CONTROLLER_BUTTON_DPAD_DOWN},
    {"SDL_CONTROLLER_BUTTON_DPAD_LEFT",SDL_CONTROLLER_BUTTON_DPAD_LEFT},
    {"SDL_CONTROLLER_BUTTON_DPAD_RIGHT",SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
    {"SDL_CONTROLLER_BUTTON_MISC1",SDL_CONTROLLER_BUTTON_MISC1},
    {"SDL_CONTROLLER_BUTTON_PADDLE1",SDL_CONTROLLER_BUTTON_PADDLE1},
    {"SDL_CONTROLLER_BUTTON_PADDLE2",SDL_CONTROLLER_BUTTON_PADDLE2},
    {"SDL_CONTROLLER_BUTTON_PADDLE3",SDL_CONTROLLER_BUTTON_PADDLE3},
    {"SDL_CONTROLLER_BUTTON_PADDLE4",SDL_CONTROLLER_BUTTON_PADDLE4},
    {"SDL_CONTROLLER_BUTTON_TOUCHPAD",SDL_CONTROLLER_BUTTON_TOUCHPAD},
    {"SDL_CONTROLLER_BUTTON_MAX",SDL_CONTROLLER_BUTTON_MAX}
};

};

#endif