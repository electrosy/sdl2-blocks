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

void ley::HighScores::renderScoreFonts(ley::Renderables* re, std::vector<ley::Font*> fonts) {

    (*fonts.begin())->updateMessage("     Name                    Level                 Lines                   Score");
    re->push_back(*fonts.begin());

    std::multimap<unsigned long, std::tuple<std::string, int, int>>::reverse_iterator it;
    int counter = 1; //max 11
    for(it = highscoresdata.rbegin(); it != highscoresdata.rend() && counter < 11; ++it, ++counter) {
      
      std::string score = std::to_string(it->first);

      (*fonts.at(counter)).updateMessage(std::get<0>(it->second) + "     " + std::to_string((std::get<2>(it->second))) + "         " + std::to_string(std::get<1>(it->second))  + "     " + score);
      
      re->push_back( (fonts.at(counter)) );
    }
}

void ley::HighScores::push(unsigned long score, std::string name, int level, int lines) {
  highscoresdata.emplace(score, std::make_tuple(name,lines,level));
}

void ley::HighScores::write() {
  std::ofstream myfile;
  myfile.open ("highscores.csv");

  std::multimap<unsigned long, std::tuple<std::string, int, int>>::reverse_iterator it;
  for(it = highscoresdata.rbegin(); it != highscoresdata.rend(); ++it) {
    myfile << it->first << ',' << std::get<0>(it->second) << "," << std::get<1>(it->second) << "," << std::get<2>(it->second) <<std::endl;
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

            std::string name, level, lines, score;
            std::getline(ss,score,','); std::cout<<"\""<<score<<"\"";
            std::getline(ss,name,','); std::cout<<", \""<<name<<"\"";
            std::getline(ss,lines,','); std::cout<<", \""<<lines<<"\"";
            std::getline(ss,level,','); std::cout<<", \""<<level<<"\"";

            int nLevel, nLines;
            unsigned long dScore;

            nLevel = std::stoi(level);
            nLines = std::stoi(lines);
            dScore = std::stol(score);

            highscoresdata.emplace(dScore, std::make_tuple(name,nLines,nLevel));
            
            SDL_Log("Test Read in File, Name: %s Level: %s Lines: %s Score: %s", name.c_str(), level.c_str(), lines.c_str(), score.c_str());
        }
    }
    return 0;
}