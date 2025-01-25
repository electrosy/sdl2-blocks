#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"

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
    ley::UIMenu languageUI;

    Renderables mRenderables;
    Renderables mDebugRenderables;

    void updateLanguageFont();
};

}

#endif