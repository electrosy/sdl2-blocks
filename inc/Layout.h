#include <SDL2/SDL.h>

#ifndef LAYOUT_H
#define LAYOUT_H


namespace ley {


class Layout {


private:
    

    SDL_Rect mStartPoint; //start point plus width and height.
    unsigned int mHorizontalSpacing;
    unsigned int mVerticalSpacing;
    unsigned int mRows;
    unsigned int mColumns;

public:

    SDL_Rect getRectForCell(int cellNum);
    void setLayout(SDL_Rect startPoint, unsigned int horizontalSpacing, unsigned int verticalSpacing, unsigned int rows, unsigned int columns);
};

}


#endif