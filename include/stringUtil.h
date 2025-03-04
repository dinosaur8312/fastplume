#ifndef STRINGUTIL_H
#define STRINGUTIL_H
#include <vector>
#include <string>

namespace FastPlume
{

    // Utility function to trim whitespace from a string
    inline std::string trim(const std::string &str)
    {
        size_t first = str.find_first_not_of(" \t\r\n");
        size_t last = str.find_last_not_of(" \t\r\n");
        return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
    }
    std::string removeSpecialCharacters(const std::string &line);

    std::string removeCommasInsideQuotes(const std::string &line);
     double safeConvertToDouble(const std::string &str);
    

} // namespace FastPlume
#endif // STRINGUTIL_H