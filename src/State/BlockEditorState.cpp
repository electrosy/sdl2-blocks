#include "../../inc/State/BlockEditorState.h"
 

typedef ley::Textures TextureManager;

namespace ley {

const std::string BlockEditorState::sBlockEditorID = "BLOCKEDITOR";

BlockEditorState::BlockEditorState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{0,0,100,50},
    mHelpTipFont{1000,690,100,50},
    mShiftControlsFont{1000,690,100,50} {

    // TODO localization
    //mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("block editor comming soon", 0, false, capitalizationtype::capitalizeWords));
    mShiftControlsFont.updateMessage("Press \"alt + arrow\", \"shift + arrow\", \"ctrl + arrow\" to shift blocks");
    mShiftControlsFont.setFontSize(EDITOR_FONT_SIZE);
    mShiftControlsFont.right(SCREEN_WIDTH);
    mShiftControlsFont.bottom(SCREEN_HEIGHT, 1);
    mHelpTipFont.updateMessage("Press \"right ctrl + d\" to restore to default blocks");
    mHelpTipFont.setFontSize(EDITOR_FONT_SIZE);
    mHelpTipFont.right(SCREEN_WIDTH);
    mHelpTipFont.bottom(SCREEN_HEIGHT, 2);
    
    updateBlockEditorFonts();

    int boardWidth = 35;
    int boardHeight = 20;
    mLayout.setLayout({84,20,30,30},0,0,boardWidth,boardHeight);
    mLayout.setMajorGrid(5, 10, 10);
    mBlockUIMenu.setWidth(boardWidth);

    int numCells = boardWidth * boardHeight;
    for(int i = 0; i < numCells; ++i) {
        
        mTiles.push_back(std::make_unique<UI_Tile>());
        SDL_Rect layoutRect = mLayout.getRectForCell(i);

        mTiles.back()->setPos({layoutRect.x,layoutRect.y});
        mTiles.back()->setWidth(layoutRect.w, layoutRect.w, 2);
        mTiles.back()->setVisible(false);
        mTiles.back()->setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
        mTiles.back()->setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
        mTiles.back()->setRegEx("\\b[defghij]\\b");
        
        if(BLOCK_SIZE == 0) {
            SDL_Log("Block Size is 0");
            return;
        }

        int xTile = mBlockUIMenu.rowAt(i);
        int yTile = mBlockUIMenu.columnAt(i);
        SDL_Point major = getMajorTileFromMinor({xTile,yTile});
    }

    mActiveUIElement = mTiles.back().get();
        
    for(const std::shared_ptr<UI_Tile> &tile : mTiles) {
        mBlockUIMenu.pushUIElement(
            [this,&tile](){tile->handleFocusChange(&mActiveUIElement, tile->getPreviousValuePtr());},
            [this,&tile]()->bool{return tile->hasFocus();},
            [this,&tile]() {tile->commit();}
        );   
    }

    loadBlocksKey();
    loadFromBlockDataPtr(mGameModel->getFileDataPtr(), &mBlockData);
}

void BlockEditorState::update(ley::Command command) {
    
    setSelectedTextureChar();

    //Update the current tile font
    int xTile = mBlockUIMenu.column();
    int yTile = mBlockUIMenu.row();
    SDL_Point major = getMajorTileFromMinor({xTile,yTile});
    mCurrentTileFont.updateMessage("Block editor tile " + std::to_string(xTile + 1) + "," + std::to_string(yTile + 1) 
                                    + " Major " + std::to_string(major.x + 1) + "," + std::to_string(major.y + 1));
    mCurrentTileFont.setFontSize(EDITOR_FONT_SIZE);
    mCurrentTileFont.bottom(SCREEN_HEIGHT, 1);
    mCurrentTileFont.left();
   
    //set all the key borders to hidden
    for(auto& entry : mBlocksKeyRects) {
        entry.second.first = false;
    }

    //set the selected box to true.
    if(mBlocksKeyRects.find(mSelectedTextureChar) != mBlocksKeyRects.end()) {
        mBlocksKeyRects[mSelectedTextureChar].first = true;
    }

    if(command == ley::Command::none) {
        return; // do nothing if there is no command to process.
    }

    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
            break;

        case ley::Command::shiftdown :
        case ley::Command::shiftup :
        case ley::Command::shiftright :
        case ley::Command::shiftleft :
        {
                SDL_Point major = getMajorTileFromMinor({mBlockUIMenu.column(), mBlockUIMenu.row()});
                shiftBlock(major.x,major.y, command);
        }
            break;

        case ley::Command::shiftmajleft :
        case ley::Command::shiftmajright :
        case ley::Command::shiftmajup :
        case ley::Command::shiftmajdown :
            {
                SDL_Point major = getMajorTileFromMinor({mBlockUIMenu.column(), mBlockUIMenu.row()});
                shiftBlock(major.x,0, command);
                shiftBlock(major.x,1, command);
                shiftBlock(major.x,2, command);
                shiftBlock(major.x,3, command);
            }
            break;

        case ley::Command::shiftalldown :
        case ley::Command::shiftallup :
        case ley::Command::shiftallleft :
        case ley::Command::shiftallright :
            {
                for(int i = 0; i < NUM_DIFFERENT_BLOCKS; ++i) {
                    for(int j = 0; j < MAX_ORIENTATIONS; ++j) {
                        shiftBlock(i,j, command);
                    }
                }
            }
            break;
        case ley::Command::restoredefault :
                restoreBlocksDefaultFile();
            break;

        default :
            break;
    }

