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

SDL_Color ParticleSystem::paletteColor(float towardBase) const {
    const ThemeDef& theme = activeThemeDef();
    const float t = std::max(0.f, std::min(1.f, towardBase));
    const SDL_Color hot = theme.fontHot;
    const SDL_Color base = theme.fontBase;
    return {
        static_cast<Uint8>(hot.r + (static_cast<int>(base.r) - static_cast<int>(hot.r)) * t),
        static_cast<Uint8>(hot.g + (static_cast<int>(base.g) - static_cast<int>(hot.g)) * t),
        static_cast<Uint8>(hot.b + (static_cast<int>(base.b) - static_cast<int>(hot.b)) * t),
        255
    };
}

float ParticleSystem::nextF(float lo, float hi) {
    mRng ^= mRng << 13;
    mRng ^= mRng >> 17;
    mRng ^= mRng << 5;
    const float u = static_cast<float>(mRng) / static_cast<float>(0xFFFFFFFFu);
    return lo + u * (hi - lo);
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

void ParticleSystem::burst(int count, float x, float y, float xJitter, float yJitter,
                           float vxLo, float vxHi, float vyLo, float vyHi,
                           float lifeLo, float lifeHi, Uint8 alpha, Uint8 sizeLo, Uint8 sizeHi,
                           float towardBase) {
    const SDL_Color c = paletteColor(towardBase);
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.x = x + nextF(-xJitter, xJitter);
        p.y = y + nextF(-yJitter, yJitter);
        p.vx = nextF(vxLo, vxHi);
        p.vy = nextF(vyLo, vyHi);
        p.maxLife = nextF(lifeLo, lifeHi);
        p.life = p.maxLife;
        p.r = c.r;
        p.g = c.g;
        p.b = c.b;
        p.a = alpha;
        p.size = static_cast<Uint8>(nextI(sizeLo, sizeHi));
        spawn(p);
    }
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
    // Primary moment: a spark along every cell of the cleared row.
    const int cols = std::max(1, playfield.w / BLOCKSIZE_PX);
    const int perCell = 3;
    const float rowMid = static_cast<float>(linePixelY) + BLOCKSIZE_PX * 0.5f;
    const float half = BLOCKSIZE_PX * 0.45f;

    for (int col = 0; col < cols; ++col) {
        const float cx = static_cast<float>(playfield.x) + (col + 0.5f) * BLOCKSIZE_PX;
        const float side = (cx < playfield.x + playfield.w * 0.5f) ? -1.f : 1.f;
        burst(perCell, cx, rowMid, half, half,
              side * 40.f, side * 180.f, -90.f, -10.f,
              0.32f, 0.52f, 95, 2, 3, 0.f);
    }

    // End-cap pops so a single-line clear still reads as a sweep.
    if (lineCount <= 1) {
        burst(6, static_cast<float>(playfield.x), rowMid, 4.f, half,
              -200.f, -80.f, -60.f, 10.f, 0.28f, 0.44f, 90, 2, 3, 0.1f);
        burst(6, static_cast<float>(playfield.x + playfield.w), rowMid, 4.f, half,
              80.f, 200.f, -60.f, 10.f, 0.28f, 0.44f, 90, 2, 3, 0.1f);
    }
}

void ParticleSystem::emitHardDrop(const SDL_Rect& piecePx) {
    // Medium impact spray along the landing edge.
    const float x = static_cast<float>(piecePx.x) + piecePx.w * 0.5f;
    const float y = static_cast<float>(piecePx.y + piecePx.h);
    const float xJ = std::max(8.f, piecePx.w * 0.5f);
    burst(18, x, y, xJ, 4.f,
          -90.f, 90.f, 30.f, 140.f,
          0.22f, 0.40f, 78, 2, 3, 0.2f);
}

void ParticleSystem::emitPieceLock(const SDL_Rect& piecePx) {
    // Medium settle around the locked silhouette.
    const float x = static_cast<float>(piecePx.x) + piecePx.w * 0.5f;
    const float y = static_cast<float>(piecePx.y) + piecePx.h * 0.5f;
    const float xJ = std::max(6.f, piecePx.w * 0.45f);
    const float yJ = std::max(6.f, piecePx.h * 0.45f);
    burst(14, x, y, xJ, yJ,
          -70.f, 70.f, -50.f, 25.f,
          0.20f, 0.36f, 70, 2, 2, 0.35f);
}

void ParticleSystem::emitCombo(SDL_Point hudOrigin, int comboCount, int lineCount, const SDL_Rect& playfield) {
    if (comboCount > 1) {
        const int n = std::min(22, 8 + comboCount * 4);
        burst(n, static_cast<float>(hudOrigin.x) + 45.f, static_cast<float>(hudOrigin.y) + 10.f,
              40.f, 10.f, -40.f, 40.f, -130.f, -40.f,
              0.32f, 0.55f, 88, 2, 3, 0.05f);
    }

    // Tetris+ (4 or more lines at once): a stronger playfield bloom.
    if (lineCount >= 4) {
        const int extra = 32 + (lineCount - 4) * 8;
        const float cx = static_cast<float>(playfield.x) + playfield.w * 0.5f;
        const float cy = static_cast<float>(playfield.y) + playfield.h * 0.45f;
        burst(extra, cx, cy, playfield.w * 0.35f, playfield.h * 0.2f,
              -160.f, 160.f, -180.f, 40.f,
              0.38f, 0.62f, 100, 2, 4, 0.f);
    }
}

} // namespace ley
