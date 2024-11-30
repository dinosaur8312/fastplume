#include "FastPlumeImpl.h"
#include "jsonHandler.h"

#define EPSILON 0.00001

namespace FastPlume
{
    FastPlumeImpl::FastPlumeImpl() {}

    FastPlumeImpl::FastPlumeImpl(const std::string &JsonInputFileName)
    {
        // print message
        //  Process the JSON file to get the configuration
        taskConfig config = jsonHandler::processJsonFile(JsonInputFileName);

        // Process the CSV files to get the data
        m_coefData = dispersionCoef(config.coefCSVPath);
        m_taskData = taskData(config.taskCSVPath, "./");
    }

    void FastPlumeImpl::validateConfiguration() const
    {
        if (outputMethod == "CSV")
        {
            //if (outputFilePath.empty())
            //{
            //    throw std::runtime_error("Error: outputFilePath must be specified when outputMethod is CSV.");
            //}

            if (outputDirectory.empty())
            {
                std::cerr << "Warning: outputDirectory is not set. Output will be saved in the code execution directory." << std::endl;
            }
        }
    }

    FastPlumeImpl &FastPlumeImpl::setAttr(const std::string &attrName, const std::vector<double> &values)
    {
        m_taskData.setAttr(attrName, values);
        return *this;
    }
    FastPlumeImpl &FastPlumeImpl::setEvapAttr(const std::string &attrName, const std::vector<double> &values)
    {
        m_taskEvapData.setAttr(attrName, values);
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setAttr(const std::string &attrName, const std::vector<int> &values)
    {
        m_taskData.setAttr(attrName, values);
        return *this;
    }
    FastPlumeImpl &FastPlumeImpl::setEvapAttr(const std::string &attrName, const std::vector<int> &values)
    {
        m_taskEvapData.setAttr(attrName, values);
        return *this;
    }


    FastPlumeImpl &FastPlumeImpl::setAttr(const std::string &attrName, const std::vector<std::vector<double>> &values)
    {
        if (m_taskData.getTaskNum() <= 0)
        {
            throw std::invalid_argument("Task data not initialized. Please set task data before setting locData.");
        }
        // All tasks share the same locData
        if (values.size() < 2 || values.size() > 4)
        {
            throw std::invalid_argument("Invalid vector size for locdata_single. Expected 2, 3, or 4 vectors.");
        }

        locData loc;
        if (values.size() == 4)
        {
            loc.x = values[0];
            loc.y = values[1];
            loc.z = values[2];
            loc.t = values[3];
        }
        else if (values.size() == 3)
        {
            loc.x = values[0];
            loc.z = values[1];
            loc.y.assign(values[0].size(), 0.0); // Initialize y as zero
            loc.t = values[2];
        }
        else if (values.size() == 2)
        {
            loc.x = values[0];
            loc.y.assign(values[0].size(), 0.0); // Initialize y as zero
            loc.z.assign(values[0].size(), 0.0); // Initialize z as zero
            loc.t = values[1];
        }

        std::vector<locData> locDataAllTasks(m_taskData.getTaskNum(), loc);
        m_taskData.setAttr("locData", locDataAllTasks);
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setAttr(const std::string &attrName, const std::vector<std::vector<std::vector<double>>> &values)
    {
        if (m_taskData.getTaskNum() <= 0)
        {
            throw std::invalid_argument("Task data not initialized. Please set task data before setting locData.");
        }
        // All tasks have their own locData
        if (values.size() != m_taskData.getTaskNum())
        {
            throw std::invalid_argument("Invalid vector size for locdata_multiple. Expected size equal to number of tasks.");
        }

        std::vector<locData> locDataAllTasks;
        for (const auto &taskLocData : values)
        {
            if (taskLocData.size() < 2 || taskLocData.size() > 4)
            {
                throw std::invalid_argument("Invalid vector size for locdata_single. Expected 2, 3, or 4 vectors.");
            }

            locData loc;
            if (taskLocData.size() == 4)
            {
                loc.x = taskLocData[0];
                loc.y = taskLocData[1];
                loc.z = taskLocData[2];
                loc.t = taskLocData[3];
            }
            else if (taskLocData.size() == 3)
            {
                loc.x = taskLocData[0];
                loc.z = taskLocData[1];
                loc.y.assign(taskLocData[0].size(), 0.0); // Initialize y as zero
                loc.t = taskLocData[2];
            }
            else if (taskLocData.size() == 2)
            {
                loc.x = taskLocData[0];
                loc.y.assign(taskLocData[0].size(), 0.0); // Initialize y as zero
                loc.z.assign(taskLocData[0].size(), 0.0); // Initialize z as zero
                loc.t = taskLocData[1];
            }

            locDataAllTasks.push_back(loc);
        }

        m_taskData.setAttr("v_locData", locDataAllTasks);
        return *this;
    }

    // Helper setAttr for locData directly, hidden from user
    FastPlumeImpl &FastPlumeImpl::setAttr(const std::string &attrName, const std::vector<locData> &values)
    {
        m_taskData.setAttr(attrName, values);
        return *this;
    }

    template <typename T>
    std::vector<T> FastPlumeImpl::getAttr(const std::string &attrName) const {
        return m_taskData.getAttr<T>(attrName);
    }

    template <typename T>
    std::vector<T> FastPlumeImpl::getEvapAttr(const std::string &attrName) const {
        return m_taskEvapData.getAttr<T>(attrName);
    }


    FastPlumeImpl &FastPlumeImpl::setDispersionCoefCSV(const std::string &filePath)
    {
        // m_coefData.parseCSV(filePath);
        DispersionCoefCSVPath = filePath;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setAgentPropertiesCSV(const std::string &filePath)
    {
        // m_agentData.parseCSV(filePath);
        AgentPropertiesCSVPath = filePath;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setTaskDataCSV(const std::string &filePath)
    {
        // m_taskData.parseCSV(filePath);
        TaskDataCSVPath = filePath;
        return *this;
    }
    FastPlumeImpl &FastPlumeImpl::setEvapTaskDataCSV(const std::string &filePath)
    {
        // m_taskData.parseCSV(filePath);
        EvapTaskDataCSVPath = filePath;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setOutputFilePath(const std::string &filePath)
    {
        outputFilePath = filePath;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setEvapOutputFilePath(const std::string &filePath)
    {
        outputEvapFilePath = filePath;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setOutputMethod(const std::string &method)
    {
        outputMethod = method;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setEvapOutputMethod(const std::string &method)
    {
        outputEvapMethod = method;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setLocDataCSVImportDirectory(const std::string &directory)
    {
        // locDataDirectory = directory;
        LocDataCSVImportDirectory = directory;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setOutputDirectory(const std::string &directory)
    {
        outputDirectory = directory;
        return *this;
    }

    FastPlumeImpl &FastPlumeImpl::setEvapOutputDirectory(const std::string &directory)
    {
        outputEvapDirectory = directory;
        return *this;
    }

    float FastPlumeImpl::interpolate_x_from_sig_component(int istab, float wind, float sig_value, const std::string &flag)
    {
        return m_coefData.interpolate_x_from_sig_component(istab, wind, sig_value, flag);
    }

    std::vector<float> FastPlumeImpl::interpolate_sig_component(int istab, float wind, float x, const std::string &flag)
    {
        return m_coefData.interpolate_sig_component(istab, wind, x, flag);
    }

    double FastPlumeImpl::calcDepletion(const taskDataRow &row, double x, double z)
    {
        double x_high = x;
        double x_low = 10.0;
        int integral_steps = 100;

        double log_x_high = log(x_high);
        double log_x_low = log(x_low);
        double log_step = (log_x_high - log_x_low) / (integral_steps - 1);

        // Precompute x values in logarithmic scale for integration points
        std::vector<double> x_values(integral_steps);
        for (int i = 0; i < integral_steps; ++i)
        {
            x_values[i] = exp(log_x_low + i * log_step);
        }

        double sumz = 0;
        for (int i = 0; i < integral_steps - 1; ++i)
        {
            double x_mid = (x_values[i] + x_values[i + 1]) * 0.5;
            double dx = x_values[i + 1] - x_values[i];

            // Perform interpolation and zFunction calculations at midpoint x
            double sigz = this->interpolate_sig_component(row.istab, row.wind, x_mid, "z")[0];
            double zfunc = zFunction(z, row.zplume, row.hml, sigz);

            sumz += zfunc * dx;
        }

        return exp(-sumz * row.vd / row.wind);
    }

    std::vector<taskDataRow> FastPlumeImpl::getResults() const
    {
        return m_taskData.getAllTaskRows();
    }

    std::vector<taskEvapDataRow> FastPlumeImpl::getEvapResults() const
    {
        return m_taskEvapData.getAllTaskRows();
    }

    void FastPlumeImpl::printResult() const
    {
        std::vector<taskDataRow> results = m_taskData.getAllTaskRows();

        std::cout << "FastPlume Simulation Results:" << std::endl;
        for (size_t i = 0; i < results.size(); ++i)
        {
            const taskDataRow &row = results[i];
            std::cout << "Task " << i + 1 << ":\n";
            std::cout << "  id: " << row.id << "\n"
                      << "  istab: " << row.istab << ", wind: " << row.wind << "\n"
                      << "  sig_x0: " << row.sig_x0 << ", sig_y0: " << row.sig_y0
                      << ", sig_z0: " << row.sig_z0 << "\n"
                      << "  mass: " << row.mass << ", hml: " << row.hml << ", dur: " << row.dur
                      << "\n";

            std::cout << "\n  Locations:\n\n";
            for (const auto &loc : row.locations)
            {
                //print loc index
                std::cout << "  Location " << &loc - &row.locations[0] << ":\n";
                std::cout << "    x: " << loc.x
                          << ", y: " << loc.y
                          << ", z: " << loc.z
                          << ", t: " << loc.t << "\n"
                          << "    sig_x: " << loc.sig_x
                            << ", sig_y: " << loc.sig_y
                            << ", sig_z: " << loc.sig_z << "\n"
                          << "    zfunc: " << loc.zfunc
                            << ", yfunc: " << loc.yfunc
                            << ", xfunc: " << loc.xfunc << "\n"
                          << "    qyz: " << loc.qyz
                            << ", cpeak: " << loc.cpeak
                            << ", concentration: " << loc.concentration
                            << "    dinf: " << loc.dinf
                            << ", dosage: " << loc.dosage
                            << "\n\n";
            }
            std::cout << "--------------------------------\n";
        }
    }

    void FastPlumeImpl::printEvapResult() const
    {
        std::vector<taskEvapDataRow> results = m_taskEvapData.getAllTaskRows();

        std::cout << "FastPlume Evaporation Simulation Results:" << std::endl;
        for (size_t i = 0; i < results.size(); ++i)
        {
            const taskEvapDataRow &row = results[i];
            std::cout << "Task " << i + 1 << ":\n";
            std::cout << "  id: " << row.id << "\n"
                      << "  surfaceTemperature: " << row.surfaceTemperature << ", AtmPressure: " << row.AtmPressure
                      << ", windSpeed: " << row.windSpeed << "\n"
                      << "  surfaceType: " << row.surfaceType << ", agentName: " << row.agentName
                      << ", puddleLength: " << row.puddleLength << ", puddleWidth: " << row.puddleWidth
                      << ", puddle_area: " << row.puddle_area << "\n"
                      << "  windDirection: " << row.windDirection << ", lengthAlongWind: " << row.lengthAlongWind
                      << ", quantityRemaining: " << row.quantityRemaining << "\n"
                      << "  vaporPressure: " << row.vaporPressure << ", puddleLengthAlongWind: " << row.puddleLengthAlongWind
                      << ", airDensity: " << row.airDensity << ", airViscosity: " << row.airViscosity
                      << ", schmidtNumberD: " << row.schmidtNumberD << ", airDiffusivity: " << row.airDiffusivity
                      << "\n";
            std::cout << "--------------------------------\n";
        }
    }

    void FastPlumeImpl::run()
    {
        // Validate the configuration
        validateConfiguration();
        if (!DispersionCoefCSVPath.empty())
        {
            m_coefData = dispersionCoef(DispersionCoefCSVPath);
        }
        if (!TaskDataCSVPath.empty())
        {

            m_taskData = taskData(TaskDataCSVPath, LocDataCSVImportDirectory);
        }

        m_taskData.matchData();
        //m_taskData.printData();
        // print message
        std::cout << "Running FastPlumeImpl with " << this->getTaskNum() << " tasks\n";

        // print m_taskData
        // m_taskData.printData();
        // sort m_coefData
        // m_coefData.sortData();
        // print m_coefData
        // m_coefData.printData();

        // Implementation of the core functionality
        // e.g., process `coefData` and `refData`
        // find min and max wind speed and x in coefs table
        // loop over all case in taskData

        for (int i = 0; i < this->getTaskNum(); i++)
        {
            auto row = m_taskData.getRow(i);
            // Add output file prefix to ../tests directory

            std::ofstream outputFile;
            if (outputMethod == "CSV")
            {
                std::string output_file = outputDirectory + row.output_file;

                outputFile.open(output_file);
                outputFile << "case,istab,wind,x,y,z,t,dur,";
                outputFile << "sig_x,sig_y,sig_z,zfunc,yfunc,xfunc,qyz,cpeak,concentration,dinf,dose,";
                if (row.vd > 0)
                    outputFile << "c_surf,c_surf_inf, C_dep, D_dep, delta_dep";
                outputFile << "\n";
                //std::cout << "Writing to " << output_file << "\n";
            }
            // std::cout << "row.dur = " << row.dur << "\n";
            double sig_x0 = row.sig_x0;
            double sig_y0 = row.sig_y0;
            double sig_z0 = row.sig_z0;
            // printf("sig_x0 = %f, sig_y0 = %f, sig_z0 = %f\n", sig_x0, sig_y0, sig_z0);
            auto locdata = row.locations;

            int istab = row.istab;
            double wind = row.wind;

            bool flag = false;

            row.xv = sig_x0 > EPSILON ? this->interpolate_x_from_sig_component(istab, wind, sig_x0, "x") : 0;

            row.yv = sig_y0 > EPSILON ? this->interpolate_x_from_sig_component(istab, wind, sig_y0, "y") : 0;

            row.zv = sig_z0 > EPSILON ? this->interpolate_x_from_sig_component(istab, wind, sig_z0, "z") : 0;

            //  printf("i = %d, row.xv = %f, row.yv = %f, row.zv = %f\n", i, row.xv, row.yv, row.zv);

            //   printf("locdata.getLocNum() = %d\n", locdata.getLocNum());

            for (int j = 0; j < locdata.size(); j++)
            {
                double x = locdata[j].x;
                double y = locdata[j].y;
                double zrcp = locdata[j].z;
                double t = locdata[j].t;
                // printf("i = %d, j = %d, x = %f, y = %f, z = %f, t = %f\n", i, j, x, y, zrcp, t);

                double x_merge = std::max(0., x) + row.xv;
                double y_merge = std::max(0., x) + row.yv;
                double z_merge = std::max(0., x) + row.zv;

                // printf("i = %d, j = %d, x = %f, y = %f, z = %f, t = %f, row.dur = %f, x_merge = %f, y_merge = %f, z_merge = %f\n", i, j, x, y, zrcp, t, row.dur, x_merge, y_merge, z_merge);

                double sig_x = this->interpolate_sig_component(istab, wind, x_merge, "x")[0];
                double sig_y = this->interpolate_sig_component(istab, wind, y_merge, "y")[0];
                double sig_z = this->interpolate_sig_component(istab, wind, z_merge, "z")[0];

                double zfunc = zFunction(zrcp, row.zplume, row.hml, sig_z);
                double yfunc = pdfFunction(y, sig_y);

                // printf("i = %d, j = %d, x = %f, y = %f, z = %f, t = %f, row.dur = %f, sig_x = %f, sig_y = %f, sig_z = %f, zfunc = %f, yfunc = %f\n", i, j, x, y, zrcp, t, row.dur, sig_x, sig_y, sig_z, zfunc, yfunc);

                double qyz = row.mass * yfunc * zfunc;
                double xfunc, concentration, cpeak, xfuncp;
                double tip, tail, constant, tip1, tip0;
                double dosage;
                double dinf;

                double delta_dep, C_dep, D_dep;
                double c_surf, c_surf_inf;
                if (t >= 0)
                {
                    if (row.dur < 1)
                    {
                        xfunc = pdfFunction(row.wind * t - x, sig_x);
                        concentration = xfunc * qyz;
                        // print using scientific notation
                        // printf("xk_flag, pdfa = %f, xfunc = %f, qyz = %f, concentration = %8.4e\n", row.wind * row.t - row.x, xfunc, qyz, concentration);
                        xfuncp = pdfFunction(std::min(x, 0.), sig_x);
                        cpeak = xfuncp * qyz;
                        tail = 0;
                        tip = 0;

                        double arg0 = -x;
                        double arg1 = row.wind * t - x;
                        xfunc = (cdfFunction(arg1, sig_x) - cdfFunction(arg0, sig_x)) / row.wind;
                        dosage = xfunc * qyz;
                        double xfuncinf = (1.f - cdfFunction(arg0, sig_x)) / wind;
                        dinf = xfuncinf * qyz;
                    }
                    else
                    {
                        // Calculate the tip
                        double tip_time = t; // Adjust according to Python's tpeak function logic if necessary
                        tip = cdfFunction(wind * tip_time - x, sig_x);

                        // Calculate the tail
                        double ut = std::max(0., t - row.dur) * wind; // corrected for the release duration
                        tail = cdfFunction(ut - x, sig_x);

                        // Calculate concentration
                        concentration = (tip - tail) * qyz / row.wind / row.dur;

                        double tp = std::max(row.dur, 0.5 * row.dur + x / wind);

                        // Calculate peak concentration
                        double tipp = cdfFunction(row.wind * tp - x, sig_x);
                        double utp = std::max(0., tp - row.dur) * wind;
                        double tailp = cdfFunction(utp - x, sig_x);
                        cpeak = (tipp - tailp) * qyz / row.wind / row.dur;

                        tip1 = IcdfFunction(row.wind * t - x, sig_x);
                        tip0 = IcdfFunction(-x, sig_x);
                        tip = tip1 - tip0;

                        // For t<=T, tail is attached to the source
                        double tail0 = cdfFunction(-x, sig_x) * std::min(t, row.dur) * wind / sig_x;

                        // For t>=T, tail is moving
                        double arg = row.wind * std::max(0., t - row.dur);
                        double tail2 = IcdfFunction(arg - x, sig_x);
                        double tail1 = IcdfFunction(-x, sig_x);
                        tail = tail2 - tail1 + tail0;

                        constant = qyz * sig_x / (row.dur * (wind * wind));
                        dosage = (tip - tail) * constant;
                        dinf = qyz / wind * cdfFunction(x, sig_x);
                    }

                    if (row.vd >= 0)
                    {
                        delta_dep = calcDepletion(row, x, zrcp);
                        C_dep = concentration * delta_dep;
                        D_dep = dosage * delta_dep;
                        c_surf = row.vd * D_dep;
                        c_surf_inf = row.vd * dinf * delta_dep;
                    }
                } // if (t >= 0)

                // printf("i = %d, j = %d, x = %f, y = %f, z = %f, t = %f, row.dur = %f, sig_x = %f, sig_y = %f, sig_z = %f, zfunc = %f, yfunc = %f, xfunc = %f, qyz = %f, cpeak = %f, concentration = %f, dinf = %f, dosage = %f\n", i, j, x, y, zrcp, t, row.dur, sig_x, sig_y, sig_z, zfunc, yfunc, xfunc, qyz, cpeak, concentration, dinf, dosage);
                if (outputMethod == "CSV")
                {

                    outputFile << i << "," << istab << "," << wind << ",";
                    outputFile << x << "," << y << "," << zrcp << ",";
                    outputFile << t << "," << row.dur << ",";
                    outputFile << sig_x << "," << sig_y << "," << sig_z << ",";
                    outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
                    outputFile << std::scientific << std::setprecision(8) << cpeak << "," << concentration << "," << dinf << "," << dosage << ",";
                    if (row.vd > 0)
                    {
                        outputFile << std::scientific << std::setprecision(8) << c_surf << "," << c_surf_inf << "," << C_dep << "," << D_dep << "," << delta_dep;
                        ;
                    }
                    outputFile << "\n";
                }

                // Write data back to taskDataRow if outputMethod is vector
                if (outputMethod == "Vector")
                {
                    //print
                    locdata[j].sig_x = sig_x;
                    locdata[j].sig_y = sig_y;
                    locdata[j].sig_z = sig_z;

                    locdata[j].concentration = concentration;
                    locdata[j].dosage = dosage;
                    locdata[j].cpeak = cpeak;
                    locdata[j].dinf = dinf;
                    locdata[j].xfunc = xfunc;
                    locdata[j].xfuncp = xfuncp;
                    locdata[j].yfunc = yfunc;
                    locdata[j].zfunc = zfunc;
                    locdata[j].qyz = qyz;
                    locdata[j].ctip = tip;
                    locdata[j].ctail = tail;
                }

            } // for (int j = 0; j < xyzt_rows.size(); j++)

            if (outputMethod == "CSV")
            {
                outputFile.close();
                std::cout << "CSV file written to " << outputDirectory + row.output_file << "\n";
            }

            if (outputMethod == "Vector")
            {
                row.locations = locdata;
                m_taskData.setRow(i, row);
            }
            // exit(0);
        } // end of loop over all rows in data

        // outputFile.close();
        std::cout << "Run completed successfully.\n";

    } // end of run

    void FastPlumeImpl::run_evap()
    {
        return;
    }

} // namespace FastPlume