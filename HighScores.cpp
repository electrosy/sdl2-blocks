/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: see header.
Date: Aug/16/2021
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include <SDL2/SDL.h>

#include "HighScores.h"
#include "Font.h"


ley::HighScores::HighScores() : clean(true) {

}

ley::HighScores::~HighScores() {

}
void ley::HighScores::setClean(bool c) {
  clean = c;
}

bool ley::HighScores::isClean() {
  return clean;
}

void ley::HighScores::renderScoreFonts(ley::Renderables* re, std::vector<ley::Font*> fonts, HighScoresRow placeholder, int placeholderrow) {

    (*fonts.begin())->updateMessage("Name              Level      Lines    Score");
    re->push_back(*fonts.begin());

    HighScoresType::reverse_iterator it;
    int counter = 1;
    bool placeholderadded = false;
    const auto COLUMN_SIZE_NAME = 20;
    const auto COLUMN_SIZE_LEVEL = 10;
    const auto COLUMN_SIZE_LINES = 7;
    const auto COLUMN_SIZE_SCORE = 6;

    std::string score;
    std::string name;
    std::string level;
    std::string lines;
    std::string hs_line;

    auto set_hs_line = [&score, &name, &level, &lines, &hs_line, &fonts, &counter, &re]() {
      name.append(COLUMN_SIZE_NAME - name.length(), '.');
      level.append(COLUMN_SIZE_LEVEL - level.length(), '.');
      lines.append(COLUMN_SIZE_LINES - lines.length(), '.');
      score.insert(score.begin(), COLUMN_SIZE_SCORE - score.length(), '.');

      hs_line = name + level + lines + score;

      (*fonts.at(counter)).updateMessage(hs_line);
      re->push_back((fonts.at(counter)) );
    };

    for(it = highscoresdata.rbegin(); it != highscoresdata.rend() && counter < HIGHSCORES_NUM_DISPLAY; ++counter) {
      
      if(placeholderrow == counter) {

        score = std::to_string(placeholder.first);
        name = std::get<0>(placeholder.second);
        level = std::to_string(std::get<1>(placeholder.second));
        lines = std::to_string(std::get<2>(placeholder.second));
        placeholderadded = true;

      } else {

        score = std::to_string(it->first);
        name = std::get<0>(it->second);
        level = std::to_string(std::get<2>(it->second));
        lines = std::to_string(std::get<1>(it->second));
        ++it; //only increment the iterator when we use the stored data.
      }

      set_hs_line();
    }

    if(!placeholderadded && placeholderrow > 0) {
      score = std::to_string(placeholder.first);
      name = std::get<0>(placeholder.second);
      level = std::to_string(std::get<1>(placeholder.second));
      lines = std::to_string(std::get<2>(placeholder.second));

      set_hs_line();
    }
}

void ley::HighScores::push(unsigned long score, std::string name, int level, int lines) {
  highscoresdata.emplace(score, std::make_tuple(name,lines,level));
}

void ley::HighScores::write() {
  std::ofstream myfile;
  myfile.open ("highscores.csv");

  HighScoresType::reverse_iterator it;
  int counter = 1;
  for(it = highscoresdata.rbegin(); it != highscoresdata.rend(); ++it) {
    myfile << counter++ << ',' << it->first << ',' << std::get<0>(it->second) << "," << std::get<1>(it->second) << "," << std::get<2>(it->second) <<std::endl;
  }

  myfile.close();
}

int ley::HighScores::read() {

    highscoresdata.clear();
    SDL_Log("Read Function called!");

    std::ifstream inFile("highscores.csv");
    if (inFile.is_open())
    {
        std::string line;
        while( std::getline(inFile,line) )
        {
            std::stringstream ss(line);

            std::string name, level, lines, score, index;
            std::getline(ss,index,',');
            std::getline(ss,score,',');
            std::getline(ss,name,',');
            std::getline(ss,lines,',');
            std::getline(ss,level,',');

            int nLevel, nLines;
            unsigned long dScore;

            nLevel = std::stoi(level);
            nLines = std::stoi(lines);
            dScore = std::stol(score);

            highscoresdata.emplace(dScore, std::make_tuple(name,nLines,nLevel));
        }
    }
    return 0;
}

int ley::HighScores::isNewHigh(int n) {

  auto it = highscoresdata.rbegin();
  auto i = 0;
  for(; it != highscoresdata.rend(); ++it) {
    i++;
    if(it->first <= n) {
      SDL_Log("Your new high score is at position:%d", i);
      break;
    }
    SDL_Log("HighScores Test: %d", it->first);
  }
  
  return i <= HIGHSCORES_NUM_DISPLAY-1 ? i : 0;
}

void ley::HighScores::setHighScore(int score, std::string name, int level, int lines) {
    //push on the new high score
    push(score, name, level, lines);
    write();
    setClean(false);
}