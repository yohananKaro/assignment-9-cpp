#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <vector>

namespace registration {

std::string              toUpper(const std::string& str);
std::string              toLower(const std::string& str);
std::string              trim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string              join(const std::vector<std::string>& parts,
                              const std::string& separator);
bool                     startsWith(const std::string& str, const std::string& prefix);
bool                     endsWith(const std::string& str, const std::string& suffix);
std::string              replaceAll(const std::string& str,
                                    const std::string& from,
                                    const std::string& toStr);
int                      countChar(const std::string& str, char target);
bool                     isPalindrome(const std::string& str);
std::string              reverseString(const std::string& str);

} // namespace registration

#endif // STRINGUTILS_HPP
