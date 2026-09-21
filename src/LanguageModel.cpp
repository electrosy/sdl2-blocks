#include "../inc/LanguageModel.h"
#include "../inc/gfx/Font.h"
#include "SDL2/SDL.h"
#include <fstream>
#include <sstream>

void ley::LanguageModel::registerLanguage(const std::string& code, const std::string& nativeName, const std::string& labelKey) {
    mLanguageCodes.push_back(code);
    mLanguages.emplace(code, nativeName);
    mLanguageLabelKeys.emplace(code, labelKey);
}

ley::LanguageModel::LanguageModel()
:
mCurrentLanguage{"en"} {

    // ISO 639 / BCP-47 language codes with native display names
    registerLanguage("en", "English", "english");
    registerLanguage("es", "Español", "spanish");
    registerLanguage("fr", "Français", "french");
    registerLanguage("de", "Deutsch", "german");
    registerLanguage("pt-BR", "Português (Brasil)", "portuguese");
    registerLanguage("ru", "Русский", "russian");
    registerLanguage("ja", "日本語", "japanese");
    registerLanguage("zh-CN", "简体中文", "chinese");
    
}

void ley::LanguageModel::loadLanguage() {
    loadLanguageData(mCurrentLanguage);
    setCurrentFontFile(fontFileForLanguage(mCurrentLanguage));
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

std::string ley::LanguageModel::getLanguageLabelKey(const std::string& code) const {
    auto it = mLanguageLabelKeys.find(code);
    if (it != mLanguageLabelKeys.end()) {
        return it->second;
    }
    return {};
}

std::string ley::LanguageModel::getLanguageNativeName(const std::string& code) const {
    auto it = mLanguages.find(code);
    if (it != mLanguages.end()) {
        return it->second;
    }
    return {};
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
    const std::string labelKey = getLanguageLabelKey(mCurrentLanguage);
    if (!labelKey.empty()) {
        return getWord(labelKey, 0, true, capitalizationtype::capitalizeFirst);
    }

    const std::string nativeName = getLanguageNativeName(mCurrentLanguage);
    if (!nativeName.empty()) {
        return nativeName;
    }

    return getWord("english", 0, true, capitalizationtype::capitalizeFirst);
}
