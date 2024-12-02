#include "FastPlume.h"
#include <iostream>
#include <vector>

int main()
{
    try
    {

        std::cout << "\n=== Using setTaskDataCSV and setDispersionCoefCSV for setup, output to CSV ===" << std::endl;

        FastPlume::FastPlume plume;

        const std::string JobPath = "/home/xianlong/Code/FastPlume/jobs/Dust/2024-17-15/";
        const std::string CodePath = "/home/xianlong/Code/FastPlume/";

        plume.setTaskDataCSV("./taskFile/Dust_Case_Definitions_2024-17-15.csv")
            .setDispersionCoefCSV(CodePath + "fixture/hpac_dispersion_coefs.csv")
            .setLocDataCSVImportDirectory(JobPath + "locFiles/")
            .setOutputDirectory(JobPath + "outputFiles/")
            .setOutputMethod("CSV");

        plume.run();

        std::cout << "Test completed: Task data and coefficients set using CSV files, results output to CSV file." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
