#pragma once

#include "csv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "SimConfig.hpp"

struct CSVDataRow
{
     int id;
    // int index;
    double x=0; // Assuming "X" represents a numeric value
    double y =0;
    double z = 0;
    // double D0;
    double sig_x =0;
    double sig_y=0;
    double sig_z=0;
    double sig_x0 =0;
    double sig_y0=0;
    double sig_z0=0;
    // double D0U_Q;
    // double coef_y;
    // double expon_y;
    // double coef_z;
    // double expon_z;
    // int iwind; // Iwind as integer type
    int istab=0; // Istab as integer type
    double wind=0.f;
    double mass=0.f;
    int icurve=0;
    double decay = 0.f;
    double vd = 0.f;

    double hml=0;
    double zplume=0;
    double zrcp;
    double t =0;
    double concentration =0;
    double dosage =0 ;
    double xv =0;
    double yv =0;
    double zv =0;
    double dur =0;

    double cpeak=0;
    double dinf=0;
    double xfunc=0;
    double xfuncp=0;
    double yfunc=0;
    double zfunc =0 ;
    double qyz =0 ;
    double ctip =0;
    double ctail =0;
    // char stab; // Stab as character type
    // std::string how; // How as string type
    std::string xyzt_file;
    std::string output_file;
};

struct xyztRow
{
     int iloc;
    double x=0; // Assuming "X" represents a numeric value
    double y =0;
    double z = 0;
    double t =0;
};

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
};

class IstabData
{
    public:
        int istab;
        std::vector<IstabWindData> istabwindData;
};

class DispersionCoefficients
{
    public:
       std::vector<IstabData> istabData;
       std::vector<float> x;
       std::vector<float> ln_x;
       float slope_ln_x;
       float intercept_ln_x;
};


class simulationData
{
    std::vector<int> id;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> sig_x;
    std::vector<double> sig_y;
    std::vector<double> sig_z;
    std::vector<double> sig_x0;
    std::vector<double> sig_y0;
    std::vector<double> sig_z0;
    std::vector<int> istab;
    std::vector<double> wind;
    std::vector<double> mass;
    std::vector<int> icurve;
    std::vector<double> decay;
    std::vector<double> vd; 
    std::vector<double> hml;
    std::vector<double> zplume;
    std::vector<double> zrcp;
    std::vector<double> t;
    std::vector<double> concentration;
    std::vector<double> dosage;
    std::vector<double> xv;
    std::vector<double> yv;
    std::vector<double> zv;
    std::vector<double> dur;
    std::vector<double> cpeak;
    std::vector<double> dinf;
    std::vector<double> xfunc;
    std::vector<double> xfuncp;
    std::vector<double> yfunc;
    std::vector<double> zfunc;
    std::vector<double> qyz;
    std::vector<double> ctip;
    std::vector<double> ctail;
    std::vector<std::string> xyzt_file;
    std::vector<std::string> output_file;
};

class CSVParser
{
public:
    static std::vector<CSVDataRow> parseCoefCSV(const std::string &filePath)
    {
        std::vector<CSVDataRow> rows;
        io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
        in.read_header(io::ignore_extra_column, "x", "sig_x", "sig_y", "sig_z", "istab", "wind");
        CSVDataRow row;
        while (in.read_row(row.x, row.sig_x, row.sig_y, row.sig_z, row.istab, row.wind))
        {
            rows.push_back(row);
        }
        return rows;
    }

    static std::vector<xyztRow> parsexyztCSV(const std::string &filePath)
    {
        std::vector<xyztRow> rows;
        io::CSVReader<5, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
        in.read_header(io::ignore_extra_column, "iloc", "x", "y", "z", "t");
        xyztRow row;
        while (in.read_row(row.iloc,row.x, row.y, row.z, row.t))
        {
            rows.push_back(row);
        }
        return rows;
    }

