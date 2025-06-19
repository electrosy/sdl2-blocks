#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"
#include "../../inc/UI/UI_Tile.h"
#include "../../inc/Layout.h"

#ifndef BLOCKEDITORSTATE_H
#define BLOCKEDITORSTATE_H

namespace ley {

class BlockEditorState : public ley::GameState {

private:
    Video * mVideoSystem;
    GameModel * mGameModel;
    static const std::string sBlockEditorID;
    Font mTitleFont;
    UIMenu mBlockUIMenu;
    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;
    Layout mLayout;
    Layout mKeyLayout;
    std::vector<ley::Sprite> mBlockKeySprites;
    std::vector<ley::Font> mBlockKeyFonts;
    //std::vector<SDL_Rect> mBlocksKeyRects;
    std::map<std::string, std::pair<bool, SDL_Rect>> mBlocksKeyRects;
    std::vector<std::shared_ptr<UI_Tile>> mTiles;
    std::string mPreviousOptionsValue;
    std::string mSelectedTextureChar;
    void updateBlockEditorFonts();
    void loadBlocksKey();
    BlockDataType mBlockData;
    void transferBlockToTiles(int xMajor, int yMajor, BlockDataType* inBlockPtr);
    std::shared_ptr<UI_Tile> tileAt(int inX, int inY);

public:
    BlockEditorState(ley::Video * v, ley::GameModel * gm);
    virtual std::string getStateID() const { return sBlockEditorID; }

    virtual void update(ley::Command command);
    virtual void render();
    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();
    virtual void loadRenderables();

    void loadFromBlockDataPtr(BlockFileDataMapType* blockDataMapPtr, BlockDataType* blockDataTypePtr);
    
};

}

#endif