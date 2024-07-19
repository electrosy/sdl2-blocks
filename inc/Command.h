#ifndef COMMAND_H
#define COMMAND_H

namespace ley {

enum class Command {
    down,
    right,
    up,
    left,
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
    nextSong,
    previousSong,
    tab,
    backspace}; //these include directions and other inputs, this is overloaded maybe a couple enums would work better?

typedef struct {
    std::pair<ley::Command, std::vector<Uint8>> down;
    std::pair<ley::Command, std::vector<Uint8>> right;
    std::pair<ley::Command, std::vector<Uint8>> up;
    std::pair<ley::Command, std::vector<Uint8>> left;
    std::pair<ley::Command, std::vector<Uint8>> none;
    std::pair<ley::Command, std::vector<Uint8>> pause;
    std::pair<ley::Command, std::vector<Uint8>> space;
    std::pair<ley::Command, std::vector<Uint8>> console;
    std::pair<ley::Command, std::vector<Uint8>> cclockwise;
    std::pair<ley::Command, std::vector<Uint8>> clockwise;
    std::pair<ley::Command, std::vector<Uint8>> debugkeystoggle;
    std::pair<ley::Command, std::vector<Uint8>> debugcolide;
    std::pair<ley::Command, std::vector<Uint8>> debugtexture;
    std::pair<ley::Command, std::vector<Uint8>> debugnextlevel;
    std::pair<ley::Command, std::vector<Uint8>> debugprevlevel;
    std::pair<ley::Command, std::vector<Uint8>> debugclear;
    std::pair<ley::Command, std::vector<Uint8>> debugfill;
    std::pair<ley::Command, std::vector<Uint8>> debugonlyline;
    std::pair<ley::Command, std::vector<Uint8>> enter;
    std::pair<ley::Command, std::vector<Uint8>> quit;
    std::pair<ley::Command, std::vector<Uint8>> increaseVolume;
    std::pair<ley::Command, std::vector<Uint8>> decreaseVolume;
    std::pair<ley::Command, std::vector<Uint8>> nextSong;
    std::pair<ley::Command, std::vector<Uint8>> previousSong;
    std::pair<ley::Command, std::vector<Uint8>> tab;
    std::pair<ley::Command, std::vector<Uint8>> backspace;
} KeyBindings;

}

#endif