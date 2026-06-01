#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <string>
#include <unordered_map>

namespace ley {

enum class capitalizationtype {
    capitalizeFirst,
    capitalizeWords,
    capitalizeNone
};

class LanguageModel {

private:
    std::string mCurrentLanguage;
    std::unordered_map<std::string, std::string>mLanguages;
    std::unordered_map<std::string, std::string>mLanguageFields;

public:
    LanguageModel();
    std::string getLanguage() const { return mCurrentLanguage; };
    std::string getLanguageString() const;
    void setLanguage(const std::string& language) { mCurrentLanguage = language; };
    void loadLanguageData(const std::string& language);
    void loadLanguage();
    std::string getWord(const std::string& field, int pad, bool left, capitalizationtype capType) const;
    std::string padTo(const std::string& input, char padChar, unsigned long size, bool left) const;
    std::string capitalizeFirstLeterOfEveryWord(const std::string& input) const;
    std::string capitalizeFirstLetter(const std::string& input) const;

};
    
}

#endif
