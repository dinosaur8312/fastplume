#include "taskEvapData.h"
#include "stringUtil.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>

namespace FastPlume
{

    taskEvapData::taskEvapData(const std::string &csvFilePath)
    {
        parseCSV(csvFilePath);
    }

    taskEvapData::~taskEvapData()
    {
        // Destructor logic if any
    }

    void taskEvapData::parseCSV(const std::string &csvFilePath)
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

        // Map column names to indices for easy lookup
        while (std::getline(headerStream, columnName, ','))
        {
            columnName = trim(columnName);
            //std::cout << "columnName: " << columnName << " index: " << index << std::endl;
            columnIndices[columnName] = index++;

        }

        // Check required columns
        const std::vector<std::string> requiredColumns = {"agent_fp",
                                                          "time",
                                                          "istab",
                                                          "surface_temperature",
                                                          "atmPressure",
                                                          "surface_type"};
        for (const auto &col : requiredColumns)
        {
            if (columnIndices.find(col) == columnIndices.end())
            {
                throw std::runtime_error("Missing required column in CSV: " + col);
            }
        }

        // Read data lines
        while (std::getline(file, line))
        {

            line = removeCommasInsideQuotes(line);
           // std::cout << "line: " << line << std::endl;

            std::istringstream lineStream(line);
            std::string cell;
            int colIdx = 0;

            // Temporary variables to store value
            int idVal = 0;
            double qMgVal = 0.0;
            double surfaceTemperatureVal = 0.0;
            double AtmPressureVal = 0.0;
            std::string agent_fpVal;
            double timeVal = 0.0;
            int istabVal = 0;
            double windSpeedVal = 0.0;
            std::string surfaceTypeVal;

            // Parse each cell and assign to the corresponding attribute based on column index
            while (std::getline(lineStream, cell, ','))
            {
                //std::cout << "colIdx: " << colIdx << " cell: " << cell << std::endl;

                if (colIdx == columnIndices["id"])
                    idVal = std::stoi(cell);
                else if (colIdx == columnIndices["surface_temperature"])
                    surfaceTemperatureVal = std::stod(cell);
                else if (colIdx == columnIndices["atmPressure"])
                {
                    AtmPressureVal = std::stod(cell);
                    //std::cout << "AtmPressureVal: " << AtmPressureVal << std::endl;
                }
                else if (colIdx == columnIndices["windSpeed"] || colIdx == columnIndices["U"] || colIdx == columnIndices["wind"])
                    windSpeedVal = std::stod(cell);
                else if (colIdx == columnIndices["surface_type"])
                    surfaceTypeVal = cell;
                else if (colIdx == columnIndices["agent_fp"])
                    agent_fpVal = cell;
                else if (colIdx == columnIndices["Q_mg"] || colIdx == columnIndices["Q"])
                    qMgVal = std::stod(cell);
                else if (colIdx == columnIndices["Q_kg"])
                    qMgVal = std::stod(cell) * 1e6;
                else if (colIdx == columnIndices["time"])
                    timeVal = std::stod(cell);
                else if (colIdx == columnIndices["istab"])
                    istabVal = std::stoi(cell);

                colIdx++;
            }
            // Store the parsed values in class member vectors
            id.push_back(idVal);
            surfaceTemperature.push_back(surfaceTemperatureVal);
            AtmPressure.push_back(AtmPressureVal);
            windSpeed.push_back(windSpeedVal);
            surfaceType.push_back(surfaceTypeVal);
            agentName.push_back(agent_fpVal);
            time.push_back(timeVal);
            stabilityClass.push_back(istabVal);
            quantityRemaining.push_back(qMgVal);
        }

        file.close();

        id.resize(quantityRemaining.size()); // Resize id vector to the size of istab
        std::iota(id.begin(), id.end(), 0);

