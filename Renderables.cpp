/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/17/2020
*/
#include "Renderables.h"

/* RAII */
ley::Renderables::Renderables() {

}

ley::Renderables::~Renderables() {

}
/* Accessors */
unsigned int ley::Renderables::size() {
   return renderables.size() > 0 ? renderables.size() : 0;
}

/* Functions */
void ley::Renderables::push_back(Renderable * r) {
    renderables.push_back(r);
}

void ley::Renderables::renderAll() {
    
    if(renderables.size() > 0) {
        for(int i = 0; i < renderables.size(); ++i) {
                renderables[i]->render();
            }
    }
}