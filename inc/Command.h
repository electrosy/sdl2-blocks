#ifndef COMMAND_H
#define COMMAND_H

namespace ley {

enum class Command {
    down,
    right,
    up,
    left,
    shiftdown,
    shiftright,
    shiftup,
    shiftleft,
    none,
    pause,
    space,
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
    fullscreen}; //these include directions and other inputs, this is overloaded maybe a couple enums would work better?

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

}

#endif