    mBlockUIMenu.runCommand(command);

    for(const std::shared_ptr<UI_Tile> &tile : mTiles) {
        dynamic_cast<UIWidget*>( tile.get() )->update();
    }

}

void BlockEditorState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    //render the highlight rects.
    for(std::pair<const std::string, std::pair<bool, SDL_Rect>>& rect_pair : mBlocksKeyRects) {
        const std::string& key = rect_pair.first; // Key (std::string)
        std::pair<bool, SDL_Rect>& value = rect_pair.second; // Value (std::pair<bool, SDL_Rect>)

        SDL_SetRenderDrawColor(mVideoSystem->getRenderer(), CWHITE.r, CWHITE.g, CWHITE.b, CWHITE.a);
        if(value.first) {
            SDL_RenderDrawRect(mVideoSystem->getRenderer(), &value.second);
        }
    }
    
    mBlockUIMenu.render(mVideoSystem);
}

void BlockEditorState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mHelpTipFont);
    mRenderables.push_back(&mShiftControlsFont);
    mRenderables.push_back(&mCurrentTileFont);

    for(const std::shared_ptr<UI_Tile> &tile : mTiles) {
        mRenderables.push_back( dynamic_cast<UIWidget*>( tile.get() ));
    }
}

bool BlockEditorState::onEnter() {

    SDL_Log("Entering BlockEditorState");
    loadRenderables();

    for(const std::shared_ptr<UI_Tile> &tile : mTiles) {
        dynamic_cast<UIWidget*>( tile.get() )->setVisible(true);
    }

    
    return true;
}

bool BlockEditorState::onReEnter() {
    SDL_Log("ReEntering BlockEditorState");
    return true;
}

bool BlockEditorState::onExit() {
    SDL_Log("Exiting BlockEditorState");

    mActiveUIElement = {};

    WriteTileDataToFile();
    mGameModel->readBlockData();

    return true;
}

bool BlockEditorState::onPause() {
    GameState::onPause();
    
    return true;
}

void BlockEditorState::updateBlockEditorFonts() {

    //mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("block editor coming soon", 0, false, capitalizationtype::capitalizeWords));
}

void BlockEditorState::loadBlocksKey() {

    std::vector<std::string> blocksToCheck;
    blocksToCheck.push_back("d");
    blocksToCheck.push_back("e");
    blocksToCheck.push_back("f");
    blocksToCheck.push_back("g");
    blocksToCheck.push_back("h");
    blocksToCheck.push_back("i");
    blocksToCheck.push_back("j");

    mKeyLayout.setLayout({330,640,0,0}, 100,0,7,1);

    SDL_Rect start_rect;
    start_rect.x = 0;
    start_rect.y = 0;
    start_rect.w = BLOCKSIZE_PX;
    start_rect.h = BLOCKSIZE_PX;

    mKeyLayout.resetIndex();
    for(std::string blockStr : blocksToCheck) {
        mBlockKeyFonts.push_back(ley::Font());
        mBlockKeyFonts.back().updateMessage(blockStr);
        SDL_Rect fontLayoutRect = mKeyLayout.getNextRect();
        mBlockKeyFonts.back().setPos({fontLayoutRect.x - 25, fontLayoutRect.y});
        mBlocksKeyRects.emplace( blockStr,  std::make_pair(false, (SDL_Rect){fontLayoutRect.x - 37, fontLayoutRect.y - 10, 90, 50})  );
    }

    for(int i = 0; i < blocksToCheck.size(); ++i) {
        //spriteBackground = ley::Sprite(TextureManager::Instance()->getTexture(background_level.c_str()), 0, {start_rect}, {500,{0,0,0,0}});
        mBlockKeySprites.push_back(ley::Sprite(TextureManager::Instance()->getTexture(blocksToCheck[i]), 0, {start_rect}, {10,{0,0,0,0}}));
        SDL_Rect blockKeyPosition = mKeyLayout.getRectForCell(i);
        mBlockKeySprites.back().setPos(blockKeyPosition.x, blockKeyPosition.y);
    }

    //Load the block sprites into the renderables.
    for(ley::Sprite& sprite : mBlockKeySprites) {
        mRenderables.push_back(&sprite);
    }

    //Load the fonts into the renderables.
    for(ley::Font& font : mBlockKeyFonts) {
        mRenderables.push_back(&font);
    }
}

