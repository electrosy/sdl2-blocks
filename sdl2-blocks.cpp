#include <SDL2/SDL.h>
#include <stdio.h>

#include "Video.h"
#include "Input.h"
#include "Sprite.h"

int main(){


    bool game_running = 1;

    ley::Video mainVideo;
    ley::Input mainInput;
    ley::Sprite mainSprite(mainVideo.getRenderer(), "assets/BlockPiece.bmp");
    ley::Sprite catSprite(mainVideo.getRenderer(), "assets/char9.png");

    while(game_running) {
        SDL_Delay(1);

    /*** RENDER ***/
        //Render Sprites
        {
            catSprite.render(0,0);
            mainSprite.render(10,10);
        }
        //Output video screen
        mainVideo.render();

    /*** GET INPUT ***/
        game_running = mainInput.pollEvents();

    /*** UPDATE ***/


    /*** CLEAR ****/
        mainVideo.clear();
    }

    return 1;
}
