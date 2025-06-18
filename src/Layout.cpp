#include "../inc/Layout.h"

void ley::Layout::setLayout(
    SDL_Rect startPoint, 
    Uint16 horizontalSpacing, 
    Uint16 verticalSpacing, 
    Uint16 columns,
    Uint16 rows) {

        mStartPoint = startPoint;
        mHorizontalSpacing = horizontalSpacing;
        mVerticalSpacing = verticalSpacing;
        mRows = rows;
        mColumns = columns;
}

SDL_Rect ley::Layout::getRectForCell(int cellNum) {

    int column = cellNum % mColumns;
    int row = cellNum / mColumns;
    // Calculate extra spacing for major grid lines
    int majorColumns = column / mMajorGridSize; // Number of major grid lines before this column
    int majorRows = row / mMajorGridSize;
    return {
        mStartPoint.x + column * (mStartPoint.w + mHorizontalSpacing) + (majorColumns * mMajorHorizontalSpacing),
        mStartPoint.y + row * (mStartPoint.h + mVerticalSpacing) - (row) + (majorRows * mMajorVerticalSpacing),
        mStartPoint.w,
        mStartPoint.h};
}

SDL_Rect ley::Layout::getNextRect() {

    return getRectForCell(mCurrentIndex++);
}

void ley::Layout::resetIndex() {
    mCurrentIndex = 0;
}

void ley::Layout::setMajorGrid(Uint16 size, Uint16 majorHorizontalSpacing, Uint16 majorVerticalSpacing) {
    mMajorGridSize = size;
    mMajorHorizontalSpacing = majorHorizontalSpacing;
    mMajorVerticalSpacing = majorVerticalSpacing;
}