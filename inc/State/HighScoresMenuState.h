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

    std::vector<ley::Font*> fonts;
    ley::Font fontHighScores0 = {450, 190, 400, 40};
    ley::Font fontHighScores1 = {450, 225, 400, 40};
    ley::Font fontHighScores2 = {450, 260, 400, 40};
    ley::Font fontHighScores3 = {450, 295, 400, 40};
    ley::Font fontHighScores4 = {450, 330, 400, 40};
    ley::Font fontHighScores5 = {450, 365, 400, 40};
    ley::Font fontHighScores6 = {450, 400, 400, 40};
    ley::Font fontHighScores7 = {450, 435, 400, 40};
    ley::Font fontHighScores8 = {450, 470, 400, 40};
    ley::Font fontHighScores9 = {450, 505, 400, 40};
    ley::Font fontHighScores10 = {450, 540, 400, 40};
};

}

#endif