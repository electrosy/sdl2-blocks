# Particle system scaffold

Lightweight CPU particles (SDL_RenderFillRect, no GPU compute). Off by default so the playfield stays unchanged until you opt in.

`APPLICATION_VER` stays **0.7.5.0**. Bump toward 0.7.6.0 (and add a releasenotes stub) when the feature is complete — after moments, density, and readability are locked.

## How to enable

Any one of these:

1. **Environment** (wins over config, good for a one-off test):

   ```
   ABLOCKALYPSE_PARTICLES=1 make
   ABLOCKALYPSE_PARTICLES=1 ./sdl2-blocks
   ```

   `all` turns on every emit stub (line clear + hard drop + piece lock + combo):

   ```
   ABLOCKALYPSE_PARTICLES=all ./sdl2-blocks
   ```

   `0` / `off` forces the system off.

2. **mainconfig.csv** (persisted on exit):

   ```
   particles,on
   particlemoments,lineclear
   ```

   Use `particlemoments,all` to live-test the other stubs.

3. **F11** (`debugparticles`) toggles the system for the session. Default keyboard maps include this binding. Overlay (`F12` then `` ` ``) shows `Particles ON n/160`.

## Demo path

**Line clear** is the wired demo. Clearing one or more lines emits a short, low-alpha burst along that row (theme `fontHot`: jazz bright red, ablockalypse purple). Cap is 160 particles; newest are dropped when full.

Pause **freezes** the pool (no motion / ageing). Entering Play or `resetGame` **clears** it.

## What Steven still needs to lock

These emit hooks exist and can be called, but they are not shippable moments yet:

| Moment | API | Default |
| --- | --- | --- |
| Line clear | `ParticleSystem::emitLineClear` / `ParticleMoment::lineClear` | Demo (on when particles are enabled) |
| Hard drop | `emitHardDrop` / `hardDrop` | Stub (needs `particlemoments,all` or env `all`) |
| Piece lock | `emitPieceLock` / `pieceLock` | Stub |
| Combo | `emitCombo` / `combo` | Stub (combo count > 1) |

Open questions:

- Which of the four moments stay in a release build, and at what intensity?
- Should pause freeze (current) or clear?
- Should particles draw on the top layer (current, low alpha) or under the blocks?
- Options-menu toggle vs config/env only?

Until those are decided, leave the version at 0.7.5.0.
