/*
sdl2-blocks
Copyright (C) 2026 Steven Philley

Purpose: see header.
*/
#include "../inc/Theme.h"
#include "../inc/gfx/Renderable.h"

namespace {

// ── Jazz (original) ──────────────────────────────────────────────────────────
// Music: Pixabay tracks from the pre-s3m playlist, in their original order.
// NOTE: only BG slot 1 (bridge) is confirmed from the old loadTextures();
//       the remaining order is a reconstruction — reorder freely.
const ley::ThemeDef JAZZ_THEME {
    "jazz",
    {
        // In-repo jazz playlist (non-FULL_ASSETS path from historical Audio.cpp).
        // The 9 Pixabay tracks under assets/audio/music/ were FULL_ASSETS-only and were never committed to this public repo.
        "./assets/audio/wild_jazz.mp3",       // OpenGameArt — Alex McCulloch
        "./assets/audio/jazz.ogg",            // OpenGameArt — Spring Spring
        "./assets/audio/Shake and Bake.mp3",  // OpenGameArt — Matthew Pablo
    },
    {
        {"BG_JAZZ_01", "assets/background/jazz/bridge-7504605_1280x720.jpg"},
        {"BG_JAZZ_02", "assets/background/jazz/alley-89197_1280x720.jpg"},
        {"BG_JAZZ_03", "assets/background/jazz/building-5002861_1280x720.jpg"},
        {"BG_JAZZ_04", "assets/background/jazz/clouds-2517653_1280x720.jpg"},
        {"BG_JAZZ_05", "assets/background/jazz/night-7593233_1280x720.jpg"},
        {"BG_JAZZ_06", "assets/background/jazz/sea-7707983_1280x720.jpg"},
        {"BG_JAZZ_07", "assets/background/jazz/starry-sky-2051448_1280x720.jpg"},
        {"BG_JAZZ_08", "assets/background/jazz/sunset-7898136_1280x720.jpg"},
        {"BG_JAZZ_09", "assets/background/jazz/heaven-5114501_1280x720.jpg"},
    },
    {"ablockalypse-logo-2025", "assets/graphic/logo-slight-shadow-900x450.png"},
    0.75, 0,   // 900x450 art with transparent padding; scaled so it clears the menu at y=283
    ley::CDARKTEAL, ley::CBRIGHTRED, ley::CWHITE,
};

// ── Ablockalypse (current) ───────────────────────────────────────────────────
const ley::ThemeDef ABLOCKALYPSE_THEME {
    "ablockalypse",
    {
        "./assets/s3m/ADREAM.ogg",
        "./assets/s3m/BBSINTRO.ogg",
        "./assets/s3m/FM-SCRAP.ogg",
        "./assets/s3m/HARDCORE.ogg",
        "./assets/s3m/INC.ogg",
        "./assets/s3m/IZ-MIH.ogg",
        "./assets/s3m/OASIS.ogg",
        "./assets/s3m/STEVEE.ogg",
        "./assets/s3m/UNITECH.ogg",
        "./assets/s3m/VOUGE-ME.ogg",
    },
    {
        {"BG_ABL_01", "assets/background/Ablockalypse/background1.jpg"},
        {"BG_ABL_02", "assets/background/Ablockalypse/background2.jpg"},
        {"BG_ABL_03", "assets/background/Ablockalypse/background3.jpg"},
        {"BG_ABL_04", "assets/background/Ablockalypse/background4.jpg"},
        {"BG_ABL_05", "assets/background/Ablockalypse/background5.jpg"},
        {"BG_ABL_06", "assets/background/Ablockalypse/background6.jpg"},
        {"BG_ABL_07", "assets/background/Ablockalypse/background7.jpg"},
        {"BG_ABL_08", "assets/background/Ablockalypse/background8.jpg"},
        {"BG_ABL_09", "assets/background/Ablockalypse/background9.jpg"},
        {"BG_ABL_10", "assets/background/Ablockalypse/background10.jpg"},
    },
    {"ablockalypse-logo-2026", "assets/ablockalypse-logo-v2.png"},
    1.0, 25,
    ley::CORANGE, ley::CPURPLE, ley::CPINK,
};

const std::vector<ley::Theme> ALL_THEMES = { ley::Theme::jazz, ley::Theme::ablockalypse };

ley::Theme gActiveTheme = ley::THEME_DEFAULT;
unsigned int gThemeRevision = 0;

} // namespace

ley::Theme ley::stringToTheme(const std::string& s) {
    if (s == "jazz")         return Theme::jazz;
    if (s == "ablockalypse") return Theme::ablockalypse;
    SDL_Log("stringToTheme: unknown theme '%s', using default", s.c_str());
    return THEME_DEFAULT;
}

std::string ley::themeToString(Theme t) {
    return themeDef(t).name;
}

const ley::ThemeDef& ley::themeDef(Theme t) {
    switch (t) {
        case Theme::jazz: return JAZZ_THEME;
        default:          return ABLOCKALYPSE_THEME;
    }
}

const std::vector<ley::Theme>& ley::allThemes() {
    return ALL_THEMES;
}

void ley::setActiveTheme(Theme t) {
    if (t == gActiveTheme) return;
    gActiveTheme = t;
    ++gThemeRevision;
    SDL_Log("Active theme: %s (revision %u)", themeToString(t).c_str(), gThemeRevision);
}

ley::Theme ley::activeTheme() {
    return gActiveTheme;
}

const ley::ThemeDef& ley::activeThemeDef() {
    return themeDef(gActiveTheme);
}

unsigned int ley::themeRevision() {
    return gThemeRevision;
}
