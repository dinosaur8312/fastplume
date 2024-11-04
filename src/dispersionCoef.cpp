#include "dispersionCoef.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace FastPlume
{

    void dispersionCoef::sortData()
    {
        // Sort istabData by istab values in ascending order
        std::sort(istabData.begin(), istabData.end(), [](const IstabData &a, const IstabData &b)
                  { return a.istab < b.istab; });

        // Update istab vector to match sorted istabData
        istab.clear();
        for (auto &istabEntry : istabData)
        {
            istab.push_back(istabEntry.istab);

            // Sort the data within each IstabData instance
            istabEntry.sortData();
        }
    }

    void dispersionCoef::printData() const
    {
        std::cout << "Dispersion Coefficients:\n";
        std::cout << "Istab values: ";
        for (const auto &val : istab)
            std::cout << val << " ";
        std::cout << "\nX values: ";
        for (const auto &val : x)
            std::cout << val << " ";
        std::cout << "\nLn_X values: ";
        for (const auto &val : ln_x)
            std::cout << val << " ";
        std::cout << "\nSlope/Intercept Ln_X: " << slope_ln_x << ", " << intercept_ln_x << "\n";
        std::cout << "----------------\nIstabData:\n";
        for (const auto &istabEntry : istabData)
        {
            istabEntry.printData();
            std::cout << "================\n";
        }
    }

    void dispersionCoef::parseCSV(const std::string &filePath)
    {
        // Intermediate data structures
        std::unordered_map<int, std::unordered_map<float, IstabWindData>> istabWindMap;
        std::vector<float> x_values;

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filePath);
        }

        std::string line;
        std::getline(file, line); // Read the header line

        // Identify column positions for required columns
        std::unordered_map<std::string, int> column_map;
        std::istringstream headerStream(line);
        std::string column;
        int columnIndex = 0;
        while (std::getline(headerStream, column, ','))
        {
            column_map[column] = columnIndex++;
        }

        // Check if all required columns are present
        const std::vector<std::string> requiredColumns = {"istab", "wind", "x", "sig_x", "sig_y", "sig_z"};
        for (const auto &required : requiredColumns)
        {
            if (column_map.find(required) == column_map.end())
            {
                throw std::runtime_error("Missing required column: " + required);
            }
        }

        // Read data lines
        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            std::string value;
            float x = 0, sig_x = 0, sig_y = 0, sig_z = 0, wind = 0;
            int istab = 0;

            // Parse line based on column positions
            int currentColumn = 0;
            while (std::getline(ss, value, ','))
            {
                if (currentColumn == column_map["x"])
                {
                    x = std::stof(value);
                }
                else if (currentColumn == column_map["sig_x"])
                {
                    sig_x = std::stof(value);
                }
                else if (currentColumn == column_map["sig_y"])
                {
                    sig_y = std::stof(value);
                }
                else if (currentColumn == column_map["sig_z"])
                {
                    sig_z = std::stof(value);
                }
                else if (currentColumn == column_map["istab"])
                {
                    istab = std::stoi(value);
                }
                else if (currentColumn == column_map["wind"])
                {
                    wind = std::stof(value);
                }
                currentColumn++;
            }

            IstabWindData &windData = istabWindMap[istab][wind];
            windData.istab = istab;
            windData.wind = wind;

            // Add values to the respective vectors
            windData.sig_x.push_back(sig_x);
            windData.sig_y.push_back(sig_y);
            windData.sig_z.push_back(sig_z);
            windData.ln_sig_x.push_back(std::log(sig_x));
            windData.ln_sig_y.push_back(std::log(sig_y));
            windData.ln_sig_z.push_back(std::log(sig_z));

            // Collect unique x-values for global ln_x calculation
            if (std::find(x_values.begin(), x_values.end(), x) == x_values.end())
            {
                x_values.push_back(x);
            }
        }
        file.close();

        // Sort x_values and calculate global ln_x
        std::sort(x_values.begin(), x_values.end());
        this->x = x_values;
        for (float x_val : x_values)
        {
            this->ln_x.push_back(std::log(x_val));
        }

        // Calculate global slope and intercept for ln_x
        computeLinearRegression(this->ln_x, this->slope_ln_x, this->intercept_ln_x);

        // Process each istab and wind data
        for (const auto &[istabKey, windMap] : istabWindMap)
        {
            IstabData istabData;
            istabData.istab = istabKey;

            for (const auto &[windKey, windData] : windMap)
            {
                IstabWindData processedWindData = windData;

                // Perform linear regression for ln_sig_x, ln_sig_y, ln_sig_z
                computeLinearRegression(windData.ln_sig_x, processedWindData.slope_ln_sig_x, processedWindData.intercept_ln_sig_x);
                computeLinearRegression(windData.ln_sig_y, processedWindData.slope_ln_sig_y, processedWindData.intercept_ln_sig_y);
                computeLinearRegression(windData.ln_sig_z, processedWindData.slope_ln_sig_z, processedWindData.intercept_ln_sig_z);

                istabData.istabwindData.push_back(processedWindData);
            }
            this->istabData.push_back(istabData);
        }

    }

    int dispersionCoef::find_nearest_wind_index(const std::vector<float> &wind_values, float wind) const
    {
        auto it = std::lower_bound(wind_values.begin(), wind_values.end(), wind);
        int idx = std::distance(wind_values.begin(), it);
        return idx == 0 ? idx : idx - 1;
    }

    int dispersionCoef::find_nearest_x_index(float ln_x_value) const
    {
        auto it = std::lower_bound(ln_x.begin(), ln_x.end(), ln_x_value);
        int idx = std::distance(ln_x.begin(), it);
        //printf("find_nearest_x_index: ln_x_value=%f, idx=%d\n", ln_x_value, idx);
        return idx == 0 ? idx : idx - 1;
    }

    std::vector<float> dispersionCoef::interpolate_sig_component(int istab, float wind, float x, const std::string &flag)
    {
        //printf("interpolate_sig_component: istab=%d, wind=%f, x=%f, flag=%s\n", istab, wind, x, flag.c_str());

        std::vector<float> interpolated_values;

        auto it_istab = std::find_if(istabData.begin(), istabData.end(), [&](const IstabData &d)
                                     { return d.istab == istab; });
        if (it_istab == istabData.end())
            return {}; // istab not found

        int wind_idx = find_nearest_wind_index(it_istab->wind, wind);
        //printf("wind_idx = %d\n", wind_idx);
        float wind1 = it_istab->wind[wind_idx];
        float wind2 = it_istab->wind[wind_idx + 1];
        float wind_weight = (wind - wind1) / (wind2 - wind1);

        //printf("wind1=%f, wind2=%f, wind_weight=%f\n", wind1, wind2, wind_weight);

        float ln_x_val = std::log(x);
        int x_idx = find_nearest_x_index(ln_x_val);
        //printf("x_idx = %d\n", x_idx);
        float ln_x1 = ln_x[x_idx];
        float ln_x2 = ln_x[x_idx + 1];
        float x_weight = (ln_x_val - ln_x1) / (ln_x2 - ln_x1);

        //printf("ln_x1=%f, ln_x2=%f, x_weight=%f\n", ln_x1, ln_x2, x_weight);

        auto &data1 = it_istab->istabwindData[wind_idx];
        auto &data2 = it_istab->istabwindData[wind_idx + 1];
        //printf("data1.ln_sig_x[x_idx]=%f, data2.ln_sig_x[x_idx]=%f\n", data1.ln_sig_x[x_idx], data2.ln_sig_x[x_idx]);
        //printf("data1.ln_sig_x[x_idx+1]=%f, data2.ln_sig_x[x_idx+1]=%f\n", data1.ln_sig_x[x_idx + 1], data2.ln_sig_x[x_idx + 1]);

        if (flag == "x" || flag == "all")
        {
        
            float ln_sig_x1 = (1 - wind_weight) * data1.ln_sig_x[x_idx] + wind_weight * data2.ln_sig_x[x_idx];
            float ln_sig_x2 = (1 - wind_weight) * data1.ln_sig_x[x_idx + 1] + wind_weight * data2.ln_sig_x[x_idx + 1];

            interpolated_values.push_back(std::exp((1 - x_weight) * ln_sig_x1 + x_weight * ln_sig_x2));
        }
        if (flag == "y" || flag == "all")
        {
            float ln_sig_y1 = (1 - wind_weight) * data1.ln_sig_y[x_idx] + wind_weight * data2.ln_sig_y[x_idx];
            float ln_sig_y2 = (1 - wind_weight) * data1.ln_sig_y[x_idx + 1] + wind_weight * data2.ln_sig_y[x_idx + 1];
            interpolated_values.push_back(std::exp((1 - x_weight) * ln_sig_y1 + x_weight * ln_sig_y2));
        }
        if (flag == "z" || flag == "all")
        {
            float ln_sig_z1 = (1 - wind_weight) * data1.ln_sig_z[x_idx] + wind_weight * data2.ln_sig_z[x_idx];
            float ln_sig_z2 = (1 - wind_weight) * data1.ln_sig_z[x_idx + 1] + wind_weight * data2.ln_sig_z[x_idx + 1];
            interpolated_values.push_back(std::exp((1 - x_weight) * ln_sig_z1 + x_weight * ln_sig_z2));
        }
        return interpolated_values;
    }

    float dispersionCoef::interpolate_x_from_sig_component(int istab, float wind, float sig_value, const std::string &flag)
    {
        // Find the IstabData for the specified istab value
        auto it_istab = std::find_if(istabData.begin(), istabData.end(), [&](const IstabData &d)
                                     { return d.istab == istab; });
        if (it_istab == istabData.end())
            return -1; // istab not found

        // Find the nearest wind index
        int wind_idx = find_nearest_wind_index(it_istab->wind, wind);
        float wind1 = it_istab->wind[wind_idx];
        float wind2 = it_istab->wind[wind_idx + 1];
        float wind_weight = (wind - wind1) / (wind2 - wind1);

        // Calculate the log of the target sig_value
        float ln_sig_value = std::log(sig_value);

        // Get pointers to the appropriate ln_sig_* vectors based on the flag
        const std::vector<float> *ln_sig_vec1 = nullptr;
        const std::vector<float> *ln_sig_vec2 = nullptr;

        if (flag == "x")
        {
            ln_sig_vec1 = &it_istab->istabwindData[wind_idx].ln_sig_x;
            ln_sig_vec2 = &it_istab->istabwindData[wind_idx + 1].ln_sig_x;
        }
        else if (flag == "y")
        {
            ln_sig_vec1 = &it_istab->istabwindData[wind_idx].ln_sig_y;
            ln_sig_vec2 = &it_istab->istabwindData[wind_idx + 1].ln_sig_y;
        }
        else if (flag == "z")
        {
            ln_sig_vec1 = &it_istab->istabwindData[wind_idx].ln_sig_z;
            ln_sig_vec2 = &it_istab->istabwindData[wind_idx + 1].ln_sig_z;
        }

        // Binary search to locate the position in ln_sig where ln_sig_value would fit
        auto it_lower = std::lower_bound(ln_sig_vec1->begin(), ln_sig_vec1->end(), ln_sig_value);
        int i = std::distance(ln_sig_vec1->begin(), it_lower) - 1;

        // Ensure the index is within bounds
        if (i < 0 || i >= ln_sig_vec1->size() - 1)
        {
            std::cerr << "Error: ln_sig_value out of interpolation bounds.\n";
            return -1;
        }

        // Get the bounding ln_sig values for interpolation
        float ln_sig1 = (1 - wind_weight) * (*ln_sig_vec1)[i] + wind_weight * (*ln_sig_vec2)[i];
        float ln_sig2 = (1 - wind_weight) * (*ln_sig_vec1)[i + 1] + wind_weight * (*ln_sig_vec2)[i + 1];

        // Interpolate to find the corresponding ln_x
        float x_weight = (ln_sig_value - ln_sig1) / (ln_sig2 - ln_sig1);
        return std::exp((1 - x_weight) * ln_x[i] + x_weight * ln_x[i + 1]);
    }

} // namespace FastPlume