void BlockEditorState::loadFromBlockDataPtr(BlockFileDataMapType* blockDataMapPtr, BlockDataType* blockDataTypePtr) {
    SDL_Rect rect;
    char o = '0';
    bool canRotate;
    BlockNameType blockType = BlockNameType::cube;

    std::vector<BlockNameType> blocksToLoad;

    blocksToLoad.push_back(BlockNameType::cube);
    blocksToLoad.push_back(BlockNameType::tee);
    blocksToLoad.push_back(BlockNameType::rLee);
    blocksToLoad.push_back(BlockNameType::zee);
    blocksToLoad.push_back(BlockNameType::mzee);
    blocksToLoad.push_back(BlockNameType::lLee);
    blocksToLoad.push_back(BlockNameType::line);


    int blockTypeIndex = 0;
    for(BlockNameType blockType : blocksToLoad) {

        for(int i = 0; i < 4; ++i) {
            Block::setBlockDataFromFile(blockType,i,blockDataTypePtr, false, &rect, &canRotate);
            transferBlockToTiles(blockTypeIndex,i,blockDataTypePtr);
        }

        ++blockTypeIndex;
    }
    
}

void BlockEditorState::transferBlockToTiles(int xMajor, int yMajor, BlockDataType* inBlockPtr) {

    SDL_Point layoutSize = mLayout.getSize();
    Uint16 layoutMajorSize = mLayout.getMajorGridSize();
    Uint16 row = 0;
    for(std::array<BlockTexCode, BLOCK_SIZE>& blockRow : (*inBlockPtr) ) {
        Uint16 col = 0;
        for(BlockTexCode code : blockRow) {
            int tileX = col + (xMajor * layoutMajorSize);
            int tileY = row + (yMajor * layoutMajorSize);
            tileAt(tileX, tileY)->setTextureName(TEXCODE_CHAR.at(code));
            ++col;
        }
        ++row;
    }
}

std::shared_ptr<UI_Tile> BlockEditorState::tileAt(int inX, int inY) {
    
    SDL_Point layoutSize = mLayout.getSize();

    int value = inY * layoutSize.x + inX;

    if(value >= 0 && value < mTiles.size()) {
        return mTiles[value];
    }
    else {
        return {};
    }
}

void BlockEditorState::WriteTileDataToFile() {
    
    SDL_Point layoutSize = mLayout.getSize();
    Uint16 layoutMajorSize = mLayout.getMajorGridSize();
    std::vector<std::string> rowDataVector;

    std::vector<std::string> blockNamesToSave;
    blockNamesToSave.push_back("a-");
    blockNamesToSave.push_back("b-");
    blockNamesToSave.push_back("c-");
    blockNamesToSave.push_back("d-");
    blockNamesToSave.push_back("e-");
    blockNamesToSave.push_back("f-");
    blockNamesToSave.push_back("g-");

    if(layoutMajorSize == 0) {
        SDL_Log("BlockEditorState::WriteTileDataToFile() - layoutMajorSize was 0");
        return;
    }

    std::string rowData;
    int numBlocks = layoutSize.x / layoutMajorSize;
    int numOrientations = layoutSize.y / layoutMajorSize;
    for(int i = 0; i < numBlocks; ++i) { // each block
        for(int j = 0; j < numOrientations; ++j) { // each orientation
            rowData = blockNamesToSave[i] + std::to_string(j) + "-";
            GetMajorTileRows(i,j, rowData, true, &rowDataVector);
            if( !(i==numBlocks-1&&j==numOrientations-1)) { //not the very last line
                rowDataVector.push_back(""); // add an empty line
            }
        }
    }

    std::ofstream myfile;
    myfile.open("blocks.csv");
    
    for(std::string row : rowDataVector) {
        SDL_Log("RowData: %s", row.c_str());
        myfile << row.c_str() << std::endl;
    }

    myfile.close();
}

