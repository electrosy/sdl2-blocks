#include "../inc/ProgressBar.h"

ley::ProgressBar::ProgressBar(SDL_Rect rect)
: Renderable(),
border(rect), 
progress(border) { 

}

void ley::ProgressBar::operator()(SDL_Rect rect) {
    border = rect;
    progress = rect;

    SDL_Log("ProgressBar::operator() called");
}

void ley::ProgressBar::adjustProgress(float elapsed, float total) {

  if(total != 0) {
    progress.w = ceil(((elapsed/total)*border.w));
  }
}

void ley::ProgressBar::render(SDL_Renderer* r, bool d) {

  int alpha = SDL_ALPHA_OPAQUE;
  
  SDL_SetRenderDrawColor(r, 255, 255, 255, alpha);
  SDL_RenderDrawRect(r, &border);
  SDL_SetRenderDrawColor(r, 255, 0, 0, alpha);
  SDL_RenderFillRect(r, &progress);
}
