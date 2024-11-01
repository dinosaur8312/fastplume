#pragma once
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "CSVParser.hpp"
#include "SimConfig.hpp"
#define PRTCHECK 0
void findFourCoefs(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                   int &id0, int &id1, int &id2, int &id3, bool &flag);
void findFourCoefs_sig(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float sig_x,
                   int &id0, int &id1, int &id2, int &id3, bool &flag,const int sigid);
void compareSigmaData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float x, CSVDataRow row,
                      float &sig_x, float &exp_log_sig_x_coef, float &error_logx,
                      const int sigid);
void compareXData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float sig_x, CSVDataRow row,
                      float &x, float &x_coef, float &error_x,
                      const int sigid);
void compareZfunction(std::vector<CSVDataRow> data, std::ofstream &outputFile);
void generateSample(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile);
void generateDose(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile);
void generateDoseBatch(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs);
void generateCompleteBatch(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs);
void generateSourceSigma(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile);
void generateComplete(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile);

template <int N>
double zReflections(double zrcp, double zplume, double hml, double sigz)
{
    double arg = (zrcp - zplume) / sigz;
    if (abs(arg) > 4.)
        return 0.f;
    double zf = exp(-0.5 * arg * arg);
    if constexpr (N < 5)
    {
        if (0.0 < zplume)
            zf += zReflections<N + 1>(zrcp, -zplume, hml, sigz);
        if (zplume < hml)
            zf += zReflections<N + 1>(zrcp, 2.0f * hml - zplume, hml, sigz);
    }
    else
    {
        hml;
    }
    return zf;
}

inline double zFunction(double zrcp, double zplume, double hml, double sigz)
{
    if((hml < zrcp)&&(zplume<=hml))
        return 0.;


    static constexpr double INV_ROOT2PI = 0.3989422804014327;
    float zrefl = 0.;
    if(zplume<=hml)
    {
        if(zrcp>hml)
            return 0.;
        if(sigz>hml)
            return 1. / hml;
        zplume = std::max(0.001, zplume);

        if(zplume<=hml)
            zplume = std::min(hml-0.001, zplume);

        zrefl = zReflections<0>(zrcp, zplume, hml, sigz);
        
    }
    else
    {
        if(zrcp<=hml)
            return 0.;
        float arg = (zrcp - zplume) / sigz;

        zrefl = exp(-0.5 * arg * arg);

        arg = (zrcp +2.0 * hml - zplume) / sigz;
        zrefl += exp(-0.5 * arg * arg);
    }


    return INV_ROOT2PI / sigz * zrefl;
}

inline double gaussFunction( double x)
{

    return  exp(-0.5 * x * x) ;
}
inline double pdfFunction( double y,double sigy = 1.)
{
    static constexpr float INV_ROOT2PI = 0.3989422804014327;
    float arg = y / sigy;

    return INV_ROOT2PI* exp(-0.5 * arg * arg) /sigy;
}

inline double cdfFunction(double x, double scale = 1.)
{
    static constexpr double INV_SQRT2 = 0.7071067811865475;
    return 0.5 * (1. + erf(INV_SQRT2 * (x/scale)));
}

