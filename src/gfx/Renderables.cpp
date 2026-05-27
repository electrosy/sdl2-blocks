/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/17/2020
*/
#include "../../inc/gfx/Renderables.h"

/* RAII */
ley::RenderablesPtr::RenderablesPtr() {}
ley::RenderablesPtr::~RenderablesPtr() {}

/* Functions */
void ley::RenderablesPtr::push_back(Renderable * r) {
    renderables.push_back(r);
}

void ley::RenderablesPtr::renderAll(SDL_Renderer * r, bool d) {
    for (const auto value : renderables) {
        if (value->isVisible()) {
            value->render(r, d);
        }
    }
}
