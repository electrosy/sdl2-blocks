/*
sdl2-blocks
Copyright (C) 2026 Steven Philley

Purpose: Theme definitions (music + visuals) and the process-wide active theme.
         Each ThemeDef is pure data; the rest of the code asks for "the active
         theme's X" and never branches on which theme it is.

         GameModel owns the user-facing setting (getTheme/setTheme, persisted by
         ConfigIO) and pushes changes here. UI code that has no GameModel pointer
         (UIElement) reads the active palette directly. Long-lived consumers
         (resumed states, Video) compare themeRevision() against the revision
         they last applied to know when to refresh.
*/
#pragma once

#include <string>
#include <vector>

#include <SDL2/SDL.h>

namespace ley {

enum class Theme { jazz, ablockalypse };

constexpr Theme THEME_DEFAULT = Theme::ablockalypse;

struct TextureAsset {
    std::string key;   // TextureManager key
    std::string path;  // file path relative to the executable
};

struct ThemeDef {
    std::string name;                         // config / Options value
    std::vector<std::string> musicPaths;      // gameplay playlist, in play order
    std::vector<TextureAsset> backgrounds;    // per level; levels past the end reuse the last one
    TextureAsset logo;                        // main menu logo
    double logoScale;
    unsigned int logoY;
    SDL_Color fontBase;                       // UIElement base (unselected)
    SDL_Color fontHot;                        // UIElement hot (selected)
    SDL_Color fontMain;                       // UIElement main
};

// String <-> enum. Unknown strings fall back to THEME_DEFAULT.
Theme stringToTheme(const std::string& s);
std::string themeToString(Theme t);

// Definition for a specific theme, and the list of every theme (for loading all textures).
const ThemeDef& themeDef(Theme t);
const std::vector<Theme>& allThemes();

// Process-wide active theme. setActiveTheme bumps the revision only on an actual change.
void setActiveTheme(Theme t);
Theme activeTheme();
const ThemeDef& activeThemeDef();
unsigned int themeRevision();

} // namespace ley
