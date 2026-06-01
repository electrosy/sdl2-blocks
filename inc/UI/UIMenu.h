/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley. All rights reserved.

Purpose: A Collection of UIElements
Date: Jul/17/2021
*/
#ifndef UIMENU_H
#define UIMENU_H

#include <unordered_map>

namespace ley {

enum class UIMenuItem {cell,row};

class UIMenu {

private:

    int mCurrentIndex;
    char mFaderControl = 0; //0 = fade in, 1 = fade out, 2 = hold //Run fade in reverse from in to out instead of out to in.
    int mWidth = 1; //1 is single dimensional. If width is more than one then the menu is created as a grid and down will jump down to the next row. ;)
    std::vector<ley::UIElement> mElements;
    std::unordered_map<std::string, int> mLabelIndex; // label → index in mElements for O(1) lookup
    std::vector<ley::Font*> mFontsUsed;    
    ley::RenderablesPtr mRenderables;
    ley::Fader mFader;
    bool isCurrentCellNull();
    void validNext(ley::UIMenuItem inMenuItem);
    void validPrevious(ley::UIMenuItem inMenuItem);
    int maxValidRow();
    int minValidRow();
    int maxValidCellInRow();
    int minValidCellInRow();
    

protected:
   
public:

    UIMenu();
    ~UIMenu();

    int row();
    int column();
    int rowAt(int index);
    int columnAt(int index);
    void next(UIMenuItem item);
    void previous(UIMenuItem item);
    SDL_Rect currentSrc();
    SDL_Rect currentDest();
    void pushFont(const std::string& elementid, ley::Font* inFont, SDL_Renderer* r);
    void pushFont(const std::string& label, SDL_Point pos, const std::string& s, SDL_Renderer* r, int size);
    void pushUIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter);
    void pushPlaceHolder();
    int getIndex();
    void clear(); //clear out all the elements.
    int count() const;
    void addRenderables(ley::RenderablesPtr);
    int getElementById(const std::string&);
    ley::UIElement* getElementPtr(const std::string& label); //Get elementPtr from label
    ley::UIElement* getCurrentElementPtr();
    void renderBaseMenuItems(ley::Video* v);
    void renderHotItem(ley::Video* v);
    void render(ley::Video* v);
    void runCommand(ley::Command command);
    void setWidth(int width) { mWidth = width; };
};

}

#endif