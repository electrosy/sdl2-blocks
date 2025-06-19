#include <SDL2/SDL.h>

#ifndef LAYOUT_H
#define LAYOUT_H


namespace ley {


class Layout {


private:
    

    SDL_Rect mStartPoint; //start point plus width and height.
    Uint16 mMajorGridSize = 1; // major grid can be set to give a larger space every x cells.
    Uint16 mHorizontalSpacing = 0;
    Uint16 mVerticalSpacing = 0;
    Uint16 mMajorHorizontalSpacing = 0;
    Uint16 mMajorVerticalSpacing = 0;
    Uint16 mRows;
    Uint16 mColumns;
    Uint16 mCurrentIndex = 0;

public:

    SDL_Rect getRectForCell(int cellNum);
    SDL_Rect getNextRect();
    void setLayout(SDL_Rect startPoint, Uint16 horizontalSpacing, Uint16 verticalSpacing, Uint16 rows, Uint16 columns);
    void setMajorGrid(Uint16 size, Uint16 majorHorizontalSpacing, Uint16 majorVerticalSpacing);
    void resetIndex();
    SDL_Point getSize() { return {mColumns, mRows};};
    Uint16 getMajorGridSize() { return mMajorGridSize;};
};

}


#endif