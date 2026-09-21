/*
sdl2-blocks
Copyright (C) 2026 Steven Philley

Purpose: Lightweight CPU particle pool rendered through the existing
         Renderable path (same pattern as Font / ProgressBar / Board).
         No GPU compute. Cap is small so a 144 FPS frame stays cheap.

         Steven still needs to lock which gameplay moments fire (see
         ParticleMoment in GameModel.h and docs/particles.md). The emit
         methods below are the stable hooks; callers can stay behind the
         particles / particlemoments flags until that call is made.
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

    // Theme colors are sampled at emit time from Theme.h (activeThemeDef).
    void emitLineClear(int linePixelY, int lineCount, const SDL_Rect& playfield);
    void emitHardDrop(const SDL_Rect& piecePx);
    void emitPieceLock(const SDL_Rect& piecePx);
    void emitCombo(SDL_Point origin, int comboCount);

private:
    void spawn(const Particle& p);
    SDL_Color themeColor(int which) const; // 0 base, 1 hot, 2 main
    float nextF(float lo, float hi);
    int nextI(int lo, int hi);

    std::vector<Particle> mParticles;
    bool mFrozen = false;
    unsigned mRng = 0;
};

} // namespace ley
