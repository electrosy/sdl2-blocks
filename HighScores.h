/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: Handle all the functions for the High Scores (read, write, display)
Date: Aug/16/2021
*/

#include <tuple>

#include "Renderables.h"
#include "Font.h"


#ifndef HIGHSCORES_H
#define HIGHSCORES_H


namespace ley {

class HighScores {

private:

    //key: score Value: Name, Level, Lines
    std::multimap<unsigned long, std::tuple<std::string, int, int>> highscoresdata;
    bool clean; //a flag to know when to reload the highschores data.

protected:

public:
    HighScores();
    ~HighScores();

    void write();
    int read();
    void push(unsigned long, std::string, int, int);
    void renderScoreFonts(SDL_Renderer*, ley::Renderables*, std::vector<ley::Font*>);
    void setClean(bool c);
    bool isClean();
};
}
#endif