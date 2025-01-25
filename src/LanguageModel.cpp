#include "../inc/LanguageModel.h"
#include "SDL2/SDL.h"
#include <fstream>
#include <sstream>

ley::LanguageModel::LanguageModel()
:
mCurrentLanguage{"en"} {

    //ISO 639 language code
    mLanguages.emplace("en", "English");
    mLanguages.emplace("es", "Spanish");

    loadLanguageData(mCurrentLanguage);

}

void ley::LanguageModel::loadLanguage() {
    loadLanguageData(mCurrentLanguage);
}

void ley::LanguageModel::loadLanguageData(std::string language) {
    //load config
    std::string fileName = language + ".csv";
    std::ifstream inFile(fileName);
    
    mLanguageFields.clear();

    if (inFile.is_open())
    {
        std::string line;
        while(std::getline(inFile,line) )
        {
            std::stringstream ss(line);

            std::string field, word;
            std::getline(ss,field,',');
            std::getline(ss,word,',');

            mLanguageFields.emplace(field, word);
        }
    }
}

std::string ley::LanguageModel::getWord(std::string field, int pad, bool left) {

    std::string string = mLanguageFields[field];
    string[0] = toupper(string[0]);
    
    return padTo(string, ' ', pad, left);
}

std::string ley::LanguageModel::padTo(std::string input, char padChar, unsigned long size, bool left) {
    
    //return early if the pad become a negative number.
    int width = size - input.length();

    if( width < 0) {
        return input;
    }

    if(left) {
        input.append(size - input.length(), padChar);
    } else {
        
        input.insert(input.begin(), width, padChar);
        // when right aligned also add an additional space after the word
        input.append(1, ' ');
    }
    
    return  input;
}
