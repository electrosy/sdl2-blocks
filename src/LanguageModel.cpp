#include "../inc/LanguageModel.h"
#include "../inc/gfx/Font.h"
#include "SDL2/SDL.h"
#include <fstream>
#include <sstream>

ley::LanguageModel::LanguageModel()
:
mCurrentLanguage{"en"} {

    // ISO / BCP-47 style codes with native display names (stable order)
    const struct {
        const char* code;
        const char* nativeName;
        const char* nameKey;
    } langs[] = {
        {"en",    "English",            "english"},
        {"es",    "Español",            "spanish"},
        {"fr",    "Français",           "french"},
        {"de",    "Deutsch",            "german"},
        {"pt-BR", "Português (Brasil)", "portuguese"},
        {"ru",    "Русский",            "russian"},
        {"ja",    "日本語",               "japanese"},
        {"zh-CN", "简体中文",             "chinese"},
        {"it",    "Italiano",           "italian"},
        {"ko",    "한국어",               "korean"},
        {"pl",    "Polski",             "polish"},
        {"nl",    "Nederlands",         "dutch"},
        {"tr",    "Türkçe",             "turkish"},
        {"uk",    "Українська",         "ukrainian"},
    };

    for (const auto& lang : langs) {
        mLanguageOrder.emplace_back(lang.code);
        mLanguages.emplace(lang.code, lang.nativeName);
        mLanguageNameKeys.emplace(lang.code, lang.nameKey);
    }
}

void ley::LanguageModel::setLanguage(const std::string& language) {
    mCurrentLanguage = language;
    ley::Font::setActiveFontLanguage(language);
}

void ley::LanguageModel::loadLanguage() {
    loadLanguageData(mCurrentLanguage);
}

void ley::LanguageModel::loadLanguageData(const std::string& language) {
    // Keep font path in sync with the language being loaded (startup + switches).
    ley::Font::setActiveFontLanguage(language);

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
    else {
        SDL_Log("ley::LanguageModel::loadLanguageData could not open: %s", fileName.c_str());
    }
}

std::string ley::LanguageModel::getNativeLanguageName(const std::string& code) const {
    auto it = mLanguages.find(code);
    return (it != mLanguages.end()) ? it->second : code;
}

std::string ley::LanguageModel::getLanguageNameKey(const std::string& code) const {
    auto it = mLanguageNameKeys.find(code);
    return (it != mLanguageNameKeys.end()) ? it->second : "";
}

bool ley::LanguageModel::hasLanguage(const std::string& code) const {
    return mLanguages.find(code) != mLanguages.end();
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
    std::string nameKey = getLanguageNameKey(mCurrentLanguage);
    if (nameKey.empty()) {
        // Unknown code — fall back to english label
        nameKey = "english";
    }
    return getWord(nameKey, 0, true, capitalizationtype::capitalizeFirst);
}
