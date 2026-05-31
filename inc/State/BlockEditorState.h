#include "BaseState.h"
#include "../UI/UIMenu.h"
#include "../UI/UI_Tile.h"
#include "../../inc/Layout.h"
#include <unordered_map>

#ifndef BLOCKEDITORSTATE_H
#define BLOCKEDITORSTATE_H

namespace ley {

class BlockEditorState : public ley::BaseState {

Uint8 EDITOR_FONT_SIZE = 16;

private:
    Font mTitleFont;
    Font mHelpTipFont;
    Font mCurrentTileFont;
    Font mShiftControlsFont;
    UIMenu mBlockUIMenu;
    Layout mLayout;
    Layout mKeyLayout;
    std::vector<ley::Sprite> mBlockKeySprites;
    std::vector<ley::Font> mBlockKeyFonts;
    std::unordered_map<std::string, std::pair<bool, SDL_Rect>> mBlocksKeyRects;
    std::vector<std::shared_ptr<UI_Tile>> mTiles;
    std::string mPreviousOptionsValue;
    std::string mSelectedTextureChar;
    void updateBlockEditorFonts();
    void loadBlocksKey();
    BlockDataType mBlockData;
    void transferBlockToTiles(int xMajor, int yMajor, BlockDataType* inBlockPtr);
    std::shared_ptr<UI_Tile> tileAt(int inX, int inY);
    void shiftBlock(int inXMajor, int inYMajor, ley::Command direction);
    void createBlockDataFromStrings(BlockDataType* blockDataPtr, std::vector<std::string>* stringDataPtr);
    void rowDataUp(std::vector<std::string>* rowData);
    void rowDataDown(std::vector<std::string>* rowData);
    void rowDataRight(std::vector<std::string>* rowData);
    void rowDataLeft(std::vector<std::string>* rowData);
    SDL_Point getMajorTileFromMinor(SDL_Point minor); //returns the major tiles based on the minor x,y.
    void restoreBlocksDefaultFile();
    void setSelectedTextureChar();

public:
    BlockEditorState(ley::Video * v, ley::GameModel * gm);
    virtual StateID getStateID() const { return StateID::BlockEditor; }

    virtual void update(ley::Command command);
    virtual void render();
    virtual bool onExit()  override;
    virtual bool onEnter();
    virtual bool resume();
    virtual void loadRenderables();

    void loadFromBlockDataPtr(BlockFileDataMapType* blockDataMapPtr, BlockDataType* blockDataTypePtr);
    void WriteTileDataToFile();
    void GetMajorTileRows(int inXMajor, int inYMajor, std::string prefix, bool outputOrientation, std::vector<std::string>* rowDataVectorPtr);
    
};

}

#endif
