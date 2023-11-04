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
void ley::UIMenu::renderBaseMenuItems(ley::Video* v) {
    
    //Display all the base menu elements
    for(int i = 0; i < elements.size() && !elements.empty(); ++i) {
        SDL_Rect source = elements[i].getSource();
        SDL_Rect destination = elements[i].getDestination();
        SDL_Texture* baseTexture = elements[i].getBase();
        
        SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
    }
}

void ley::UIMenu::renderHotItem(ley::Video* v) {
    //Display either the hot or flicker depending on the current flag

    if(elements.size() <= 0) {return; /*EARLY EXIT*/}
    
    SDL_Rect src_rect = currentSrc();
    SDL_Rect dest_rect = currentDest();

    if(count() > 0) {
        if((SDL_GetTicks() % 50) % 10) {
            setHot(true);
            SDL_RenderCopy(v->getRenderer(), currentTex(), &src_rect, &dest_rect);
        } else {
            setHot(false);
            SDL_RenderCopy(v->getRenderer(), currentTex(), &dest_rect, &dest_rect);
        }
    }
}

void ley::UIMenu::renderSelectors(ley::Video* v) {
    //Display all the available selectors
    for (std::multimap<std::string,UIElement>::iterator it = selectors.begin(); it!=selectors.end(); ++it) {
        
        if(it->second.isActiveSelector()) {
            SDL_Rect source = it->second.getSource();
            SDL_Rect destination = it->second.getDestination();
            SDL_Texture* baseTexture = it->second.getTexture();

            SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
        }
    }
}

void ley::UIMenu::render(ley::Video* v) {
    renderBaseMenuItems(v);
    renderHotItem(v);
    renderSelectors(v);

    renderables.renderAll(v->getRenderer(), false);
}

void ley::UIMenu::runCommand(ley::Command command) {
    if(count() > 0) {
            
        if(command == ley::Command::down || command == ley::Command::right) {
            next();
        }
        
        if(command == ley::Command::cclockwise || command == ley::Command::up || command == ley::Command::left) {
            previous();
        }

        if(command == ley::Command::space) {
            toggle(); //this will toggle the selector
        }
    }
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