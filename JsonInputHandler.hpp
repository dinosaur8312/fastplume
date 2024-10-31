#pragma once

#include <json.hpp>
#include "SimConfig.hpp"
#include <string>
#include <fstream>

class JsonInputHandler
{
public:
    explicit JsonInputHandler(const std::string &inputFileName = "");
    static SimConfig processJsonFile(const std::string &filePath);

private:
    static std::string findJsonFile(const std::string &fileName);
    static std::string basePath;
    static std::string currentPath;
};
