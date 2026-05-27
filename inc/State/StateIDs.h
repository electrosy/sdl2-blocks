/*
sdl2-blocks
Copyright (C) 2020-2025 Steven Philley

Purpose: Central enum for all game-state identities.
         Previously each state kept a static const std::string sXxxID member.
         Using an enum class gives O(1) comparison, no string allocation, and
         a single place to see every state the machine knows about.
*/
#pragma once

#include <string>

namespace ley {

enum class StateID {
    None,           // sentinel — returned when the stack is empty
    Intro,
    Menu,
    Play,
    Pause,
    GameOver,
    Credits,
    OptionMenu,
    HighScoresMenu,
    KeyboardOptions,
    LanguageOptions,
    BlockEditor
};

/// Human-readable name for logging. Not performance-sensitive.
inline std::string stateIDName(StateID id) {
    switch (id) {
        case StateID::Intro:            return "INTRO";
        case StateID::Menu:             return "MENU";
        case StateID::Play:             return "PLAY";
        case StateID::Pause:            return "PAUSE";
        case StateID::GameOver:         return "GAMEOVER";
        case StateID::Credits:          return "CREDITS";
        case StateID::OptionMenu:       return "OPTIONMENU";
        case StateID::HighScoresMenu:   return "HIGHSCORES";
        case StateID::KeyboardOptions:  return "KEYBOARDOPTIONS";
        case StateID::LanguageOptions:  return "LANGUAGEOPTIONS";
        case StateID::BlockEditor:      return "BLOCKEDITOR";
        default:                        return "UNKNOWN";
    }
}

} // namespace ley
