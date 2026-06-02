#ifndef LANGUAGEOPTIONSSTATE_H
#define LANGUAGEOPTIONSSTATE_H

#include "BaseState.h"
#include "../UI/UIMenu.h"

namespace ley {

class LanguageOptionsState : public ley::BaseState {
public:

    LanguageOptionsState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool resume();

    virtual StateID getStateID() const { return StateID::LanguageOptions; }

private:


    ley::Font mTitleFont;
    ley::Font mCurrentLanguageFont;
    ley::UIMenu mLanguageUI;


    void updateLanguageFont();
};

}

#endif