# Particle system (0.7.6.0)

Lightweight CPU particles (`SDL_RenderFillRect`, no GPU compute). Cap **160**. Pause **freezes** the pool; entering Play or `resetGame` **clears** it.

Steven locked the ship moments for 0.7.6.0. They are **on by default**.

## Ship moments

| Moment | Intensity | Color | Notes |
| --- | --- | --- | --- |
| Line clear | Primary, full | Theme `fontHot` | Spark along every cell of the cleared row |
| Hard drop | Medium | `fontHot` → slight `fontBase` | Impact spray on the landing edge |
| Piece lock | Medium | `fontHot` → `fontBase` | Settle around the locked piece |
| Combo / Tetris+ | Combo HUD; stronger bloom on 4+ lines | `fontHot` | Tetris+ fires even on the first Tetris |

Palette is **theme-only** (jazz `fontHot` bright red / ablockalypse purple, mixed only with that theme’s `fontBase`). No rainbow. No ambient board dust.

Jazz and ablockalypse both work: colors come from `activeThemeDef()` at emit time.

## Debug toggles (kept)

Default play has particles on. These remain for overlay work:

1. **Environment** (wins over config):

   ```
   ABLOCKALYPSE_PARTICLES=0 ./sdl2-blocks   # force off
   ABLOCKALYPSE_PARTICLES=1 ./sdl2-blocks   # force on
   ABLOCKALYPSE_PARTICLES=off ./sdl2-blocks
   ```

2. **F11** (`debugparticles`) session toggle. Overlay (`F12` then `` ` ``) shows `Particles ON n/160`.

3. **mainconfig.csv** (persisted on exit):

   ```
   particles,on
   particlemoments,ship
   ```

   `particlemoments,lineclear` is a debug-only mask (line clear, no drop/lock/combo).

## Files

- `inc/gfx/Particle.h`, `src/gfx/Particle.cpp` — pool + emit
- `GameModel` queues `ParticleCue` from line clear / lock / hard drop / combo
- `Video::pumpParticles` consumes cues onto the top `Renderable` layer
