#include "../inc/Config.h"

#include <fstream>
#include <sstream>

void ley::Config::read() {
     
    SDL_Log("Read Config Function called!");

    std::ifstream inFile("config.cfg");
    if (inFile.is_open())
    {
        std::string line;
        while( std::getline(inFile,line) )
        {
            std::stringstream ss(line);

            std::string sHeight, sWidth;
            std::getline(ss,sHeight,',');
            std::getline(ss,sWidth,',');

            mBoardHeight = std::stoi(sHeight);
            mBoardWidth = std::stoi(sWidth);
        }
    }

    SDL_Log("Board height and width is:%d, %d ", mBoardHeight, mBoardWidth);
}