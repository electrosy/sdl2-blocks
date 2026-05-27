/*
sdl2-blocks
Copyright (C) 2020-2025 Steven Philley

Purpose: ConfigIO owns all CSV file reading and writing that was previously
         scattered through GameModel. Every method is static — this class
         has no instance state.

Files managed:
  mainconfig.csv       — language, key timing, gameplay toggles
  keyboard-config.csv  — keyboard bindings for the "play" context
  gamepad-config.csv   — gamepad button bindings for the "play" context
  blocks.csv           — block shape data
*/
#pragma once

#include <vector>
#include "Input.h"
#include "Block.h"

namespace ley {

class GameModel; // forward declaration — full header included in ConfigIO.cpp

class ConfigIO {
public:
    ConfigIO() = delete; // all static; never instantiated

    // Main game config (language, key delay, grid guide, etc.)
    static void readMainConfig(GameModel* gm);
    static void writeMainConfig(const GameModel* gm);

    // Keyboard bindings
    static void readKeyboardBindings(std::vector<KeyBindingRow>* out);
    static void writeKeyboardBindings(const KeyBindingsType& bindings);

    // Gamepad bindings
    static void readGamepadBindings(std::vector<ControllerButtonRow>* out);
    static void writeGamepadBindings(const PadBindingsType& bindings);

    // Block shape data
    static void readBlockData(BlockFileDataMapType* out);

    // Board dimensions from config.csv
    struct BoardSize { int width; int height; };
    static BoardSize readBoardSize();
};

} // namespace ley
