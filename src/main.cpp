#include "FastPlume.h"
#include <iostream>
#include <vector>

int main() {
    try {
        // Test 1: Using setAttr to construct the task and output as a vector
        if(0)
        {
            std::cout << "=== Test 1: Using setAttr for task setup and output as vector ===" << std::endl;

            FastPlume::FastPlume plume;

            plume.setAttr("istab", std::vector<int>{1, 2, 3})
                 .setAttr("wind", std::vector<double>{10.0, 15.0, 20.0})
                 .setAttr("mass", std::vector<double>{1000.0, 1500.0, 2000.0})
                 .setAttr("dur", std::vector<double>{60.0, 120.0, 180.0});

            plume.setDispersionCoefCSV("../data/dispersion_coef.csv")
                 .setOutputMethod("Vector");

            plume.run();

            std::cout << "Test 1 completed: Task data constructed using setAttr, results output to vector." << std::endl;
        }

        // Test 2: Using setTaskDataCSV and setDispersionCoefCSV to set up task and output to CSV
        {
            std::cout << "\n=== Test 2: Using setTaskDataCSV and setDispersionCoefCSV for setup, output to CSV ===" << std::endl;

            FastPlume::FastPlume plume;

            plume.setTaskDataCSV("../tests/Evp_virtual_Definitions_2024-08-05.csv")
                 .setDispersionCoefCSV("../fixture/hpac_dispersion_coefs.csv")
                 .setLocDataCSVImportDirectory("../tests/location_files/")
                 .setOutputDirectory("../tests/")
                 .setOutputMethod("CSV");

            plume.run();

            std::cout << "Test 2 completed: Task data and coefficients set using CSV files, results output to CSV file." << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