void BlockEditorState::GetMajorTileRows(int inXMajor, int inYMajor, std::string prefix, bool outputOrientation, std::vector<std::string>* rowDataVectorPtr) {

    SDL_Point layoutSize = mLayout.getSize();
    Uint16 layoutMajorSize = mLayout.getMajorGridSize();
    std::string rowOfData;
    
    for(int i = 0; i < layoutMajorSize; ++i) { //each row
        int row = i;
        rowOfData = prefix + (outputOrientation ? std::to_string(i) + "," : "");
        
        for(int j = 0; j < layoutMajorSize; ++j) { // each texture
            int col = j;
            int tileX = col + (inXMajor * layoutMajorSize);
            int tileY = row + (inYMajor * layoutMajorSize);

            rowOfData += tileAt(tileX, tileY)->getCurrentTextureName();
            
            if((j + 1) % layoutMajorSize == 0) {
                //SDL_Log("Currentoutputtexture: %s", rowOfData.c_str());
                //myfile << rowOfData;
                rowDataVectorPtr->push_back(rowOfData);
                rowOfData = "";
            }
        }
    }   
}

void BlockEditorState::shiftBlock(int inXMajor, int inYMajor, ley::Command direction) {

    std::vector<std::string> rowData;
    GetMajorTileRows(inXMajor, inYMajor, "", false, &rowData);

    switch (direction) {
        case ley::Command::shiftmajdown :
        case ley::Command::shiftdown :
        case ley::Command::shiftalldown :
            rowDataDown(&rowData);
        break;

        case ley::Command::shiftmajup :
        case ley::Command::shiftup :
        case ley::Command::shiftallup :
            rowDataUp(&rowData);
        break;
        
        case ley::Command::shiftmajright :
        case ley::Command::shiftright :
        case ley::Command::shiftallright :
            rowDataRight(&rowData);
        break;

        case ley::Command::shiftmajleft :
        case ley::Command::shiftleft :
        case ley::Command::shiftallleft :
            rowDataLeft(&rowData);
        break;
    }

    BlockDataType blockData;
    createBlockDataFromStrings(&blockData, &rowData);
    transferBlockToTiles(inXMajor, inYMajor, &blockData);

}
void BlockEditorState::rowDataUp(std::vector<std::string>* rowData) {

    int topRowIndex = 0;
    int bottomRowIndex = mLayout.getMajorGridSize() - 1;
    std::string topRow = (*rowData)[topRowIndex];
    //shift all rows down
    for(int i = 0; i < bottomRowIndex; ++i) {
         (*rowData)[i] = (*rowData)[i + 1];
    }
    //put the old bottom row onto the top row
    (*rowData)[bottomRowIndex] = topRow;
}

void BlockEditorState::rowDataDown(std::vector<std::string>* rowData) {

    int topRowIndex = 0;
    int bottomRowIndex = mLayout.getMajorGridSize() - 1;
    std::string bottomRow = (*rowData)[bottomRowIndex];
    //shift all rows down
    for(int i = bottomRowIndex; i > 0; --i) {
         (*rowData)[i] = (*rowData)[i - 1];
    }
    //put the old bottom row onto the top row.
    (*rowData)[topRowIndex] = bottomRow;
}

void BlockEditorState::rowDataRight(std::vector<std::string>* rowData) {

    int topRowIndex = 0;
    int firstColumnIndex = 0;
    int lastColumnIndex = mLayout.getMajorGridSize() - 1;
    int lastRowIndex = mLayout.getMajorGridSize() - 1;
    //for each row shift the characters to the right
    for(int i = 0; i <= lastRowIndex; ++i) {
        char c = (*rowData)[i][lastColumnIndex];
        for(int j = lastColumnIndex; j > 0; --j) {
            (*rowData)[i][j] = (*rowData)[i][j - 1];
        }
        (*rowData)[i][firstColumnIndex] = c;
    }
}

