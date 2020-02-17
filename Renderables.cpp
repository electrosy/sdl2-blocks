/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/17/2020
*/
#include "Renderables.h";

/* RAII */
ley::Renderables::Renderables() {

}

ley::Renderables::~Renderables() {

}

/* Functions */
void ley::Renderables::push_back(Renderable * r) {
    renderables.push_back(r);
}

void ley::Renderables::renderAll() {
    for(Renderable* ren : renderables) {
       // ren->render();
    }
}