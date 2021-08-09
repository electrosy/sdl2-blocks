/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: See header.
Date: Jul/17/2021
*/
#include <string>

#include "Video.h"
#include "Input.h"
#include "UIMenu.h"
#include "UIElement.h"
#include "Textures.h"

typedef ley::Textures TextureManager;

ley::UIMenu::UIMenu() 
: hot(false), currentIndex(0) {
    
}

ley::UIMenu::~UIMenu() {

}
void ley::UIMenu::push(std::string label, const SDL_Rect src, const SDL_Rect dest, std::string b, std::string t, std::string th) {
    //push a UI Element into the UI Menu

    SDL_Texture* base = TextureManager::Instance()->getTexture(b);
    SDL_Texture* tex = TextureManager::Instance()->getTexture(t);
    SDL_Texture* texhot = TextureManager::Instance()->getTexture(th);

    UIElement temp(label, src, dest, base, tex, texhot);
    elements.push_back(temp);
}

void ley::UIMenu::setHot(bool h) {
    hot = h;
}
SDL_Texture* ley::UIMenu::currentTex() {
    if(hot) {
        return elements.at(currentIndex).getTextureHot();
    }
    else {
        return elements.at(currentIndex).getTexture();
    }
}

int ley::UIMenu::runMenu(ley::Video* v, ley::Input* i, ley::GameModel* m, bool fs, std::string t, SDL_Rect r, double fpsDelay, menutypes ty) {
    
    bool runmain = true;
    
    SDL_Texture* background = TextureManager::Instance()->getTexture(t); //Background texture is passed in.

    //Buttons for the options menu.
    SDL_Texture* optBtnBack = TextureManager::Instance()->getTexture("opt-back");
    SDL_Texture* optBtnBackHot = TextureManager::Instance()->getTexture("opt-back-hot");

    SDL_SetTextureBlendMode(background,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(background, 255);

    std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*>> baseElements;
    getBaseElements(&baseElements);



    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = 0; src_rect.y = 0; src_rect.h = r.h ; src_rect.w = r.w;
    dest_rect.x = r.x; dest_rect.y = r.y; dest_rect.h = r.h; dest_rect.w = r.w;

    //default - start
    SDL_Rect current_rect = {0,0,150,60};
    SDL_Rect current_dest_rect = {29,199,150,60};

    /*
    ley::UIMenu mainmenu;
    mainmenu.push("start",{0,0,139,46},{25,199,139,46},btnStart,startButton,startButtonHot);
    mainmenu.push("highscore",{0,0,323,64},{29,282,323,64},btnHighScores,hsButton,hsButtonHot);
    mainmenu.push("options",{0,0,218,63},{29,365,218,63},btnOptions,options,optionsHot);
    mainmenu.push("exit",{0,0,100,49},{30,451,100,49},btnExit,exit,exitHot);
    */

    ley::UIMenu optionsmenu;
    //optionsmenu.push("back", {0,0,139,46},{25,199,139,46})
    
    unsigned int alphaFrameIndex = 0;
    bool faddedin = false;
    char fadespeed = 5;
    while(runmain == true) {
        SDL_Delay(fpsDelay);

        if(!faddedin) {
            if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                if(alphaFrameIndex < 255) {
                    alphaFrameIndex++;
                }
                else {
                    faddedin = true;
                }
            }
        }
        SDL_SetTextureAlphaMod(background, alphaFrameIndex);
       // SDL_SetTextureAlphaMod(currentButton, alphaFrameIndex);
        SDL_RenderCopy(v->getRenderer(), background, &src_rect, &dest_rect);

        //Display all the base menu elements
        for(int i = 0; i < baseElements.size() && !baseElements.empty(); ++i) {
            SDL_Rect source = std::get<0>(baseElements.at(i));
            SDL_Rect destination = std::get<1>(baseElements.at(i));
            SDL_Texture* baseTexture = std::get<2>(baseElements.at(i));
            
            SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
        }
        
        //Display either the hot or flicker depending on the current flag
        if((SDL_GetTicks() % 50) % 10) {
            if(faddedin) {
                setHot(true);
                SDL_RenderCopy(v->getRenderer(), currentTex(), &current_rect, &current_dest_rect);
            }
        } else {
            if(faddedin) {
                setHot(false);
                SDL_RenderCopy(v->getRenderer(), currentTex(), &current_dest_rect, &current_dest_rect);
            }
        }
        v->render();

        ley::Direction frameDirection = i->pollMainMenuEvents(runmain,fs,(*m));
        
        if(frameDirection == ley::Direction::down || frameDirection == ley::Direction::right) {
            next();
        }
        
        if(frameDirection == ley::Direction::up || frameDirection == ley::Direction::left) {
            previous();
        }

       current_rect = currentSrc();
       current_dest_rect = currentDest();

        v->clear();
    }

    return getIndex();
} 

void ley::UIMenu::getBaseElements(std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*> > *baseElements) {
    //Iterate through all elements and return only the base elements.

    for(int i = 0; i < elements.size() && !elements.empty(); ++i) {
        
        baseElements->push_back(   std::make_tuple( 
                                    elements.at(i).getSource(), elements.at(i).getDestination(), elements.at(i).getBase()
                                    ));
    }
}

void ley::UIMenu::previous() {
    if(currentIndex > 0) {
        currentIndex--;
    }
}

void ley::UIMenu::next() {

    if(currentIndex < elements.size()-1) {
        currentIndex++;
    }
}

SDL_Rect ley::UIMenu::currentDest() {
    return elements.at(currentIndex).getDestination();
}

SDL_Rect ley::UIMenu::currentSrc() {
    return elements.at(currentIndex).getSource();
}

int ley::UIMenu::getIndex() {
    return currentIndex;
}

void ley::UIMenu::clear() {
    elements.clear();
    currentIndex = 0;
}