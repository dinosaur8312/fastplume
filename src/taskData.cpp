#include "taskData.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>

namespace FastPlume
{

    std::vector<locDataRow> locData::toLocDataRows() const
    {
        std::vector<locDataRow> locDataRows;
        for (size_t i = 0; i < x.size(); ++i)
        {
            locDataRow row;
            row.iloc = iloc[i];
            row.x = x[i];
            row.y = y[i];
            row.z = z[i];
            row.t = t[i];
            row.sig_x = sig_x[i];
            row.sig_y = sig_y[i];
            row.sig_z = sig_z[i];
            row.concentration = concentration[i];
            row.dosage = dosage[i];
            row.cpeak = cpeak[i];
            row.dinf = dinf[i];
            row.xfunc = xfunc[i];
            row.xfuncp = xfuncp[i];
            row.yfunc = yfunc[i];
            row.zfunc = zfunc[i];
            row.qyz = qyz[i];
            row.ctip = ctip[i];
            row.ctail = ctail[i];

            locDataRows.push_back(row);
        }
        return locDataRows;
    }

    void locData::printData() const
    {
        std::cout << "LocData:" << std::endl;
        std::cout << "iloc: ";
        for (const auto &val : iloc)
            std::cout << val << " ";
        std::cout << "\nx: ";
        for (const auto &val : x)
            std::cout << val << " ";
        std::cout << "\ny: ";
        for (const auto &val : y)
            std::cout << val << " ";
        std::cout << "\nz: ";
        for (const auto &val : z)
            std::cout << val << " ";
        std::cout << "\nsig_x: ";
        for (const auto &val : sig_x)
            std::cout << val << " ";
        std::cout << "\nsig_y: ";
        for (const auto &val : sig_y)
            std::cout << val << " ";
        std::cout << "\nsig_z: ";
        for (const auto &val : sig_z)
            std::cout << val << " ";
        std::cout << "\nt: ";
        for (const auto &val : t)
            std::cout << val << " ";
        std::cout << "\nconcentration: ";
        for (const auto &val : concentration)
            std::cout << val << " ";
        std::cout << "\ndosage: ";
        for (const auto &val : dosage)
            std::cout << val << " ";
        std::cout << "\ncpeak: ";
        for (const auto &val : cpeak)
            std::cout << val << " ";
        std::cout << "\ndinf: ";
        for (const auto &val : dinf)
            std::cout << val << " ";
        std::cout << "\nxfunc: ";
        for (const auto &val : xfunc)
            std::cout << val << " ";
        std::cout << "\nxfuncp: ";
        for (const auto &val : xfuncp)
            std::cout << val << " ";
        std::cout << "\nyfunc: ";
        for (const auto &val : yfunc)
            std::cout << val << " ";
        std::cout << "\nzfunc: ";
        for (const auto &val : zfunc)
            std::cout << val << " ";
        std::cout << "\nqyz: ";
        for (const auto &val : qyz)
            std::cout << val << " ";
        std::cout << "\nctip: ";
        for (const auto &val : ctip)
            std::cout << val << " ";
        std::cout << "\nctail: ";
        for (const auto &val : ctail)
            std::cout << val << " ";
        std::cout << std::endl;
        std::cout << "\n----------------\n";
    }
    void locData::parseCSV(const std::string &filePath, const std::string &directory)
    {
        // Define the full file path
        std::string fullFilePath = directory + filePath;
        std::ifstream file(fullFilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open locData CSV file: " + fullFilePath);
        }

        std::string line;
        // Read the header line
        std::getline(file, line);
        std::istringstream headerStream(line);
        std::unordered_map<std::string, int> columnIndices;
        std::string columnName;
        int index = 0;

        // Map column names to indices for easy lookup
        bool hasX = false, hasY = false, hasZ = false, hasT = false;
        while (std::getline(headerStream, columnName, ','))
        {
            columnIndices[columnName] = index++;
            if (columnName == "x")
                hasX = true;
            else if (columnName == "y")
                hasY = true;
            else if (columnName == "z")
                hasZ = true;
            else if (columnName == "t")
                hasT = true;
        }

        // Check if required columns are present
        if (!(hasX && hasY && hasZ && hasT))
        {
            throw std::runtime_error("Required columns (x, y, z, t) are missing in locData CSV: " + fullFilePath);
        }

        // Parse each line of data
        while (std::getline(file, line))
        {
            std::istringstream lineStream(line);
            std::string cell;
            int colIdx = 0;

            // Temporary variables to store parsed values
            int ilocVal = 0;
            double xVal = 0.0, yVal = 0.0, zVal = 0.0, tVal = 0.0;

            // Parse each cell and assign to the corresponding attribute based on column index
            while (std::getline(lineStream, cell, ','))
            {
                if (columnIndices.find("iloc") != columnIndices.end() && colIdx == columnIndices["iloc"])
                {
                    ilocVal = std::stoi(cell);
                }
                else if (colIdx == columnIndices["x"])
                {
                    xVal = std::stod(cell);
                }
                else if (colIdx == columnIndices["y"])
                {
                    yVal = std::stod(cell);
                }
                else if (colIdx == columnIndices["z"])
                {
                    zVal = std::stod(cell);
                }
                else if (colIdx == columnIndices["t"])
                {
                    tVal = std::stod(cell);
                }
                colIdx++;
            }

            // Store values in locData vectors
            iloc.push_back(ilocVal);
            x.push_back(xVal);
            y.push_back(yVal);
            z.push_back(zVal);
            t.push_back(tVal);
        }

        // Initialize other locData attributes with the size of x
        size_t dataSize = x.size();
        sig_x.resize(dataSize);
        sig_y.resize(dataSize);
        sig_z.resize(dataSize);
        concentration.resize(dataSize);
        dosage.resize(dataSize);
        cpeak.resize(dataSize);
        dinf.resize(dataSize);
        xfunc.resize(dataSize);
        xfuncp.resize(dataSize);
        yfunc.resize(dataSize);
        zfunc.resize(dataSize);
        qyz.resize(dataSize);
        ctip.resize(dataSize);
        ctail.resize(dataSize);

        file.close();
    }

