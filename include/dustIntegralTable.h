#ifndef TASKDUSTDATA_H
#define TASKDUSTDATA_H

#include <iostream>
#include <vector>
#include <string>
#include "taskDataRow.h"
#include "taskData.h"
#include "dispersionCoef.h"

namespace FastPlume
{
    class dustIntegralTable
    {
    public:
        double zplume;
        double hml;
        double zv;
        int istab;
        float wind;
        double x_ref;
        std::vector<double> x;
        std::vector<double> z;
        std::vector<double> sig_z;
        std::vector<double> zfunc;
        std::vector<double> integral;



        // Constructor taking only taskDataRow as input
        explicit dustIntegralTable(const taskDataRow &row, const dispersionCoef &coefData)
        {
            // Initialize scalar fields
            zplume = row.zplume;
            hml = row.hml;
            zv = row.zv;
            istab = row.istab;
            wind = row.wind;
            x_ref = 10.0;

            // Extract and process `x` values from row.locations
            populateX(row.locations);

            populateZfunc(coefData);

            calcIntegral();
        }

        double getIntegralAtX(double x) const;


    private:
         void populateX(const std::vector<locDataRow> &locations);
         void populateZfunc(const dispersionCoef &coefData);
         void calcIntegral();
    };

}

#endif // TASKDUSTDATA_H