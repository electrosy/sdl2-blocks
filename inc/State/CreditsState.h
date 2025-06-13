#include "GameState.h"

#include "../../Video.h"
#include "../../GameModel.h"
#include "../../UIMenu.h"
#include "../../UIElement.h"

#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

namespace ley {

class CreditsState : public ley::GameState {
public:

    CreditsState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sCreditsID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sCreditsID;

    ley::Sprite mBackground;
//    ley::Sprite mCreditsPanel;

    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;

    int optionItem; //Store the option selected from the options menu.
    ley::UIMenu optionUI;

    ley::Font mNameFont;
    ley::Font mMainRolesFont;
    ley::Font mMusicByFont;
    ley::Font mMusicByNamesFont;
    ley::Font mSpecialThanksFont;
    ley::Font mSpecialThanksNamesFont;
    ley::Font mSoundEffectsFont;
    ley::Font mSoundEffectsNamesFont;
    ley::Font mAblockalypseUsesFont;
    ley::Font mAblockalypseUsesNamesFont;

};

}

#endif