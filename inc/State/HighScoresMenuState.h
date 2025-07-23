#include "GameState.h"

#include "../gfx/Video.h"
#include "../../inc/GameModel.h"
#include "../UI/UIMenu.h"
#include "../UI/UIElement.h" // TODO this is also in TextEntry

#ifndef HIGHSCORESMENUSTATE_H
#define HIGHSCORESMENUSTATE_H

namespace ley {

class HighScoresMenuState : public ley::GameState {
public:

    HighScoresMenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sHighScoresMenuID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sHighScoresMenuID;
    ley::TextEntry mLocalTextEntry;
    ley::Sprite mBackground;
    ley::Font mNameFont;
    ley::Font mLevelFont;
    ley::Font mLinesFont;
    ley::Font mScoreFont;
    ley::Font mHighScoresTitleFont;

    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;

    int optionItem; //Store the option selected from the options menu.

    ley::UIMenu highscoresmenu;
    ley::RenderablesPtr highScoreRenderables;

    SDL_Rect pos_col1; // name
    std::vector<ley::Font*> mFontsHighScores;
    ley::Font font_objects[HIGHSCORES_NUM_DISPLAY];

    void commitUI();
    void UI_ToggleFocus();
};

}

#endif