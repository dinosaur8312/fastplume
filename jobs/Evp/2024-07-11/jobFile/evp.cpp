#include "FastPlume.h"
#include <iostream>
#include <vector>

int main()
{
    try
    {

        std::cout << "\n=== Using setTaskDataCSV and setDispersionCoefCSV for setup, output to CSV ===" << std::endl;

        FastPlume::FastPlume plume;

        const std::string JobPath = "/home/xianlong/Code/FastPlume/jobs/Evp/2024-07-11/";
        const std::string CodePath = "/home/xianlong/Code/FastPlume/";

        plume.setEvapTaskDataCSV("./taskFile/Evp_Case_Definitions_2024-07-15.csv")
            .setTaskDataCSV("./taskFile/Evp_Case_Definitions_2024-07-15.csv")
            .setAgentPropertiesCSV(CodePath + "fixture/agent_properties.csv")
            .setDispersionCoefCSV(CodePath + "fixture/hpac_dispersion_coefs.csv")
            .setLocDataCSVImportDirectory(JobPath + "locFiles/")
            .setOutputDirectory(JobPath+"outputFiles/")
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
