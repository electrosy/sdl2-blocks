#include "../inc/RectContainer.h"

ley::RectContainer::RectContainer()
: Renderable() {

}

void ley::RectContainer::operator()(SDL_Renderer* r) {
    
}

void ley::RectContainer::render(SDL_Renderer* r, bool d) {

    SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (auto& [key, value]: rects) {
        SDL_RenderDrawRect(r, &value);
    }
}

void ley::RectContainer::addRect(std::string id, SDL_Rect rect) {
  rects.insert(std::make_pair(id,rect));
}
