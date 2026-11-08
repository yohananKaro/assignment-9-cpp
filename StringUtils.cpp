#include "StringUtils.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>

namespace registration {

std::string toUpper(const std::string& str) {
    std::string result = str;
    for (char& character : result) {
        character = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    }
    return result;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& character : result) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return result;
}

std::string trim(const std::string& str) {
    auto frontIter = str.begin();
    while ((frontIter != str.end()) &&
           (std::isspace(static_cast<unsigned char>(*frontIter)) != 0)) {
        ++frontIter;
    }
    auto backIter = str.end();
    while ((backIter != frontIter) &&
           (std::isspace(static_cast<unsigned char>(*std::prev(backIter))) != 0)) {
        --backIter;
    }
    return std::string(frontIter, backIter);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::size_t startPos = 0;
    constexpr std::size_t kStepOne = 1;
    while (true) {
        std::size_t delimPos = str.find(delimiter, startPos);
        if (delimPos == std::string::npos) {
            parts.push_back(str.substr(startPos));
            break;
        }
        parts.push_back(str.substr(startPos, (delimPos - startPos)));
        startPos = (delimPos + kStepOne);
    }
    return parts;
}

std::string join(const std::vector<std::string>& parts,
                 const std::string& separator) {
    if (parts.empty()) {
        return "";
    }
    std::string result = parts.front();
    constexpr std::size_t kSecondIndex = 1;
    for (std::size_t idx = kSecondIndex; idx < parts.size(); ++idx) {
        result += separator;
        result += parts[idx];
    }
    return result;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.starts_with(prefix);
}

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.ends_with(suffix);
}

std::string replaceAll(const std::string& str,
                       const std::string& from,
                       const std::string& toStr) {
    if (from.empty()) {
        return str;
    }
    std::string result;
    std::size_t startPos = 0;
    while (true) {
        std::size_t matchPos = str.find(from, startPos);
        if (matchPos == std::string::npos) {
            result += str.substr(startPos);
            break;
        }
        result += str.substr(startPos, (matchPos - startPos));
        result += toStr;
        startPos = (matchPos + from.size());
    }
    return result;
}

int countChar(const std::string& str, char target) {
    int count = 0;
    for (char character : str) {
        if (character == target) {
            ++count;
        }
    }
    return count;
}

bool isPalindrome(const std::string& str) {
    std::string cleaned;
    for (char character : str) {
        if (std::isalnum(static_cast<unsigned char>(character)) != 0) {
            cleaned += static_cast<char>(
                std::tolower(static_cast<unsigned char>(character)));
        }
    }
    std::string reversed = cleaned;
    std::reverse(reversed.begin(), reversed.end());
    return (cleaned == reversed);
}

std::string reverseString(const std::string& str) {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

} // namespace registration
