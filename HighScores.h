/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: Handle all the functions for the High Scores (read, write, display)
Date: Aug/16/2021
*/

#include <tuple>

#include "Renderables.h"
#include "./inc/Font.h"


#ifndef HIGHSCORES_H
#define HIGHSCORES_H

typedef std::pair<int, std::tuple<std::string, int, int>> HighScoresRow; //score, name, level, lines
typedef std::multimap<unsigned long, std::tuple<std::string, int, int>> HighScoresType;

namespace ley {

const auto HIGHSCORES_NUM_DISPLAY = 16; // 15 rows plus 1 for the title row

class HighScores {

private:

    //key: score Value: Name, Level, Lines
    HighScoresType highscoresdata;
    bool clean; //a flag to know when to reload the highschores data.

protected:

public:
    HighScores();
    ~HighScores();

    void write();
    int read();
    void push(unsigned long, std::string, int, int);
    void renderScoreFonts(ley::RenderablesPtr*, std::vector<ley::Font*>, HighScoresRow placeholder, int placeholderrow);
    void setClean(bool c);
    bool isClean();
    int isNewHigh(int n);
    void setHighScore(int score, std::string name, int lines, int level);
};
}
#endif