
#include "JsonInputHandler.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

// Initialize static members
std::string JsonInputHandler::basePath = "../tests/";
std::string JsonInputHandler::currentPath = "./";

JsonInputHandler::JsonInputHandler(const std::string& inputFileName) {
    // Constructor logic here (if needed)
}

SimConfig JsonInputHandler::processJsonFile(const std::string& filePath) {
    std::string resolvedPath = findJsonFile(filePath);
    //print the path
    std::cout << "Input Json Config File: " << resolvedPath << std::endl;
    std::ifstream file(resolvedPath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON input file: " + resolvedPath);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    SimConfig config;



    config.coefCSVPath = j.value("coefs_CSV", "");
    config.refCSVPath = j.value("reference_CSV", "");
    config.outputCSVPath = j.value("outputCSV", "");
    // Set computeMode based on filename
    if (config.refCSVPath.find("_02") != std::string::npos) 
    {
        config.computeMode = 1;
    } 
    else if (config.refCSVPath.find("zFunc") != std::string::npos) 
    {
        config.computeMode = 2;
    }
    else if (config.refCSVPath.find("sample") != std::string::npos)
    {
        config.computeMode = 3;
    }
    else if (config.refCSVPath.find("_01") != std::string::npos) 
    {
        config.computeMode = 0;
    }
    else if (config.refCSVPath.find("source_sigma") != std::string::npos) 
    {
        config.computeMode = 4;
    }
    else if (config.refCSVPath.find("dose_v_time") != std::string::npos) 
    {
        config.computeMode = 5;
    }
    else if (config.refCSVPath.find("py_") != std::string::npos) 
    {
        config.computeMode = 6;
    }
    else if (config.refCSVPath.find("decay") != std::string::npos) 
    {
        config.computeMode = 7;
    }
    else if (config.refCSVPath.find("depostion") != std::string::npos) 
    {
        config.computeMode = 8;
    }
    else if (config.refCSVPath.find("Evp_Case") != std::string::npos) 
    {
        config.computeMode = 9;
    }
    else if (config.refCSVPath.find("Dust_Case") != std::string::npos) 
    {
        config.computeMode = 10;
    }
    else if (config.refCSVPath.find("Sem_Case") != std::string::npos) 
    {
        config.computeMode = 11;
    }
    else if (config.refCSVPath.find("Evp_virtual") != std::string::npos) 
    {
        config.computeMode = 12;
    }
    else
    {
        config.computeMode = 6;
    }
    //print the path
    std::cout << "input coef CSV: " << config.coefCSVPath << std::endl;
    std::cout << "input ref CSV: " << config.refCSVPath << std::endl;
    std::cout << "outputCSV: " << config.outputCSVPath << std::endl;
    // Populate other fields as necessary

    return config;
}

std::string JsonInputHandler::findJsonFile(const std::string& fileName) {
    std::string defaultFileName = "input_default.json";
    if (!fileName.empty()) {
        if (fs::exists(currentPath + fileName)) {
            return currentPath + fileName;
        } else if (fs::exists(basePath + fileName)) {
            return basePath + fileName;
        }
    }
    // Fallback to default file if fileName is empty or not found
    return basePath + defaultFileName;
}
