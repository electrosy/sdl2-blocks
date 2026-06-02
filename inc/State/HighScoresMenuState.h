#ifndef HIGHSCORESMENUSTATE_H
#define HIGHSCORESMENUSTATE_H

#include "BaseState.h"
#include "../HighScores.h"   // HIGHSCORES_NUM_DISPLAY, HighScores types (was transitive via GameModel.h)
#include "../UI/UIMenu.h"
#include "../UI/UIElement.h" // TODO this is also in TextEntry

namespace ley {

class HighScoresMenuState : public ley::BaseState {
public:

    HighScoresMenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onExit()  override;
    virtual bool onEnter();
    virtual bool resume();

    virtual StateID getStateID() const { return StateID::HighScoresMenu; }

private:

    ley::TextEntry mLocalTextEntry;
    ley::Sprite mBackground;
    ley::Font mNameFont;
    ley::Font mLevelFont;
    ley::Font mLinesFont;
    ley::Font mScoreFont;
    ley::Font mHighScoresTitleFont;


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