    taskData::taskData(const std::string &csvFilePath, const std::string &locDataDirectory)
    {
        parseCSV(csvFilePath, locDataDirectory);
        // printData();
    }

    

    void taskData::parseCSV(const std::string &csvFilePath, const std::string &locDataDirectory)
    {
        std::ifstream file(csvFilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open CSV file: " + csvFilePath);
        }

        std::string line;
        std::getline(file, line);
        std::istringstream headerStream(line);
        std::unordered_map<std::string, int> columnIndices;
        std::string columnName;
        int index = 0;
        bool hasX = false, hasY = false, hasZ = false, hasT = false, hasFnFpXyzt = false;

        // Map column names to indices for easy lookup
        while (std::getline(headerStream, columnName, ','))
        {
            columnIndices[columnName] = index++;
            if (columnName == "x")
                hasX = true;
            else if (columnName == "y")
                hasY = true;
            else if (columnName == "z")
                hasZ = true;
            else if (columnName == "t")
                hasT = true;
            else if (columnName == "fn_fp_xyzt")
                hasFnFpXyzt = true;
        }

        // Check required columns
        const std::vector<std::string> requiredColumns = {"istab"};
        for (const auto &col : requiredColumns)
        {
            if (columnIndices.find(col) == columnIndices.end())
            {
                throw std::runtime_error("Missing required column in CSV: " + col);
            }
        }

        // Check for exclusive conditions
        if ((hasX || hasY || hasZ || hasT) && hasFnFpXyzt)
        {
            throw std::runtime_error("Both (x, y, z, t) and fn_fp_xyzt columns found; only one set should be present.");
        }
        else if (!((hasX && hasY && hasZ && hasT) || hasFnFpXyzt))
        {
            throw std::runtime_error("Neither (x, y, z, t) nor fn_fp_xyzt found; one must be present.");
        }

        // Read data lines
        while (std::getline(file, line))
        {
            std::istringstream lineStream(line);
            std::string cell;
            int colIdx = 0;

            // Temporary variables to store values
            int istabVal = 0;
            double windVal = 0.0, hmlVal = 0.0, qMgVal = 0.0, durVal = 0.0, vDepVal = 0.0;
            double sigX0Val = 0.0, sigY0Val = 0.0, sigZ0Val = 0.0;
            double xVal = 0.0, yVal = 0.0, zVal = 0.0, tVal = 0.0;
            std::string fnFpXyzt, fnFpOutput;
            double zplumeVal = 0.0, decayVal = 0.0;

            // locData instance to store locational data if available
            locData location;

            // Parse each cell and assign to the corresponding attribute based on column index
            while (std::getline(lineStream, cell, ','))
            {
                if (colIdx == columnIndices["istab"])
                    istabVal = std::stoi(cell);
                else if (colIdx == columnIndices["wind"] || colIdx == columnIndices["U"])
                    windVal = std::stod(cell);
                else if (colIdx == columnIndices["hml"] || colIdx == columnIndices["zi"])
                    hmlVal = std::stod(cell);
                else if (colIdx == columnIndices["Q_mg"] || colIdx == columnIndices["Q"])
                    qMgVal = std::stod(cell);
                else if (colIdx == columnIndices["Q_kg"])
                    qMgVal = std::stod(cell) * 1e6;
                else if (colIdx == columnIndices["dur"] || colIdx == columnIndices["depletion_time"])
                    durVal = std::stod(cell);
                else if (colIdx == columnIndices["v_dep"] || colIdx == columnIndices["vd"])
                    vDepVal = std::stod(cell);
                // Optional columns
                else if (columnIndices.find("sig_x0") != columnIndices.end() && colIdx == columnIndices["sig_x0"])
                    sigX0Val = std::stod(cell);
                else if (columnIndices.find("sig_y0") != columnIndices.end() && colIdx == columnIndices["sig_y0"])
                    sigY0Val = std::stod(cell);
                else if (columnIndices.find("sig_z0") != columnIndices.end() && colIdx == columnIndices["sig_z0"])
                    sigZ0Val = std::stod(cell);

                // Populate locData with x, y, z, t if available
                else if (columnIndices.find("x") != columnIndices.end() && colIdx == columnIndices["x"])
                {
                    xVal = std::stod(cell);
                    location.x.push_back(xVal);
                }
                else if (columnIndices.find("y") != columnIndices.end() && colIdx == columnIndices["y"])
                {
                    yVal = std::stod(cell);
                    location.y.push_back(yVal);
                }
                else if (columnIndices.find("z") != columnIndices.end() && colIdx == columnIndices["z"])
                {
                    zVal = std::stod(cell);
                    location.z.push_back(zVal);
                }
                else if (columnIndices.find("t") != columnIndices.end() && colIdx == columnIndices["t"])
                {
                    tVal = std::stod(cell);
                    location.t.push_back(tVal);
                }
                else if (columnIndices.find("fn_fp_xyzt") != columnIndices.end() && colIdx == columnIndices["fn_fp_xyzt"])
                    fnFpXyzt = cell;
                else if (columnIndices.find("fn_fp_output") != columnIndices.end() && colIdx == columnIndices["fn_fp_output"])
                    fnFpOutput = cell;
                else if (columnIndices.find("zplume") != columnIndices.end() && colIdx == columnIndices["zplume"])
                    zplumeVal = std::stod(cell);
                else if (columnIndices.find("decay") != columnIndices.end() && colIdx == columnIndices["decay"])
                    decayVal = std::stod(cell);

                colIdx++;
            }
            // Store the parsed values in class member vectors
            istab.push_back(istabVal);
            wind.push_back(windVal);
            hml.push_back(hmlVal);
            mass.push_back(qMgVal);
            dur.push_back(durVal);
            vd.push_back(vDepVal);

            sig_x0.push_back(sigX0Val);
            sig_y0.push_back(sigY0Val);
            sig_z0.push_back(sigZ0Val);
            xv.push_back(xVal);
            yv.push_back(yVal);
            zv.push_back(zVal);
            zplume.push_back(zplumeVal);
            decay.push_back(decayVal);

            xyzt_file.push_back(fnFpXyzt);
            output_file.push_back(fnFpOutput);

            if (hasFnFpXyzt)
            {
                location.parseCSV(fnFpXyzt, locDataDirectory);
            }
            v_locData.push_back(location);
        }

        file.close();
        id.resize(istab.size()); // Resize id vector to the size of istab
        std::iota(id.begin(), id.end(), 0);

        // taskNum = id.size();
    }

   

