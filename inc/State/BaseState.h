/*
sdl2-blocks
Copyright (C) 2020-2025 Steven Philley

Purpose: BaseState is a concrete intermediate base that provides the members and
         default behaviour shared by every concrete game state:
           - Video* and GameModel* pointers
           - mRenderables / mDebugRenderables collections
           - Default render()   : renderAll + debug overlay
           - Default onExit()   : log + return true
           - Default onPause()  : delegate to GameState + return true
*/
#pragma once

#include "GameState.h"
#include "../gfx/Video.h"
#include "../GameModel.h"
#include "../gfx/Renderables.h"
#include "../Theme.h"

namespace ley {

class BaseState : public GameState {

protected:
    Video*         mVideoSystem;
    GameModel*     mGameModel;
    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;

    explicit BaseState(Video* v, GameModel* gm);

    // Override to re-skin anything built from the theme (UIMenu colors, logos, ...).
    // Only needed by states that can stay alive across a theme change — i.e. states
    // that get resume()d, or the Options state that changes it. Newly constructed
    // states already read the active theme.
    virtual void onThemeChanged(const ThemeDef& /*theme*/) {}

private:
    unsigned int mAppliedThemeRevision; // theme revision this state was last built/refreshed for

public:
    // Calls onThemeChanged() if the active theme changed since this state last applied it.
    virtual void refreshTheme() override;

    // Default render: mRenderables + debug overlay.
    // Override and optionally call BaseState::render() to extend.
    virtual void render() override;

    // Default lifecycle: log state ID + return true.
    virtual bool onExit()  override;
    virtual bool onPause() override;

    // These remain pure virtual — each state must define them.
    virtual void update(ley::Command command) override = 0;
    virtual void loadRenderables()           override = 0;
    virtual bool onEnter()                   override = 0;
    virtual bool resume()                 override = 0;
    virtual StateID getStateID() const   override = 0;
};

} // namespace ley
