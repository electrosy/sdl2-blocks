#include "../inc/RectContainer.h"

ley::RectContainer::RectContainer()
: Renderable() {

  mFillBackgroundTransparency = 110;

}

void ley::RectContainer::operator()(SDL_Renderer* r) {
    
}

void ley::RectContainer::render(SDL_Renderer* r, bool d) {

    for (auto& [key, value]: rects) {
      SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawRect(r, &value);

      SDL_SetRenderDrawColor(r, 0, 0, 0, mFillBackgroundTransparency);

      SDL_RenderFillRect(r, &value);
    }
}

void ley::RectContainer::addRect(std::string id, SDL_Rect rect) {
  rects.insert(std::make_pair(id,rect));
}

SDL_Rect* ley::RectContainer::getRect(std::string id) {
  
  return &rects.find(id)->second;
}

void ley::RectContainer::increaseTransparency() {

  if(  !(mFillBackgroundTransparency >= 255) ) {
    mFillBackgroundTransparency++;
  }

  SDL_Log("Transparency now set to: %d", mFillBackgroundTransparency);
}

void ley::RectContainer::decreaseTransparency() {

  if(mFillBackgroundTransparency > 0) {
    mFillBackgroundTransparency--;
  }

  SDL_Log("Transparency now set to: %d", mFillBackgroundTransparency);
}