    taskData::~taskData()
    {
        // Destructor logic if any
    }

    std::vector<taskDataRow> taskData::getAllTaskRows() const
    {
        std::vector<taskDataRow> taskRows;

        for (int i = 0; i < getTaskNum(); ++i)
        {
            taskDataRow row = getRow(i);                  // Get task data row
            row.locations = v_locData[i].toLocDataRows(); // Populate location data rows
            taskRows.push_back(row);
        }

        return taskRows;
    }

    // Match data sizes across vectors
    void taskData::matchData()
    {
        // Get the target size based on `istab`
        size_t targetSize = istab.size();

        // Lambda to resize vectors in `taskData` to match `istab` size
        auto resizeToTarget = [targetSize](auto &vec)
        {
            if (vec.size() < targetSize)
                vec.resize(targetSize, typename std::decay_t<decltype(vec)>::value_type{});
        };

        // Apply resizing to all taskData vectors
        resizeToTarget(id);
        resizeToTarget(sig_x0);
        resizeToTarget(sig_y0);
        resizeToTarget(sig_z0);
        resizeToTarget(wind);
        resizeToTarget(mass);
        resizeToTarget(decay);
        resizeToTarget(vd);
        resizeToTarget(hml);
        resizeToTarget(zplume);
        resizeToTarget(xv);
        resizeToTarget(yv);
        resizeToTarget(zv);
        resizeToTarget(dur);
        resizeToTarget(xyzt_file);
        resizeToTarget(output_file);

        // Ensure `v_locData` vectors match `x` size in each locData instance
        for (auto &loc : v_locData)
        {
            size_t locTargetSize = loc.x.size();
            auto resizeLocToTarget = [locTargetSize](auto &locVec)
            {
                if (locVec.size() < locTargetSize)
                    locVec.resize(locTargetSize, typename std::decay_t<decltype(locVec)>::value_type{});
            };

            // Apply resizing to all locData vectors
            resizeLocToTarget(loc.iloc);
            resizeLocToTarget(loc.y);
            resizeLocToTarget(loc.z);
            resizeLocToTarget(loc.sig_x);
            resizeLocToTarget(loc.sig_y);
            resizeLocToTarget(loc.sig_z);
            resizeLocToTarget(loc.t);
            resizeLocToTarget(loc.concentration);
            resizeLocToTarget(loc.dosage);
            resizeLocToTarget(loc.cpeak);
            resizeLocToTarget(loc.dinf);
            resizeLocToTarget(loc.xfunc);
            resizeLocToTarget(loc.xfuncp);
            resizeLocToTarget(loc.yfunc);
            resizeLocToTarget(loc.zfunc);
            resizeLocToTarget(loc.qyz);
            resizeLocToTarget(loc.ctip);
            resizeLocToTarget(loc.ctail);
        }
    }

