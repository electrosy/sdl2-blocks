#include "GameState.h"

#include "../gfx/Video.h"
#include "../../inc/GameModel.h"
#include "../UI/UIMenu.h"

#ifndef MENUSTATE_H
#define MENUSTATE_H

namespace ley {

class MenuState : public ley::GameState {
public:

    MenuState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool onExit();
    virtual bool onReEnter();
    virtual bool onPause();

    virtual std::string getStateID() const { return sMenuID; }

private:

    ley::Video * mVideoSystem;
    ley::GameModel * mGameModel;
    static const std::string sMenuID;

    ley::Sprite mBackground;
//    ley::Sprite m3dBlocksPicture;
    ley::Sprite mAblockalypseLogo;

    RenderablesPtr mRenderables;
    RenderablesPtr mDebugRenderables;

    ley::UIMenu mainUI;
    int menuItem; //Store the option selected from the main menu.
    int optionItem; //Store the option selected from the options menu.
    ley::UIMenu optionUI;
};

}

#endif