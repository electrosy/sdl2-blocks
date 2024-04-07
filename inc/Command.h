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

}

#endif