    // Template method to set attributes
    template <typename T>
    void taskData::setAttr(const std::string &attrName, const std::vector<T> &values)
    {
        if constexpr (std::is_same<T, int>::value)
        {
            if (attrName == "id")
                id = values;
            else if (attrName == "istab")
                istab = values;
            else
                throw std::invalid_argument("Invalid attribute name for int vector");
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            if (attrName == "sig_x0")
                sig_x0 = values;
            else if (attrName == "sig_y0")
                sig_y0 = values;
            else if (attrName == "sig_z0")
                sig_z0 = values;
            else if (attrName == "wind")
                wind = values;
            else if (attrName == "mass")
                mass = values;
            else if (attrName == "decay")
                decay = values;
            else if (attrName == "vd")
                vd = values;
            else if (attrName == "hml")
                hml = values;
            else if (attrName == "zplume")
                zplume = values;
            else if (attrName == "xv")
                xv = values;
            else if (attrName == "yv")
                yv = values;
            else if (attrName == "zv")
                zv = values;
            else if (attrName == "dur")
                dur = values;
            else
                throw std::invalid_argument("Invalid attribute name for double vector");
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            if (attrName == "xyzt_file")
                xyzt_file = values;
            else if (attrName == "output_file")
                output_file = values;
            else
                throw std::invalid_argument("Invalid attribute name for string vector");
        }
        else if constexpr (std::is_same<T, locData>::value)
        {
            if (attrName == "locData")
                v_locData = values;
            else
                throw std::invalid_argument("Invalid attribute name for locData vector: " + attrName);
        }
        else
        {
            throw std::invalid_argument("Unsupported data type");
        }
    }

    
    // Explicit specialization for locData
    template <>
    void taskData::setAttr<locData>(const std::string &attrName, const std::vector<locData> &values)
    {
        if (attrName == "locData")
        {
            v_locData = values;
        }
        else
        {
            throw std::invalid_argument(" locData vector");
        }
    }