    static std::vector<CSVDataRow> parseRefCSV(const SimConfig &config)
    {
        std::vector<CSVDataRow> rows;

        // Check if the file name ends with "_02.csv"
        //bool isExtendedFormat = filePath.size() >= 7 && filePath.substr(filePath.size() - 7) == "_02.csv";

        auto filePath = config.refCSVPath;

        //print computeMode
        std::cout << "computeMode: " << config.computeMode << std::endl;



        if (config.computeMode ==1)
        {
            // For *_02.csv files, read 8 columns
            io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab", "speed", "x0x", "x0y", "x0z", "sig_x0", "sig_y0", "sig_z0");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z))
            {
                //printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 2)
        {
            io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "iplot", "zrcp", "zplume", "hml","sigz", "zfunc");
            CSVDataRow row;
            while (in.read_row(row.istab, row.x, row.y, row.z, row.sig_z, row.wind))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 3)
        {
            io::CSVReader<10, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","x","y","z", "zplume", "icurve","t");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.x, row.y, row.zrcp, row.zplume, row.icurve, row.t))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode==4)
        {
            io::CSVReader<12, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","x","sig_x0","sig_y0","sig_z0", "z","zplume", "icurve","t");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.x, row.sig_x,row.sig_y, row.sig_z, row.zrcp, row.zplume, row.icurve, row.t))
            {
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 5)
        {
            io::CSVReader<11, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","dur","x","y","z", "zplume", "icurve","t");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.dur,row.x, row.y, row.zrcp, row.zplume, row.icurve, row.t))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode==6)
        {
            io::CSVReader<14, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","x", "y","dur","sig_x0","sig_y0","sig_z0", "z","zplume", "icurve","t");
            CSVDataRow row;
            float istab;
            float icurve;
            while (in.read_row(istab, row.wind, row.hml,row.mass,row.x, row.y,row.dur, row.sig_x,row.sig_y, row.sig_z, row.zrcp, row.zplume, icurve, row.t))
            {
                row.istab = (int)istab;
                row.icurve = (int)icurve;
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 7)
        {
            io::CSVReader<13, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","dur","x","y","z", "zplume", "icurve","t", "decay", "vd");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.dur,row.x, row.y, row.zrcp, row.zplume, row.icurve, row.t, row.decay, row.vd))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 8)
        {
            io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","wind","hml","Q","x","y","t",  "dep_velocity");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.x, row.y,  row.t, row.vd))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 9)
        {
            io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "case", "Q_kg", "U", "istab","zi", "depletion_time","fn_fp_xyzt", "fn_fp_output");
            CSVDataRow row;
            while (in.read_row(row.id, row.mass, row.wind, row.istab, row.hml,row.dur, row.xyzt_file, row.output_file))
            {
                row.mass *= 1e6;
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                row.xyzt_file = "/home/xianlong/Code/mtplume/tests/unit_tests/location_files/" + row.xyzt_file ;
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 10)
        {
            io::CSVReader<9, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "case", "Q_kg", "dur","v_dep","U", "istab","zi", "fn_fp_xyzt", "fn_fp_output");
            CSVDataRow row;
            while (in.read_row(row.id, row.mass, row.dur, row.vd,row.wind, row.istab, row.hml, row.xyzt_file, row.output_file))
            {
                row.mass *= 1e6;
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                row.xyzt_file = "/home/xianlong/Code/mtplume/tests/unit_tests/location_files/" + row.xyzt_file ;
                rows.push_back(row);
            }
        }
         else if(config.computeMode == 11)
        {
            io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "case", "Q_kg", "dur","U", "istab","zi", "fn_fp_xyzt", "fn_fp_output");
            CSVDataRow row;
            while (in.read_row(row.id, row.mass, row.dur, row.wind, row.istab, row.hml, row.xyzt_file, row.output_file))
            {
                row.mass *= 1e6;
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                row.xyzt_file = "/home/xianlong/Code/mtplume/tests/unit_tests/location_files_hires/" + row.xyzt_file ;
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 12)
        {
            io::CSVReader<11, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "case", "Q_kg", "U", "istab","zi","sig_x0","sig_y0","sig_z0", "depletion_time","fn_fp_xyzt", "fn_fp_output");
            CSVDataRow row;
            while (in.read_row(row.id, row.mass, row.wind, row.istab, row.hml,row.sig_x0,row.sig_y0, row.sig_z0,row.dur, row.xyzt_file, row.output_file))
            {
                row.mass *= 1e6;
                row.xyzt_file = "/home/xianlong/Code/mtplume/tests/unit_tests/location_files/" + row.xyzt_file ;
                rows.push_back(row);
            }
        }

        else if(config.computeMode ==0)
        {
            io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab", "speed", "x", "sig_x0", "sig_y0", "sig_z0");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.x, row.sig_x, row.sig_y, row.sig_z))
            {
                row.y = std::nan("");
                row.z = std::nan("");
                rows.push_back(row);
            }
        }
        return rows;
    }
    static std::vector<CSVDataRow> parseRef2CSV(const std::string &filePath)
    {
        std::vector<CSVDataRow> rows;
        io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
        in.read_header(io::ignore_extra_column, "istab", "speed", "x", "sig_x0", "sig_y0", "sig_z0");
        CSVDataRow row;
        while (in.read_row(row.istab, row.wind, row.x, row.sig_x, row.sig_y, row.sig_z))
        {
            row.y = std::nan("");
            row.z = std::nan("");
            rows.push_back(row);
        }
        return rows;
    }
};
