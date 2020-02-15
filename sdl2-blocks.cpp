/* 
Author: Steven Philley
Purpose: A concept of blocks which can be used to create a game of interlocking blocks.
Date: Feb/15/2020
*/
#include <SDL2/SDL.h>
#include <stdio.h>

#include "Video.h"
#include "Input.h"
#include "Sprite.h"
#include "Block.h"

int main() {
    bool game_running = 1;

    ley::Video mainVideo;
    ley::Input mainInput;

    ley::Sprite mainSprite(mainVideo.getRenderer(), "assets/BlockPiece.bmp");

    // TODO create a vector of SDL_Rect to define where the frames are in the sheet.
    std::vector<SDL_Rect> catFrames;
    
    SDL_Rect catFrame1; 
    catFrame1.x = 8; catFrame1.y = 75;
    catFrame1.h = 100; catFrame1.w = 86;
    catFrames.push_back(catFrame1);

    SDL_Rect catFrame2; 
    catFrame2.x = 138; catFrame2.y = 75;
    catFrame2.h = 100; catFrame2.w = 86;
    catFrames.push_back(catFrame2);
    
    ley::Sprite catSprite(mainVideo.getRenderer(), "assets/char9.png",1, &catFrames);

    // TODO Create a single sprite block
    ley::Block firstBlock(mainSprite); //test block, not animated
    // TODO Create an animated test block
    ley::Block catAnimated(catSprite);

    // TODO Create a constructed test block

    while(game_running) {
        SDL_Delay(1);

    /*** RENDER ***/
        /* Render Sprites to buffer */
        {
            catSprite.render(0,0);
            mainSprite.render(10,10);
            firstBlock.getFrame()->render(20,20);
            catAnimated.getFrame()->render(30,30);
        }
        /* Render the Video system to the screen */
        mainVideo.render();

    /*** GET INPUT ***/
        game_running = mainInput.pollEvents();

    /*** UPDATE ***/


    /*** CLEAR ****/
        mainVideo.clear();
    }

    return 1;
}