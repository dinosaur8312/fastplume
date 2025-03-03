#include "FastPlume.h"
#include <iostream>
#include <vector>

int main() {
    try {
        // Test 1: Using setAttr to construct the task and output as a vector
        if(1)
        {
            std::cout << "=== Test 1: Using setAttr for task setup and output as vector ===" << std::endl;

            FastPlume::FastPlume plume;

            plume.setAttr("istab", std::vector<int>{3, 3, 3})
                 .setAttr("wind", std::vector<double>{0.8, 0.8, 0.8})
                 .setAttr("mass", std::vector<double>{1e8, 1e8, 1e8})
                 .setAttr("dur", std::vector<double>{9732.32, 6313.11, 4235.14})
                 .setAttr("sig_x0", std::vector<double>{1.30384, 1.30384, 1.30384})
                 .setAttr("sig_y0", std::vector<double>{1.30384, 1.30384, 1.30384})
                 .setAttr("sig_z0", std::vector<double>{0.1, 0.1, 0.1})
                 .setAttr("hml", std::vector<double>{5500, 5500, 5500});

            plume.setAttr("locData", std::vector<std::vector<double>>{
                {1, 1}, //x
                {0, 0.5988520325679565},//y
                {0, 0},//z
                {3600, 3600} //t
            });

            plume.setDispersionCoefCSV("../fixture/hpac_dispersion_coefs.csv")
                 .setOutputMethod("Vector");

            plume.run();

            auto results = plume.getResults();
            for(const auto &row : results)
            {
                std::cout << "Task " << row.id << " results:" << std::endl;
                for(const auto &loc : row.locations)
                {
                    std::cout << "Location: x=" << loc.x << ", y=" << loc.y << ", z=" << loc.z << ", t=" << loc.t << std::endl;
                    std::cout << "  sig_x=" << loc.sig_x << ", sig_y=" << loc.sig_y << ", sig_z=" << loc.sig_z << std::endl;
                    std::cout << "  zfunc=" << loc.zfunc << ", yfunc=" << loc.yfunc << ", xfunc=" << loc.xfunc << std::endl;
                    std::cout << "  qyz=" << loc.qyz << ", cpeak=" << loc.cpeak << ", concentration=" << loc.concentration << std::endl;
                    std::cout << "  dinf=" << loc.dinf << ", dosage=" << loc.dosage << std::endl;
                }
            }

            plume.printResult();

            std::cout << "Test 1 completed: Task data constructed using setAttr, results output to vector." << std::endl;
        }

        // Test 2: Using setTaskDataCSV and setDispersionCoefCSV to set up task and output to CSV
        if(1)
        {
            std::cout << "\n=== Test 2: Using setTaskDataCSV and setDispersionCoefCSV for setup, output to CSV ===" << std::endl;

            FastPlume::FastPlume plume;

            plume.setTaskDataCSV("../example/data/taskData.csv")
                 .setDispersionCoefCSV("../fixture/hpac_dispersion_coefs.csv")
                 .setLocDataCSVImportDirectory("../example/data/locs/")
                 .setOutputDirectory("../example/")
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
