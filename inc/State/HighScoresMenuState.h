#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"
#include "../../UIElement.h"

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
    
    ley::Sprite mBackground;

    Renderables mRenderables;
    Renderables mDebugRenderables;

    int optionItem; //Store the option selected from the options menu.

    ley::UIMenu highscoresmenu;
    ley::Renderables highScoreRenderables;

    SDL_Rect pos_col1; // name
    std::vector<ley::Font*> fonts;
    std::vector<ley::Font*> fonts_test;
    ley::Font font_objects[HIGHSCORES_NUM_DISPLAY];

    void onCommandEnter();
};

}

#endif