#include "jsonHandler.h"
#include "taskConfig.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace FastPlume
{

    // Initialize static members
    std::string jsonHandler::basePath = "../tests/";
    std::string jsonHandler::currentPath = "./";

    taskConfig jsonHandler::processJsonFile(const std::string &filePath)
    {
        std::string resolvedPath = findJsonFile(filePath);
        // print the path
        std::cout << "Input Json Config File: " << resolvedPath << std::endl;
        std::ifstream file(resolvedPath);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open JSON input file: " + resolvedPath);
        }

        nlohmann::json j;
        file >> j;
        file.close();

        taskConfig config;

        config.coefCSVPath = j.value("coefs_CSV", "");
        config.taskCSVPath = j.value("reference_CSV", "");
        config.outputCSVPath = j.value("outputCSV", "");

        // print the path
        std::cout << "input coef CSV: " << config.coefCSVPath << std::endl;
        std::cout << "input task CSV: " << config.taskCSVPath << std::endl;
        std::cout << "output CSV: " << config.outputCSVPath << std::endl;
        // Populate other fields as necessary

        return config;
    }

    std::string jsonHandler::findJsonFile(const std::string &fileName)
    {
        std::string defaultFileName = "input_default.json";
        if (!fileName.empty())
        {
            if (fs::exists(currentPath + fileName))
            {
                return currentPath + fileName;
            }
            else if (fs::exists(basePath + fileName))
            {
                return basePath + fileName;
            }
        }
        // Fallback to default file if fileName is empty or not found
        return basePath + defaultFileName;
    }

} // namespace FastPlume