    // Method to get attribute by name and return as vector
    template <typename T>
    std::vector<T> taskData::getAttr(const std::string &attrName) const
    {
        if (attrName == "id")
            return std::vector<T>(id.begin(), id.end());
        if (attrName == "sig_x0")
            return std::vector<T>(sig_x0.begin(), sig_x0.end());
        if (attrName == "sig_y0")
            return std::vector<T>(sig_y0.begin(), sig_y0.end());
        if (attrName == "sig_z0")
            return std::vector<T>(sig_z0.begin(), sig_z0.end());
        if (attrName == "istab")
            return std::vector<T>(istab.begin(), istab.end());
        if (attrName == "wind")
            return std::vector<T>(wind.begin(), wind.end());
        if (attrName == "mass")
            return std::vector<T>(mass.begin(), mass.end());
        if (attrName == "decay")
            return std::vector<T>(decay.begin(), decay.end());
        if (attrName == "vd")
            return std::vector<T>(vd.begin(), vd.end());
        if (attrName == "hml")
            return std::vector<T>(hml.begin(), hml.end());
        if (attrName == "zplume")
            return std::vector<T>(zplume.begin(), zplume.end());
        if (attrName == "xv")
            return std::vector<T>(xv.begin(), xv.end());
        if (attrName == "yv")
            return std::vector<T>(yv.begin(), yv.end());
        if (attrName == "zv")
            return std::vector<T>(zv.begin(), zv.end());
        if (attrName == "dur")
            return std::vector<T>(dur.begin(), dur.end());
        throw std::invalid_argument("Attribute name not recognized for retrieval");
    }

   
    // Print a specific attribute by name
    template <typename T>
    void taskData::printAttr(const std::string &attrName) const
    {
        std::vector<T> values = getAttr<T>(attrName);
        std::cout << attrName << ": ";
        for (const auto &val : values)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    
    // Print all data in the class
    void taskData::printData() const
    {
        std::cout << "Printing all data for taskData class:" << std::endl;
        printAttr<int>("id");
        printAttr<double>("sig_x0");
        printAttr<double>("sig_y0");
        printAttr<double>("sig_z0");
        printAttr<int>("istab");
        printAttr<double>("wind");
        printAttr<double>("mass");
        printAttr<double>("decay");
        printAttr<double>("vd");
        printAttr<double>("hml");
        printAttr<double>("zplume");
        printAttr<double>("xv");
        printAttr<double>("yv");
        printAttr<double>("zv");
        printAttr<double>("dur");
        std::cout << "File paths for XYZT files:" << std::endl;
        for (const auto &file : xyzt_file)
            std::cout << file << " ";
        std::cout << "\nOutput files:" << std::endl;
        for (const auto &file : output_file)
            std::cout << file << " ";
        std::cout << std::endl;

        // Print each locData instance in v_locData
        std::cout << "\nLocData for each task:" << std::endl;
        for (size_t i = 0; i < v_locData.size(); ++i)
        {
            std::cout << "Task " << i << " LocData:" << std::endl;
            v_locData[i].printData();
        }
        std::cout << std::endl;
    }

    

    taskDataRow taskData::getRow(int index) const
    {
        if (index < 0 || index >= id.size())
        {
            throw std::out_of_range("Index out of range in getRow");
        }

        taskDataRow row;
        row.id = id[index];
        row.sig_x0 = sig_x0[index];
        row.sig_y0 = sig_y0[index];
        row.sig_z0 = sig_z0[index];
        row.istab = istab[index];
        row.wind = wind[index];
        row.mass = mass[index];
        row.decay = decay[index];
        row.vd = vd[index];
        row.hml = hml[index];
        row.zplume = zplume[index];
        row.xv = xv[index];
        row.yv = yv[index];
        row.zv = zv[index];
        row.dur = dur[index];
        row.xyzt_file = xyzt_file[index];
        row.output_file = output_file[index];

        // Populate m_locData by converting each element in v_locData[index] to locDataRow
        const locData &loc_data = v_locData[index];
        row.locations = loc_data.toLocDataRows(); // Convert locData to vector of locDataRow

        return row;
    }

    
    void taskData::setRow(int index, const taskDataRow &row)
    {
        if (index < 0 || index >= id.size())
        {
            throw std::out_of_range("Index out of range in setRow");
        }

        id[index] = row.id;
        sig_x0[index] = row.sig_x0;
        sig_y0[index] = row.sig_y0;
        sig_z0[index] = row.sig_z0;
        istab[index] = row.istab;
        wind[index] = row.wind;
        mass[index] = row.mass;
        decay[index] = row.decay;
        vd[index] = row.vd;
        hml[index] = row.hml;
        zplume[index] = row.zplume;
        xv[index] = row.xv;
        yv[index] = row.yv;
        zv[index] = row.zv;
        dur[index] = row.dur;
        xyzt_file[index] = row.xyzt_file;
        output_file[index] = row.output_file;

        // Convert the vector of locDataRow in taskDataRow back to locData
        locData loc_data;
        for (const auto &loc_row : row.locations)
        {
            loc_data.iloc.push_back(loc_row.iloc);
            loc_data.x.push_back(loc_row.x);
            loc_data.y.push_back(loc_row.y);
            loc_data.z.push_back(loc_row.z);
            loc_data.t.push_back(loc_row.t);
            loc_data.sig_x.push_back(loc_row.sig_x);
            loc_data.sig_y.push_back(loc_row.sig_y);
            loc_data.sig_z.push_back(loc_row.sig_z);
            loc_data.concentration.push_back(loc_row.concentration);
            loc_data.dosage.push_back(loc_row.dosage);
            loc_data.cpeak.push_back(loc_row.cpeak);
            loc_data.dinf.push_back(loc_row.dinf);
            loc_data.xfunc.push_back(loc_row.xfunc);
            loc_data.xfuncp.push_back(loc_row.xfuncp);
            loc_data.yfunc.push_back(loc_row.yfunc);
            loc_data.zfunc.push_back(loc_row.zfunc);
            loc_data.qyz.push_back(loc_row.qyz);
            loc_data.ctip.push_back(loc_row.ctip);
            loc_data.ctail.push_back(loc_row.ctail);
        }

        v_locData[index] = loc_data; // Set the locData in the main data vector
    }

    int taskData::getTaskNum() const
    {
        return istab.size();
    }

   
} // namespace FastPlume

// Explicit template instantiations
template void FastPlume::taskData::setAttr<double>(const std::string &, const std::vector<double> &);
template void FastPlume::taskData::setAttr<int>(const std::string &, const std::vector<int> &);
template void FastPlume::taskData::setAttr<std::string>(const std::string &, const std::vector<std::string> &);
template void FastPlume::taskData::setAttr<FastPlume::locData>(const std::string &attrName, const std::vector<FastPlume::locData> &values);

