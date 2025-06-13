#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"
#include "../../inc/UI/UI_Tile.h"

#ifndef BLOCKEDITORSTATE_H
#define BLOCKEDITORSTATE_H

namespace ley {

class BlockEditorState : public ley::GameState {
public:

    BlockEditorState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sBlockEditorID; }

    ley::UI_Tile mFirstTile;

    std::string mPreviousOptionsValue;

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sBlockEditorID;

    ley::Font mTitleFont;
    ley::UIMenu mBlockUIMenu;

    Renderables mRenderables;
    Renderables mDebugRenderables;

    void updateBlockEditorFonts();

    void commitTile();
};

}

#endif