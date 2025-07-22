#include "GameState.h"

#include "../../Video.h"
#include "../../inc/GameModel.h"
#include "../UI/UIMenu.h"

#ifndef LANGUAGEOPTIONSSTATE_H
#define LANGUAGEOPTIONSSTATE_H

namespace ley {

class LanguageOptionsState : public ley::GameState {
public:

    LanguageOptionsState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sLanguageOptionsID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sLanguageOptionsID;

    ley::Font mTitleFont;
    ley::Font mCurrentLanguageFont;
    ley::UIMenu mLanguageUI;

    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;

    void updateLanguageFont();
};

}

#endif