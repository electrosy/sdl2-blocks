#include "../../inc/UI/UIWidget.h"

std::string ley::UIWidget::mLastCharPressed = "";

std::string ley::UIWidget::getLastChar() {
    return mLastCharPressed;
}