        printf("Parsed %d rows from %s\n", getTaskNum(), csvFilePath.c_str());
    }

    void taskEvapData::matchData()
    {
        size_t targetSize = getTaskNum();
        // Lambda to resize vectors in `taskData` to match `istab` size
        auto resizeToTarget = [targetSize](auto &vec)
        {
            if (vec.size() < targetSize)
                vec.resize(targetSize, typename std::decay_t<decltype(vec)>::value_type{});
        };

        // Apply resizing to all taskData vectors
        resizeToTarget(id);
        resizeToTarget(surfaceTemperature);
        resizeToTarget(AtmPressure);
        resizeToTarget(windSpeed);
        resizeToTarget(surfaceType);
        resizeToTarget(agentName);
        resizeToTarget(time);
        resizeToTarget(stabilityClass);
        resizeToTarget(quantityRemaining);
    }

    template <typename T>
    void taskEvapData::setAttr(const std::string &attrName, const std::vector<T> &values)
    {
        if constexpr (std::is_same<T, int>::value)
        {
            if (attrName == "id")
                id = values;
            else if (attrName == "istab")
                stabilityClass = values;
            else
                throw std::invalid_argument("Invalid attribute name for int vector");
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            if ((attrName == "surface_temperature") || (attrName == "surfaceTemperature"))
                surfaceTemperature = values;
            else if (attrName == "AtmPressure")
                AtmPressure = values;
            else if ((attrName == "windSpeed") || (attrName == "wind") || (attrName == "U"))
                windSpeed = values;
            else if (attrName == "time")
                time = values;
            else if ((attrName == "quantityRemaining") || (attrName == "Q_mg") || (attrName == "Q"))
                quantityRemaining = values;
            else
                throw std::invalid_argument("Invalid attribute name for double vector");
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            if (attrName == "surfaceType")
                surfaceType = values;
            else if ((attrName == "agent_fp") || (attrName == "agentName") || (attrName == "agent"))
                agentName = values;
            else
                throw std::invalid_argument("Invalid attribute name for string vector");
        }
        else
        {
            throw std::invalid_argument("Unsupported data type");
        }
    }

    template <typename T>
    std::vector<T> taskEvapData::getAttr(const std::string &attrName) const
    {
        if constexpr (std::is_same<T, int>::value)
        {
            if (attrName == "id")
                return std::vector<T>(id.begin(), id.end());
            if (attrName == "istab")
                return std::vector<T>(stabilityClass.begin(), stabilityClass.end());
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            if (attrName == "surface_temperature" || attrName == "surfaceTemperature")
                return std::vector<T>(surfaceTemperature.begin(), surfaceTemperature.end());
            if (attrName == "AtmPressure")
                return std::vector<T>(AtmPressure.begin(), AtmPressure.end());
            if (attrName == "time")
                return std::vector<T>(time.begin(), time.end());
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            if (attrName == "surfaceType")
                return std::vector<T>(surfaceType.begin(), surfaceType.end());
            if (attrName == "agent_fp" || attrName == "agentName" || attrName == "agent")
                return std::vector<T>(agentName.begin(), agentName.end());
        }
        else
        {
            throw std::invalid_argument("Unsupported type for getAttr.");
        }

        throw std::invalid_argument("Attribute name not recognized.");
    }

    // Print a specific attribute by name
    template <typename T>
    void taskEvapData::printAttr(const std::string &attrName) const
    {
        std::vector<T> values = getAttr<T>(attrName);
        std::cout << attrName << ": ";
        for (const auto &val : values)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::vector<taskEvapDataRow> taskEvapData::getAllTaskRows() const
    {
        std::vector<taskEvapDataRow> rows;
        for (size_t i = 0; i < getTaskNum(); ++i)
        {
            taskEvapDataRow row = getRow(i);
            rows.push_back(row);
        }
        return rows;
    }

    taskEvapDataRow taskEvapData::getRow(int index) const
    {
        if (index < 0 || index >= getTaskNum())
        {
            throw std::out_of_range("Index out of range in getRow");
        }

        taskEvapDataRow row;
        row.id = id[index];
      //  printf("id = %d\n", row.id);
        row.surfaceTemperature = surfaceTemperature[index];
     //   printf("surfaceTemperature = %f\n", row.surfaceTemperature);
        row.AtmPressure = AtmPressure[index];
      //  printf("AtmPressure = %f\n", row.AtmPressure);
        row.windSpeed = windSpeed[index];
       // printf("windSpeed = %f\n", row.windSpeed);
        row.surfaceType = surfaceType[index];
       // printf("surfaceType = %s\n", row.surfaceType.c_str());
        row.agentName = agentName[index];
       // printf("agentName = %s\n", row.agentName.c_str());
        row.time = time[index];
       // printf("time = %f\n", row.time);
        row.stabilityClass = stabilityClass[index];
      //  printf("stabilityClass = %d\n", row.stabilityClass);
        row.quantityRemaining = quantityRemaining[index];
        //printf("quantityRemaining = %f\n", row.quantityRemaining);

        return row;
    }

    int taskEvapData::getTaskNum() const
    {
        return time.size();
    }
} // namespace FastPlume

template void FastPlume::taskEvapData::setAttr<double>(const std::string &, const std::vector<double> &);
template void FastPlume::taskEvapData::setAttr<int>(const std::string &, const std::vector<int> &);
