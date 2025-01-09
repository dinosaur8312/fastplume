#include "dustIntegralTable.h"
#include "mathUtil.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <functional>

namespace FastPlume
{

    double dustIntegralTable::getIntegralAtX(double x_value) const
    {
        // Find the position in the x vector closest to x_value
        auto it = std::lower_bound(x.begin(), x.end(), x_value);

        // If x_value is outside the range of x, handle edge cases
        if (it == x.end())
            throw std::out_of_range("x_value is larger than the maximum value in x.");
        if (abs(*it - x_value)>1e-4)
            throw std::invalid_argument("x_value is not an element of the x vector.");

        // Return the corresponding integral value
        size_t index = it - x.begin();

     //   if((x_value>9.0)&&(x_value<11.0))
       // {
         //   printf("x_value = %f, index = %llu, integral = %f\n", x_value, index, integral[index]);
        //}
        return integral[index];
    }

    void dustIntegralTable::populateX(const std::vector<locDataRow> &locations)
    {
        if (locations.empty())
            throw std::runtime_error("Locations vector is empty");

        // Step 1: Extract raw x values from locDataRow
        std::vector<double> x_raw;
        for (const auto &loc : locations)
        {
            x_raw.push_back(loc.x);
        }

        // Step 2: Copy and sort unique x values
        std::sort(x_raw.begin(), x_raw.end());
        auto last = std::unique(x_raw.begin(), x_raw.end());
        x_raw.erase(last, x_raw.end());

        // Ensure values are positive for logarithmic scale
        for (double val : x_raw)
        {
            if (val <= 0.0)
                throw std::invalid_argument("Values in x_raw must be positive for logarithmic interpolation");
        }

        // Step 3: Interpolate additional points between neighboring x values in log scale
        const int numInterpolationPoints = 10; // Number of points in the log scale
        std::vector<double> interpolated_x;

        for (size_t i = 0; i < x_raw.size() - 1; ++i)
        {
            double x_start = x_raw[i];
            double x_end = x_raw[i + 1];

            // Add points in log scale between x_start and x_end
            for (int j = 0; j <= numInterpolationPoints; ++j)
            {
                double fraction = static_cast<double>(j) / numInterpolationPoints;
                double log_point = std::pow(10.0, std::log10(x_start) + fraction * (std::log10(x_end) - std::log10(x_start)));
                interpolated_x.push_back(log_point);
            }
        }
        interpolated_x.push_back(x_raw.back()); // Include the last value

        // Step 4: Assign the resulting vector to `x`
        x = std::move(interpolated_x);
        //printf("x size = %d\n", x.size());

        //print all x values
        //for (size_t i = 0; i < x.size(); ++i)
        //{
        //    printf("x[%d] = %f\n", i, x[i]);
       // }

 //       exit(0);
    }

    void dustIntegralTable::populateZfunc(const dispersionCoef &coefData)
    {
        // Initialize zfunc vector
        zfunc.resize(x.size());

        // Calculate zfunc values for each x
        for (size_t i = 0; i < x.size(); ++i)
        {
            float z_merge = std::max(0., x[i]) + zv;
            float sig_z = coefData.interpolate_sig_component(istab, wind, z_merge, "z")[0];
            zfunc[i] = zFunction(zval, zplume, hml, sig_z);
            if(i%50==0)
            {
                printf("x=%f, z_merge=%f, sig_z=%f, zfunc[%d] = %f\n", x[i], z_merge, sig_z, i, zfunc[i]);
            }
        }

        //printf("zfunc size = %d\n", zfunc.size());
    }

    void dustIntegralTable::calcIntegral()
    {
        // Initialize integral vector
        integral.resize(x.size());

        std::vector<double> integral_raw(x.size(), 0.0); // Intermediate vector for raw integrals

        // Compute cumulative trapezoidal integral
        for (size_t i = 1; i < x.size(); ++i)
        {
            double dx = x[i] - x[i - 1];
            double trapezoid = 0.5 * dx * (zfunc[i - 1] + zfunc[i]);
            integral_raw[i] = integral_raw[i - 1] + trapezoid;
        }
        //printf("integral_raw size = %d\n", integral_raw.size());

        // Find the integral at the lower limit x_ref
        double integral0 = 0.0;
        for (size_t i = 0; i < x.size(); ++i)
        {
            if (x[i] <= x_ref)
            {
                integral0 = integral_raw[i];
            }
            else
            {
                break;
            }
        }

        printf("integral0 = %f\n", integral0);

        // Adjust integrals to calculate from x_ref
        for (size_t i = 0; i < x.size(); ++i)
        {
            integral[i] = std::max(integral_raw[i] - integral0, 0.0);
            if(i%50==0)
            {
                printf("x=%f, zfunc=%f, integral[%d] = %f\n", x[i], zfunc[i], i, integral[i]);
            }
        }
    }

}