#include "BaseState.h"

#include "../UI/UIMenu.h"

#ifndef LANGUAGEOPTIONSSTATE_H
#define LANGUAGEOPTIONSSTATE_H

namespace ley {

class LanguageOptionsState : public ley::BaseState {
public:

    LanguageOptionsState(ley::Video * v, ley::GameModel * gm);
    virtual void update(ley::Command command);
    virtual void render();
    virtual void loadRenderables();

    virtual bool onEnter();
    virtual bool resume();

    virtual std::string getStateID() const { return sLanguageOptionsID; }

private:

    static const std::string sLanguageOptionsID;

    ley::Font mTitleFont;
    ley::Font mCurrentLanguageFont;
    ley::UIMenu mLanguageUI;


    void updateLanguageFont();
};

}

#endif