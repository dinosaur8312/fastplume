#ifndef DISPERSIONCOEF_H
#define DISPERSIONCOEF_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "mathUtil.h"

namespace FastPlume {

class IstabWindData
{
public:
    int istab;
    float wind;
    std::vector<float> sig_x;
    std::vector<float> sig_y;
    std::vector<float> sig_z;
    std::vector<float> ln_sig_x;
    std::vector<float> ln_sig_y;
    std::vector<float> ln_sig_z;
    float slope_ln_sig_x;
    float intercept_ln_sig_x;
    float slope_ln_sig_y;
    float intercept_ln_sig_y;
    float slope_ln_sig_z;
    float intercept_ln_sig_z;
    // Method to print data
    void printData() const
    {
        std::cout << "  Wind: " << wind << "\n";
        std::cout << "  Sig_x: ";
        for (const auto &val : sig_x)
            std::cout << val << " ";
        std::cout << "\n  Sig_y: ";
        for (const auto &val : sig_y)
            std::cout << val << " ";
        std::cout << "\n  Sig_z: ";
        for (const auto &val : sig_z)
            std::cout << val << " ";
        std::cout << "\n  Ln_Sig_x: ";
        for (const auto &val : ln_sig_x)
            std::cout << val << " ";
        std::cout << "\n  Ln_Sig_y: ";
        for (const auto &val : ln_sig_y)
            std::cout << val << " ";
        std::cout << "\n  Ln_Sig_z: ";
        for (const auto &val : ln_sig_z)
            std::cout << val << " ";
        std::cout << "\n  Slope/Intercept Ln_Sig_x: " << slope_ln_sig_x << ", " << intercept_ln_sig_x;
        std::cout << "\n  Slope/Intercept Ln_Sig_y: " << slope_ln_sig_y << ", " << intercept_ln_sig_y;
        std::cout << "\n  Slope/Intercept Ln_Sig_z: " << slope_ln_sig_z << ", " << intercept_ln_sig_z << "\n";
    }
};

class IstabData
{
public:
    int istab;
    std::vector<IstabWindData> istabwindData;
    std::vector<float> wind;

    // Method to sort istabwindData and update wind vector
    void sortData()
    {
        // Sort istabwindData by wind in ascending order
        std::sort(istabwindData.begin(), istabwindData.end(), [](const IstabWindData &a, const IstabWindData &b)
                  { return a.wind < b.wind; });

        // Update wind vector to match sorted istabwindData
        wind.clear();
        for (const auto &windData : istabwindData)
        {
            wind.push_back(windData.wind);
        }
    }

    // Method to print data
    void printData() const
    {
        std::cout << "Istab: " << istab << "\n";
        std::cout << "Wind values: ";
        for (const auto &w : wind)
            std::cout << w << " ";
        std::cout << "\nIstabWindData:\n";
        for (const auto &windData : istabwindData)
        {
            windData.printData();
            std::cout << "----------------\n";
        }
    }
};

class dispersionCoef
{
public:
    std::vector<IstabData> istabData;
    std::vector<int> istab;
    std::vector<float> x;
    std::vector<float> ln_x;
    float slope_ln_x;
    float intercept_ln_x;

    //default constructor
    dispersionCoef() {}

    // Constructor to parse CSV and initialize data
    dispersionCoef(const std::string &filePath) {
        parseCSV(filePath);
        sortData();
        printData();
    }

    void parseCSV(const std::string &filePath);

    // Method to sort istabData and update istab vector
    void sortData();
    
    // Method to print data
    void printData() const;

    // Interpolates one or all sig_* components based on input flags
    std::vector<float> interpolate_sig_component(int istab, float wind, float x, const std::string &flag);

    // Inverse interpolation to get x from a sig_* component
    float interpolate_x_from_sig_component(int istab, float wind, float sig_value, const std::string &flag);

private:
    // Helper to find indices for interpolation based on nearest values
    int find_nearest_wind_index(const std::vector<float> &wind_values, float wind) const;
    int find_nearest_x_index(float ln_x_value) const;

};

} // namespace FastPlume

#endif // DISPERSIONCOEF_H