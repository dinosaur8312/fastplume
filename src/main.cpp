#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "SigmaInterp.hpp"
#include "CSVParser.hpp"
#include "JsonInputHandler.hpp"
#include "FastPlume.h"

// main function
int main(int argc, char *argv[])
{
    std::string configFilePath = ""; // Default to empty, to use input_default.json

    // If a file path is provided as a command-line argument, use it
    if (argc > 1)
    {
        configFilePath = argv[1];
    }

    try
    {

        FastPlume::FastPlume plumeJob(configFilePath);
        plumeJob.run();

        // Process the JSON file to get the configuration
        SimConfig config = JsonInputHandler::processJsonFile(configFilePath);

        // Further processing with config data...
        // auto coefs = CSVParser::parseCoefCSV(config.coefCSVPath);

        // auto refData = CSVParser::parseRefCSV(config.refCSVPath);

        SigmaInterp::compareCSVdata(config);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing input file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}