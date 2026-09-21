/*
sdl2-blocks
Copyright (C) 2026 Steven Philley

Purpose: Lightweight CPU particle pool rendered through the existing
         Renderable path (same pattern as Font / ProgressBar / Board).
         No GPU compute. Cap 160 so a 144 FPS frame stays cheap.

         0.7.6.0 ship moments (Steven locked):
           line clear (primary, full), hard drop / piece lock (medium),
           combo / Tetris+ burst (stronger on 4+ lines).
         Theme palette only (fontHot / fontBase / fontMain). No rainbow,
         no ambient board dust.
*/
#pragma once

#include <cstddef>
#include <vector>

#include <SDL2/SDL.h>

#include "Renderable.h"

namespace ley {

struct Particle {
    float x = 0.f;
    float y = 0.f;
    float vx = 0.f;
    float vy = 0.f;
    float life = 0.f;     // seconds remaining
    float maxLife = 0.f;
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 a = 80;         // peak alpha; faded by remaining life
    Uint8 size = 2;
};

class ParticleSystem : public Renderable {

public:
    static constexpr std::size_t MAX_PARTICLES = 160;

    ParticleSystem();

    void update(float dtSeconds); // no-op while frozen (pause)
    void render(SDL_Renderer* r, bool d) override;

    void clear();
    void setFrozen(bool frozen);
    bool isFrozen() const { return mFrozen; }
    std::size_t alive() const { return mParticles.size(); }

    // Colors sampled at emit time from Theme.h (activeThemeDef palette only).
    void emitLineClear(int linePixelY, int lineCount, const SDL_Rect& playfield);
    void emitHardDrop(const SDL_Rect& piecePx);
    void emitPieceLock(const SDL_Rect& piecePx);
    void emitCombo(SDL_Point hudOrigin, int comboCount, int lineCount, const SDL_Rect& playfield);

private:
    void spawn(const Particle& p);
    SDL_Color paletteColor(float towardBase) const; // 0 = fontHot, 1 = fontBase
    float nextF(float lo, float hi);
    int nextI(int lo, int hi);
    void burst(int count, float x, float y, float xJitter, float yJitter,
               float vxLo, float vxHi, float vyLo, float vyHi,
               float lifeLo, float lifeHi, Uint8 alpha, Uint8 sizeLo, Uint8 sizeHi,
               float towardBase);

    std::vector<Particle> mParticles;
    bool mFrozen = false;
    unsigned mRng = 0;
};

} // namespace ley
