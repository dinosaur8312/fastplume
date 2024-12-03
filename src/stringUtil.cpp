#include "stringUtil.h"
#include <stdexcept>

namespace FastPlume
{

    std::string removeSpecialCharacters(const std::string &line)
    {
        std::string result;
        for (char ch : line)
        {
            if (ch != ',' && ch != '"' && ch != '(' && ch != ')')
            {
                // Keep all characters except comma, double quote, and brackets
                result += ch;
            }
        }
        return result;
    }

    std::string removeCommasInsideQuotes(const std::string &line)
    {
        std::string result;
        bool inQuotes = false;

        for (char ch : line)
        {
            if (ch == '"')
            {
                // Toggle the inQuotes flag when encountering a double quote
                inQuotes = !inQuotes;
                result += ch; // Keep the double quote
            }
            else if (ch == ',' && inQuotes)
            {
                // Remove commas inside quotes
                continue;
            }
            else
            {
                result += ch;
            }
        }

        return result;
    }

    bool isEmptyOrSpace(const std::string &str)
    {
        return str.empty() || str.find_first_not_of(' ') == std::string::npos;
    }

    // Function to safely convert a string to double
    double safeConvertToDouble(const std::string &str)
    {
        if (isEmptyOrSpace(str))
        {
            return 0.0; // Default to 0.0 for empty or whitespace-only strings
        }
        try
        {
            return std::stod(str); // Convert to double if valid
        }
        catch (const std::invalid_argument &)
        {
            throw std::runtime_error("Invalid numeric value: " + str);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error("Value out of range: " + str);
        }
    }
}