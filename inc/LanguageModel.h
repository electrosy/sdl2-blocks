#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <string>
#include <unordered_map>
#include <vector>

namespace ley {

enum class capitalizationtype {
    capitalizeFirst,
    capitalizeWords,
    capitalizeNone
};

class LanguageModel {

private:
    std::string mCurrentLanguage;
    // code -> native display name (e.g. "ja" -> "日本語")
    std::unordered_map<std::string, std::string> mLanguages;
    // code -> CSV name key (e.g. "ja" -> "japanese")
    std::unordered_map<std::string, std::string> mLanguageNameKeys;
    // Stable menu / API order (unordered_map iteration is unstable)
    std::vector<std::string> mLanguageOrder;
    std::unordered_map<std::string, std::string> mLanguageFields;

public:
    LanguageModel();
    std::string getLanguage() const { return mCurrentLanguage; };
    std::string getLanguageString() const;
    void setLanguage(const std::string& language);
    void loadLanguageData(const std::string& language);
    void loadLanguage();
    std::string getWord(const std::string& field, int pad, bool left, capitalizationtype capType) const;
    std::string padTo(const std::string& input, char padChar, unsigned long size, bool left) const;
    std::string capitalizeFirstLeterOfEveryWord(const std::string& input) const;
    std::string capitalizeFirstLetter(const std::string& input) const;

    // Ordered language list API (stable order for UI / selection by index)
    const std::vector<std::string>& getLanguageCodes() const { return mLanguageOrder; };
    std::string getNativeLanguageName(const std::string& code) const;
    std::string getLanguageNameKey(const std::string& code) const;
    bool hasLanguage(const std::string& code) const;

};

}

#endif
