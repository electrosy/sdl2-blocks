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
    
}

void ley::LanguageModel::loadLanguage() {
    loadLanguageData(mCurrentLanguage);
}

void ley::LanguageModel::loadLanguageData(const std::string& language) {
    //load config
    std::string fileName = "./assets/lang/" + language + ".csv";
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
std::string ley::LanguageModel::capitalizeFirstLetter(const std::string& input) const {

    std::string result = input;
    if (!result.empty()) {
        result[0] = toupper(static_cast<unsigned char>(result[0]));
    }
    return result;
}

std::string ley::LanguageModel::capitalizeFirstLeterOfEveryWord(const std::string& input) const {

    std::string result = capitalizeFirstLetter(input);
    for (int i = 0; i < static_cast<int>(result.size()); i++) {
        if (result[i] == ' ' && i + 1 < static_cast<int>(result.size())) {
            result[i + 1] = toupper(static_cast<unsigned char>(result[i + 1]));
        }
    }
    return result;
}

/*
** @left - when false the padding will be to the right otherwise it will be to the left.
*/
std::string ley::LanguageModel::getWord(const std::string& field, int pad, bool left, capitalizationtype capType) const {

    auto it = mLanguageFields.find(field);
    std::string string = (it != mLanguageFields.end()) ? it->second : "";

    if(string.empty()) {
        //We didn't find a match so just use the passed in field
        string = field;
        SDL_Log("ley::LanguageModel::getWord field not found: %s", field.c_str());
    }

    if(capType == ley::capitalizationtype::capitalizeWords) {
        string = capitalizeFirstLeterOfEveryWord(string);
    }
    else if (capType == ley::capitalizationtype::capitalizeFirst) {
        string = capitalizeFirstLetter(string);
    }

    return padTo(string, ' ', pad, left);
}

std::string ley::LanguageModel::padTo(const std::string& input, char padChar, unsigned long size, bool left) const {
    
    //return early if the pad become a negative number.

    // from https://stackoverflow.com/a/4063229 / //
    int len = 0;
    const char* s = input.c_str();
    while (*s) len += (*s++ & 0xc0) != 0x80;
    // // // // // // // // // // // // // // // //
    int width = size - len;

    if (width < 0) {
        return input;
    }

    std::string result = input;
    if (left) {
        result.append(static_cast<size_t>(width), padChar);
    } else {
        result.insert(result.begin(), width, padChar);
    }
    return result;
}

std::string ley::LanguageModel::getLanguageString() const {   
    std::string language;

    if(mCurrentLanguage == "es") {
        language = getWord("spanish", 0, true, capitalizationtype::capitalizeFirst);
    }
    else if (mCurrentLanguage == "en") {
        language = getWord("english", 0, true, capitalizationtype::capitalizeFirst);
    }
    else {
        //default to english
        language = getWord("english", 0, true, capitalizationtype::capitalizeFirst);
    }
    
    return language;
}
