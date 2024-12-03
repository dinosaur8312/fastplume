#ifndef TASKEVAPDATA_H
#define TASKEVAPDATA_H

#include <iostream>
#include <vector>
#include <string>
#include "taskEvapDataRow.h"

namespace FastPlume
{


    class taskEvapData
    {
        public:
        //default constructor
        taskEvapData() {}
        // Constructor to initialize from a CSV file path
        taskEvapData(const std::string &csvFilePath);

        // Destructor
        ~taskEvapData();

        // Template method to set attributes
        template <typename T>
        void setAttr(const std::string &attrName, const std::vector<T> &values);

        // Template method to get attributes
        template <typename T>
        std::vector<T> getAttr(const std::string &attrName) const;

        // Template method to print a specific attribute
        template <typename T>
        void printAttr(const std::string &attrName) const;

        // Method to print all data
        void printData() const;

        void matchData();

        taskEvapDataRow getRow(int index) const;
        void setRow(int index, const taskEvapDataRow &row);

        std::vector<taskEvapDataRow> getAllTaskRows() const;
        

        int getTaskNum() const;

        void parseCSV(const std::string &csvFilePath);

    private:
        std::vector <int> id;
        std::vector <double> surfaceTemperature;
        std::vector <double> AtmPressure;
        std::vector <double> windSpeed;
        std::vector <std::string> surfaceType;
        std::vector <std::string> agentName;
       // std::vector <double> puddleLength;
       // std::vector <double> puddleWidth;
        //std::vector <double> puddle_area;
        //std::vector <int> windDirection;
        //std::vector <double> lengthAlongWind;
        std::vector <double> quantityRemaining;
        //std::vector <double> vaporPressure;
        //std::vector <double> puddleLengthAlongWind;
        //std::vector <double> airDensity;
        //std::vector <double> airViscosity;
        //std::vector <double> schmidtNumberD;
        //std::vector <double> airDiffusivity;
        //std::vector <double> reynoldsNumber;
        //std::vector <double> massTransferFactor;
        //std::vector <double> molarMassVelocity;
        //std::vector <double> massTransferCoefficient;
        //std::vector <double> evaporationRate;
        //std::vector <double> tKOverE;
        //std::vector <double> collisionForDiffusion;
        //std::vector <double> collisionDiameter;
        //std::vector <double> agentDiffusivity;
        //std::vector <double> diameterOfSpill;
        //std::vector <double> reynoldsNumberSpill;
        //std::vector <double> evaporationRateSpill;
        //std::vector <double> releaseRate;
        std::vector <double> time;
        std::vector <int> stabilityClass;
        //std::vector <double> sig_x;
        //std::vector <double> sig_y;
        //std::vector <double> sig_z;
    };

} // namespace FastPlume

#endif // TASKEVAPDATA_H