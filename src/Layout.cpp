#include "../inc/Layout.h"

void ley::Layout::setLayout(
    SDL_Rect startPoint, 
    unsigned int horizontalSpacing, 
    unsigned int verticalSpacing, 
    unsigned int rows,
    unsigned int columns) {

        mStartPoint = startPoint;
        mHorizontalSpacing = horizontalSpacing;
        mVerticalSpacing = verticalSpacing;
        mRows = rows;
        mColumns = columns;
}

SDL_Rect ley::Layout::getRectForCell(int cellNum) {

    int column = cellNum % mColumns;
    int row = cellNum / mColumns;
    return {
        mStartPoint.x + column * (mStartPoint.w + mHorizontalSpacing),
        mStartPoint.y + row * (mStartPoint.h + mVerticalSpacing) - (row),
        mStartPoint.w,
        mStartPoint.h
};

}