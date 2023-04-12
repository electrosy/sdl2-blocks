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
#include "Textures.h"

typedef ley::Textures TextureManager;

ley::UIMenu::UIMenu() 
: 
hot(false), 
currentIndex(0) {
    
}

ley::UIMenu::~UIMenu() {

}
void ley::UIMenu::push(std::string label, const SDL_Rect src, const SDL_Rect dest, const std::string b, const std::string t, const std::string th) {
    //push a UI Element into the UI Menu

    SDL_Texture* base = TextureManager::Instance()->getTexture(b);
    SDL_Texture* tex = TextureManager::Instance()->getTexture(t);
    SDL_Texture* texhot = TextureManager::Instance()->getTexture(th);

    UIElement temp(label, src, dest, base, tex, texhot);
    elements.push_back(temp);
}

int ley::UIMenu::count() {
    return elements.size();
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

    return nullptr;
}
int ley::UIMenu::getElementId(std::string label) {
    
    //Iterate through the elements and find the ID that matches the label.
    int index = -1;
    for(int i = 0; i < elements.size(); ++i) {
        if (elements[i].getLabel() == label) {
            index = i;
        }
    }
    return index;
}
void ley::UIMenu::addRenderables(ley::Renderables r) {
    renderables = r;
}

void ley::UIMenu::addSelector(std::string label, const SDL_Rect src, const SDL_Rect dest, const std::string b, const std::string w, const std::string h) {

    SDL_Log("ADD Selector Test: %d", getElementId(label));

    SDL_Texture* base = TextureManager::Instance()->getTexture(b);
    SDL_Texture* tex = TextureManager::Instance()->getTexture(w);
    SDL_Texture* texhot = TextureManager::Instance()->getTexture(h);

    UIElement temp(label, src, dest, base, tex, texhot);

    //count activeselectors for this label.
    for (std::multimap<std::string,UIElement>::iterator it=selectors.begin(); it!=selectors.end(); ++it) {
        if(it->second.getLabel() == label) {
            it->second.setActiveSelector(true);
            break;
        }
    }

    if(selectors.size() == 0) {
        temp.setActiveSelector(true);
    }

    selectors.emplace(label,temp);

}
int ley::UIMenu::runMenu(ley::Video* v, ley::Input* i, ley::GameModel* m, std::string t, SDL_Rect r, double fpsDelay, menutypes ty) {
    
    bool runmain = true;
    
    SDL_Texture* background = TextureManager::Instance()->getTexture(t); //Background texture is passed in.

    SDL_SetTextureBlendMode(background,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(background, 255);

    std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*>> baseElements; //All the elements for this menu.
    getBaseElements(&baseElements);

    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = 0; src_rect.y = 0; src_rect.h = r.h ; src_rect.w = r.w;
    dest_rect.x = r.x; dest_rect.y = r.y; dest_rect.h = r.h; dest_rect.w = r.w;

    //default - start
    SDL_Rect current_rect = {0,0,150,60};
    SDL_Rect current_dest_rect = {29,199,150,60};
    
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
        SDL_RenderCopy(v->getRenderer(), background, &src_rect, &dest_rect);

        //Display all the base menu elements
        for(int i = 0; i < baseElements.size() && !baseElements.empty(); ++i) {
            SDL_Rect source = std::get<0>(baseElements.at(i));
            SDL_Rect destination = std::get<1>(baseElements.at(i));
            SDL_Texture* baseTexture = std::get<2>(baseElements.at(i));
            
            SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
        }

        //Display all the available selectors
        for (std::multimap<std::string,UIElement>::iterator it = selectors.begin(); it!=selectors.end(); ++it) {
            
            if(it->second.isActiveSelector()) {
                SDL_Rect source = it->second.getSource();
                SDL_Rect destination = it->second.getDestination();
                SDL_Texture* baseTexture = it->second.getTexture();

                SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
            }
        }
        
        //Display either the hot or flicker depending on the current flag

        if(count() > 0) {
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
        }

        renderables.renderAll(v->getRenderer());
        v->present();

        ley::Command frameDirection = i->pollMainMenuEvents(runmain,(*m));
        
        if(count() > 0) {
            
            if(frameDirection == ley::Command::down || frameDirection == ley::Command::right) {
                next();
            }
            
            if(frameDirection == ley::Command::up || frameDirection == ley::Command::left) {
                previous();
            }

            if(frameDirection == ley::Command::space) {
                toggle(); //this will toggle the selector
            }

            current_rect = currentSrc();
            current_dest_rect = currentDest();
        }

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

void ley::UIMenu::toggle() {

    const std::string testLabel = elements[currentIndex].getLabel();

    //find a hot selector with this testLabel and then set the next one to hot.
    typedef std::multimap<std::string,UIElement>::iterator ElementIterator;
    std::pair<ElementIterator,ElementIterator> result = selectors.equal_range(testLabel.c_str());

    for (ElementIterator it = result.first; it!=result.second; ++it) {
        if(it->second.isActiveSelector()) {
            it->second.setActiveSelector(false);
            ++it;
            if(it !=result.second) {
                it->second.setActiveSelector(true);
            } else {
                result.first->second.setActiveSelector(true);
                
                break;
            }
        }
        
    }

    SDL_Log("selector has been toggled at index: %d and label: %s", currentIndex, testLabel.c_str());
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

void ley::UIMenu::runIntroScreen(ley::Video* v, ley::Input* i, ley::GameModel* m, std::string t, SDL_Rect r, double fpsDelay) {
    /**** Intro Screen Loop ****/
    bool intro = true;
    SDL_Texture* test = nullptr;
    test = TextureManager::Instance()->getTexture(t);
    SDL_SetTextureBlendMode(test,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(test, 255);

    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = 0; src_rect.y = 0; src_rect.h = r.h ; src_rect.w = r.w;
    dest_rect.x = r.x; dest_rect.y = r.y; dest_rect.h = r.h; dest_rect.w = r.w;

    unsigned int alphaFrameIndex = 0;
    bool faddedin = false;
    char fadespeed = 10;
    while(intro == true) {
        if(!faddedin) {
            if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                if(alphaFrameIndex < 255) {
                    alphaFrameIndex++;
                }
                else {
                    faddedin = true;
                }
            }
        } else {
             if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                 if(alphaFrameIndex != 0) {  
                    alphaFrameIndex--;
                 }
            }
        }
        SDL_SetTextureAlphaMod(test, alphaFrameIndex);
        SDL_RenderCopy(v->getRenderer(), test, &src_rect, &dest_rect);
        v->present();
        if(i->pollTitleEvents(intro,(*m)) == ley::Command::down 
            || (alphaFrameIndex < 10 && faddedin)) {
            intro = false;
        }
        v->clear();

        SDL_Delay(fpsDelay);
    }
}