inline double IcdfFunction(double x, double scale = 1.f)
{
    double xs = x/scale;

    return xs*cdfFunction(x, scale)+pdfFunction(x/scale);
}
class SigmaInterp
{

public:
    static void compareCSVdata(SimConfig &config)
    {
#if (PRTCHECK)
        printf("xk_flag, Begin compareCSVdata\n");
#endif
        int computeMode = config.computeMode;

        auto data = CSVParser::parseRefCSV(config);
        auto coefs = CSVParser::parseCoefCSV(config.coefCSVPath);

        auto coefData = CSVParser::parseCoefCSV_new(config.coefCSVPath);

        if(computeMode == 12)
        {
            generateCompleteBatch(data, coefs);
            return;
        }
        else if(computeMode >=9)
        {
            generateDoseBatch(data, coefs);
            return;
        }

        std::ofstream outputFile(config.outputCSVPath);
        if (!outputFile)
        {
            throw std::runtime_error("Cannot open the file: " + config.outputCSVPath);
        }

        if (computeMode == 2)
        {
            compareZfunction(data, outputFile);
            return;
        }

        // Further processing with config data...

        if(computeMode ==3)
        {
            generateSample(data, coefs, outputFile);
            return;
        }
        if(computeMode ==4)
        {
            generateSourceSigma(data, coefs, outputFile);
            return;
        }
        if((computeMode ==5)||(computeMode ==7)||(computeMode ==8))
        {
            generateDose(data, coefs, outputFile);
            return;
        }
        if(computeMode ==6)
        {
            generateComplete(data, coefs, outputFile);
            return;
        }

        // find min and max wind speed and x in coefs table
        double sig_x_min = 9999999.f;
        double sig_x_max = -9999999.f;
        double sig_y_min = 9999999.f;
        double sig_y_max = -9999999.f;
        double sig_z_min = 9999999.f;
        double sig_z_max = -9999999.f;
        double windmin = 9999999.f;
        double windmax = -9999999.f;

        for (int i = 0; i < coefs.size(); i++)
        {
            auto row = coefs[i];
            double sig_x = row.sig_x;
            double wind = row.wind;
            if (sig_x < sig_x_min)
                sig_x_min = sig_x;
            if (sig_x > sig_x_max)
                sig_x_max = sig_x;
            if (wind < windmin)
                windmin = wind;
            if (wind > windmax)
                windmax = wind;
            if (computeMode == 1)
            {
                double sig_y = row.sig_y;
                double sig_z = row.sig_z;
                if (sig_y < sig_y_min)
                    sig_y_min = sig_y;
                if (sig_y > sig_y_max)
                    sig_y_max = sig_y;
                if (sig_z < sig_z_min)
                    sig_z_min = sig_z;
                if (sig_z > sig_z_max)
                    sig_z_max = sig_z;
            }
        }
        // printf("xmin = %f, xmax = %f, windmin = %f, windmax = %f\n", xmin, xmax, windmin, windmax);

    /*
        if (computeMode == 0)
            outputFile << "ID,istab,wind,x,sig_x_calc,sig_y_calc,sig_z_calc,sig_x_ref,sig_y_ref,sig_z_ref,err_x,err_y,err_z,pass\n";
        else
            outputFile << "ID,istab,wind,x,y,z,sig_x_calc,sig_y_calc,sig_z_calc,sig_x_ref,sig_y_ref,sig_z_ref,err_x,err_y,err_z,pass\n";
*/
        if (computeMode == 0)
            outputFile << "ID,istab,wind,x,x_v,sig_x,sig_y,sig_z,err_x,pass\n";
        else
            outputFile << "ID,istab,wind,x,y,z,sig_x_calc,sig_y_calc,sig_z_calc,sig_x_ref,sig_y_ref,sig_z_ref,err_x,err_y,err_z,pass\n";
        int pass_count = 0;
        // loop over all rows in data
        for (int i = 0; i < data.size(); i++)
        {
            auto row = data[i];
            double sig_x = row.sig_x;
            double sig_y, sig_z;
            int istab = row.istab;
            double wind = row.wind;
            outputFile << i << "," << istab << "," << wind << "," << sig_x << ",";

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
            printf("\ni=%d\n", i);
            printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
            if (sig_x < sig_x_min)
                sig_x = sig_x_min;
            if (sig_x > sig_x_max)
                sig_x = sig_x_max;
            // double logx = log(x);
            // double logy, logz;

            if (computeMode == 1)
            {
                sig_y = row.sig_y;
                sig_z = row.sig_z;
                outputFile << sig_y << "," << sig_z << ",";
                if (sig_y < sig_y_min)
                    sig_y = sig_y_min;
                if (sig_y > sig_y_max)
                    sig_y = sig_y_max;
                if (sig_z < sig_z_min)
                    sig_z = sig_z_min;
                if (sig_z > sig_z_max)
                    sig_z = sig_z_max;

            }

            int id0, id1, id2, id3;
            bool flag = false;
            float sig_x_ref, sig_y_ref, sig_z_ref;
            float sig_x_cal, sig_y_cal, sig_z_cal;
            float err_x, err_y, err_z;
            //findFourCoefs(coefs, istab, wind, x, id0, id1, id2, id3, flag);
            findFourCoefs_sig(coefs, istab, wind, sig_x, id0, id1, id2, id3, flag,0);

#if (PRTCHECK)
           // printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
            //compareSigmaData(coefs, id0, id1, id2, id3, x, row, sig_x_ref, sig_x_cal, err_x, 0);
            compareXData(coefs, id0, id1, id2, id3, sig_x, row, sig_x_ref, sig_x_cal, err_x, 0);

            if (computeMode == 0)
            {
                compareSigmaData(coefs, id0, id1, id2, id3, sig_x, row, sig_y_ref, sig_y_cal, err_y, 1);
                compareSigmaData(coefs, id0, id1, id2, id3, sig_x, row, sig_z_ref, sig_z_cal, err_z, 2);
            }
            else
            {
                findFourCoefs(coefs, istab, wind, sig_y, id0, id1, id2, id3, flag);
#if (PRTCHECK)
                printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
                compareSigmaData(coefs, id0, id1, id2, id3, sig_y, row, sig_y_ref, sig_y_cal, err_y, 1);
                findFourCoefs(coefs, istab, wind, sig_z, id0, id1, id2, id3, flag);
#if (PRTCHECK)
                printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
                compareSigmaData(coefs, id0, id1, id2, id3, sig_z, row, sig_z_ref, sig_z_cal, err_z, 2);
            }

#if (PRTCHECK)
            printf(" sig_x = %f, sig_x_coef = %f, ERROR_LOGX=%f% \n", sig_x_ref, sig_x_cal, err_x);
            printf(" sig_y = %f, sig_y_coef = %f, ERROR_LOGY=%f% \n", sig_y_ref, sig_y_cal, err_y);
            printf(" sig_z = %f, sig_z_coef = %f, ERROR_LOGZ=%f% \n", sig_z_ref, sig_z_cal, err_z);
#endif
            outputFile << sig_x_cal << "," << sig_y_cal << "," << sig_z_cal << ",";
            outputFile << sig_x_ref << "," << sig_y_ref << "," << sig_z_ref << ",";
            outputFile << err_x << "," << err_y << "," << err_z << ",";
            //if (err_x < 0.1f && err_y < 0.1f && err_z < 0.1f)
            if (err_x < 0.1f )
            {
                outputFile << "Yes"
                           << "\n";
                pass_count++;
            }
            else
            {
                outputFile << "No"
                           << "\n";
            }
        } // end of loop over all rows in data
        outputFile.close();
        std::cout << "CSV file has been written successfully.\n";

        double pass_rate = (double)pass_count / (double)data.size() * 100;
        // print total number of test cases
        std::cout << "Total number of test cases = " << data.size() << "\n";
        std::cout << "Pass rate = " << pass_rate << "%\n";

#if (PRTCHECK)
        printf("xk_flag, compareCSVdata\n");
#endif
        return;
    }
};