void BlockEditorState::rowDataLeft(std::vector<std::string>* rowData) {
    int topRowIndex = 0;
    int firstColumnIndex = 0;
    int lastColumnIndex = mLayout.getMajorGridSize() - 1;
    int lastRowIndex = mLayout.getMajorGridSize() - 1;
    //for each row shift the characters to the left
    for(int i = 0; i <= lastRowIndex; ++i) {
        char c = (*rowData)[i][firstColumnIndex];
        for(int j = 0; j < lastColumnIndex; ++j) {
            (*rowData)[i][j] = (*rowData)[i][j + 1];
        }
        (*rowData)[i][lastColumnIndex] = c;
    }
}

void BlockEditorState::createBlockDataFromStrings(BlockDataType* blockDataPtr, std::vector<std::string>* stringDataPtr) {

    int row = 0;
    for(std::string stringRow : (*stringDataPtr)) {
        int col = 0;
        for(char chars : stringRow) {
           (*blockDataPtr)[row][col] = CHAR_TEXCODE.at({chars});
            ++col;
        }
        ++row;
    }
}

SDL_Point BlockEditorState::getMajorTileFromMinor(SDL_Point minor) {
    
    SDL_Point major;
    Uint8 majorGridSize = mLayout.getMajorGridSize();
    if(majorGridSize != 0) {
        major.x = minor.x / majorGridSize;
        major.y = minor.y / majorGridSize;
    }

    return {major.x,major.y};
}

void BlockEditorState::restoreBlocksDefaultFile() {
 
    if(!copyFile2("blocks-default.csv", "blocks.csv")) {
        SDL_Log("Error loading blocks file");
    }
    mGameModel->readBlockData();
    loadFromBlockDataPtr(mGameModel->getFileDataPtr(), &mBlockData);
}
bool BlockEditorState::copyFile2(const std::string& source, const std::string& destination) {
    std::filesystem::path srcPath = std::filesystem::absolute(source);
    std::filesystem::path destPath = std::filesystem::absolute(destination);

    SDL_Log("Current directory: %s", std::filesystem::current_path().string().c_str());
    SDL_Log("Copying from %s to %s", srcPath.string().c_str(), destPath.string().c_str());

    // Check if source exists
    if (!std::filesystem::exists(srcPath)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Source file does not exist: %s", srcPath.string().c_str());
        return false;
    }

    // Check if destination is a directory
    if (std::filesystem::exists(destPath) && std::filesystem::is_directory(destPath)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Destination is a directory: %s", destPath.string().c_str());
        return false;
    }

    // Check if destination directory exists
    std::filesystem::path destDir = destPath.parent_path();
    if (!destDir.empty() && !std::filesystem::exists(destDir)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Destination directory does not exist: %s", destDir.string().c_str());
        return false;
    }

    // Check and set permissions if destination exists
    if (std::filesystem::exists(destPath)) {
        SDL_Log("Destination file exists: %s", destPath.string().c_str());
        std::filesystem::perms perms = std::filesystem::status(destPath).permissions();
        if ((perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Destination file is read-only: %s", destPath.string().c_str());
            std::error_code permEc;
            std::filesystem::permissions(destPath, perms | std::filesystem::perms::owner_write, 
                                        std::filesystem::perm_options::replace, permEc);
            if (permEc) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to set write permissions: %s (code: %d)",
                             permEc.message().c_str(), permEc.value());
                return false;
            }
        }

        // Workaround: Remove destination file to avoid overwrite issues
        std::error_code removeEc;
        std::filesystem::remove(destPath, removeEc);
        if (removeEc) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to remove existing destination: %s (code: %d)",
                         removeEc.message().c_str(), removeEc.value());
            return false;
        }
        SDL_Log("Removed existing destination file: %s", destPath.string().c_str());
    }

    std::error_code ec;
    std::filesystem::copy(srcPath, destPath, std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File copy failed: %s (code: %d, category: %s)",
                     ec.message().c_str(), ec.value(), ec.category().name());
        return false;
    }

    SDL_Log("Copy succeeded.");
    return true;
}

bool BlockEditorState::copyFile(const std::string& source, const std::string& destination) {
    std::error_code ec;
    std::filesystem::copy(source, destination, 
                         std::filesystem::copy_options::overwrite_existing, ec);
    if(ec) {
        SDL_Log("Files read error: %s . Value: %d", ec.message().c_str(), ec.value());
    }
    return !ec; // Return true if no error
}

void BlockEditorState::setSelectedTextureChar() {
    mSelectedTextureChar = mTiles[0]->getLastChar(); //pull the last char from the tiles widgets
}

}