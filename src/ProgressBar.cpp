#include "../inc/ProgressBar.h"

ley::ProgressBar::ProgressBar(SDL_Rect rect)
: Renderable(),
border(rect), 
progress(border) {

  s = SDL_CreateRGBSurface(0, border.w, border.h, 32, 0, 0, 0, 0);
  SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));

}

void ley::ProgressBar::adjustProgress(float elapsed, float total) {

  if(total != 0) {
    progress.w = ceil(((elapsed/total)*border.w));
  }
}

void ley::ProgressBar::render(SDL_Renderer* r) {
  SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
  
  SDL_RenderDrawRect(r, &border);

  //If texture is empty, populate it the first time we have the rendering context available
  if(!t) {
    t = SDL_CreateTextureFromSurface(r,s);
  }

  //Render the texture to the portion of progress bar we want to fill in.
  SDL_RenderCopy(r, t, NULL, &progress);
}
