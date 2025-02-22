#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <string>
#include <map>

namespace ley {

enum class capitalizationtype {
    capitalizeFirst,
    capitalizeWords,
    capitalizeNone
};

class LanguageModel {

private:
    std::string mCurrentLanguage;
    std::map<std::string, std::string>mLanguages;
    std::map<std::string, std::string>mLanguageFields;

public:
    LanguageModel();
    std::string getLanguage() { return mCurrentLanguage; };
    std::string getLanguageString();
    std::string getLanguageText(std::string language, std::string field);
    void setLanguage(std::string language) { mCurrentLanguage = language; };
    void loadLanguageData(std::string language);
    void loadLanguage();
    std::string getWord(std::string field, int pad, bool left, capitalizationtype capType);
    std::string padTo(std::string input, char padChar, unsigned long size, bool left);
    std::string capitalizeFirstLeterOfEveryWord(std::string input);
    std::string capitalizeFirstLetter(std::string input);

};
    
}

#endif
