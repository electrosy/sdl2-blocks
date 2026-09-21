/*
sdl2-blocks
Copyright (C) 2026 Steven Philley

Purpose: see header.
*/
#include "../../inc/gfx/Particle.h"
#include "../../inc/Theme.h"
#include "../../inc/Textures.h"

#include <algorithm>

namespace ley {

ParticleSystem::ParticleSystem()
: Renderable()
, mRng(SDL_GetTicks() | 1u)
{
}

void ParticleSystem::setFrozen(bool frozen) {
    mFrozen = frozen;
}

void ParticleSystem::clear() {
    mParticles.clear();
}

SDL_Color ParticleSystem::themeColor(int which) const {
    const ThemeDef& theme = activeThemeDef();
    if (which <= 0) return theme.fontBase;
    if (which == 1) return theme.fontHot;
    return theme.fontMain;
}

float ParticleSystem::nextF(float lo, float hi) {
    mRng ^= mRng << 13;
    mRng ^= mRng >> 17;
    mRng ^= mRng << 5;
    const float t = static_cast<float>(mRng) / static_cast<float>(0xFFFFFFFFu);
    return lo + t * (hi - lo);
}

int ParticleSystem::nextI(int lo, int hi) {
    if (hi <= lo) {
        return lo;
    }
    return lo + static_cast<int>(mRng % static_cast<unsigned>(hi - lo + 1));
}

void ParticleSystem::spawn(const Particle& p) {
    if (mParticles.size() >= MAX_PARTICLES) {
        return; // hard cap: drop newest rather than grow
    }
    mParticles.push_back(p);
}

void ParticleSystem::update(float dtSeconds) {
    if (mFrozen) {
        return;
    }

    if (dtSeconds <= 0.f) {
        return;
    }
    if (dtSeconds > 0.05f) {
        dtSeconds = 0.05f;
    }

    std::size_t write = 0;
    for (std::size_t i = 0; i < mParticles.size(); ++i) {
        Particle p = mParticles[i];
        p.life -= dtSeconds;
        p.x += p.vx * dtSeconds;
        p.y += p.vy * dtSeconds;
        p.vy += 90.f * dtSeconds;
        if (p.life > 0.f) {
            mParticles[write++] = p;
        }
    }
    mParticles.resize(write);
}

void ParticleSystem::render(SDL_Renderer* r, bool /*d*/) {
    if (!r) {
        return;
    }

    for (const Particle& p : mParticles) {
        const float t = p.maxLife > 0.f ? (p.life / p.maxLife) : 0.f;
        const Uint8 a = static_cast<Uint8>(std::max(0.f, std::min(255.f, p.a * t)));
        SDL_SetRenderDrawColor(r, p.r, p.g, p.b, a);
        const SDL_Rect dest{
            static_cast<int>(p.x),
            static_cast<int>(p.y),
            p.size,
            p.size
        };
        SDL_RenderFillRect(r, &dest);
    }
}

void ParticleSystem::emitLineClear(int linePixelY, int lineCount, const SDL_Rect& playfield) {
    const SDL_Color c = themeColor(1); // fontHot — jazz red / ablockalypse purple
    const int bursts = std::min(14, 8 + std::max(0, lineCount) * 2);

    for (int i = 0; i < bursts; ++i) {
        Particle p;
        p.x = static_cast<float>(playfield.x) + nextF(0.f, static_cast<float>(std::max(1, playfield.w)));
        p.y = static_cast<float>(linePixelY) + nextF(0.f, static_cast<float>(BLOCKSIZE_PX));
        p.vx = nextF(-140.f, 140.f);
        p.vy = nextF(-70.f, -10.f);
        p.maxLife = nextF(0.28f, 0.48f);
        p.life = p.maxLife;
        p.r = c.r;
        p.g = c.g;
        p.b = c.b;
        p.a = 70;
        p.size = static_cast<Uint8>(nextI(2, 3));
        spawn(p);
    }
}

void ParticleSystem::emitHardDrop(const SDL_Rect& piecePx) {
    const SDL_Color c = themeColor(0); // fontBase
    const int bursts = 8;
    for (int i = 0; i < bursts; ++i) {
        Particle p;
        p.x = static_cast<float>(piecePx.x) + nextF(0.f, static_cast<float>(std::max(1, piecePx.w)));
        p.y = static_cast<float>(piecePx.y + piecePx.h) + nextF(-4.f, 4.f);
        p.vx = nextF(-40.f, 40.f);
        p.vy = nextF(20.f, 80.f);
        p.maxLife = nextF(0.18f, 0.32f);
        p.life = p.maxLife;
        p.r = c.r;
        p.g = c.g;
        p.b = c.b;
        p.a = 55;
        p.size = 2;
        spawn(p);
    }
}

void ParticleSystem::emitPieceLock(const SDL_Rect& piecePx) {
    const SDL_Color c = themeColor(2); // fontMain
    const int bursts = 6;
    for (int i = 0; i < bursts; ++i) {
        Particle p;
        p.x = static_cast<float>(piecePx.x) + nextF(0.f, static_cast<float>(std::max(1, piecePx.w)));
        p.y = static_cast<float>(piecePx.y) + nextF(0.f, static_cast<float>(std::max(1, piecePx.h)));
        p.vx = nextF(-50.f, 50.f);
        p.vy = nextF(-40.f, 20.f);
        p.maxLife = nextF(0.16f, 0.28f);
        p.life = p.maxLife;
        p.r = c.r;
        p.g = c.g;
        p.b = c.b;
        p.a = 50;
        p.size = 2;
        spawn(p);
    }
}

void ParticleSystem::emitCombo(SDL_Point origin, int comboCount) {
    const SDL_Color c = themeColor(2);
    const int bursts = std::min(12, 6 + std::max(0, comboCount));
    for (int i = 0; i < bursts; ++i) {
        Particle p;
        p.x = static_cast<float>(origin.x) + nextF(0.f, 90.f);
        p.y = static_cast<float>(origin.y) + nextF(0.f, 20.f);
        p.vx = nextF(-30.f, 30.f);
        p.vy = nextF(-90.f, -20.f);
        p.maxLife = nextF(0.30f, 0.50f);
        p.life = p.maxLife;
        p.r = c.r;
        p.g = c.g;
        p.b = c.b;
        p.a = 65;
        p.size = 2;
        spawn(p);
    }
}

} // namespace ley
