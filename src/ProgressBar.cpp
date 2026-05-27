#include "../inc/ProgressBar.h"

ley::ProgressBar::ProgressBar(SDL_Rect rect)
: Renderable(),
  border(rect),
  progress(border) {
}

ley::ProgressBar::ProgressBar(SDL_Rect rect, Timer* timer)
: Renderable(),
  border(rect),
  progress(border),
  mTimer(timer) {
}

void ley::ProgressBar::operator()(SDL_Rect rect) {
    border   = rect;
    progress = rect;
    SDL_Log("ProgressBar::operator() called");
}

void ley::ProgressBar::adjustProgress(float elapsed, float total) {
    if(total != 0) {
        progress.w = ceil(((elapsed / total) * border.w));
    }
}

void ley::ProgressBar::render(SDL_Renderer* r, bool d) {
    if(mTimer) {
        adjustProgress(mTimer->getElapsed(), mTimer->getSpeed());
    }

    // TODO: a rainbow/gradient fill could be used instead of solid red
    SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(r, &border);
    SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(r, &progress);
}
