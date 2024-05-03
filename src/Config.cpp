#include "../inc/Config.h"

#include <fstream>
#include <sstream>

void ley::Config::read() {
     
    SDL_Log("Read Config Function called!");

    std::ifstream inFile("config.csv");
    if (inFile.is_open())
    {
        std::string line;
        while( std::getline(inFile,line) )
        {
            std::stringstream ss(line);

            std::string sHeight, sWidth;
            std::getline(ss,sWidth,'x');
            std::getline(ss,sHeight,'x');

            if(sWidth != "") {
                mBoardWidth = std::stoi(sWidth);
            }
            if(sHeight != "") {
                mBoardHeight = std::stoi(sHeight);
            }
        }
    }

    if(mBoardWidth == 0 || mBoardHeight == 0) {
        //failed to read config so set defaults.
        mBoardWidth = BOARDWIDTH_DEFAULT;
        mBoardHeight = BOARDHEIGHT_DEFAULT;
    }

    if(mBoardWidth > 30 || mBoardWidth < 8){
        mBoardWidth = BOARDWIDTH_DEFAULT;
    }

    if(mBoardHeight > 22 || mBoardHeight < 8) {
        mBoardHeight = BOARDHEIGHT_DEFAULT;
    }

    SDL_Log("Board height and width is:%d, %d ", mBoardHeight, mBoardWidth);
}