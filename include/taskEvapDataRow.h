#ifndef TASKEVAPDATAROW_H
#define TASKEVAPDATAROW_H
#include <iostream>
#include <vector>
#include <string>
namespace FastPlume
{
    class taskEvapDataRow
    {
    public:
        int id;
        double surfaceTemperature;
        double AtmPressure;
        double windSpeed;
        std::string surfaceType;
        std::string agentName;
        double puddleLength;
        double puddleWidth;
        double puddle_area;
        int windDirection;
        double lengthAlongWind;
        double quantityRemaining;
        double vaporPressure;
        double puddleLengthAlongWind;
        double airDensity;
        double airViscosity;
        double schmidtNumberD;
        double airDiffusivity;
        double reynoldsNumber;
        double massTransferFactor;
        double molarMassVelocity;
        double massTransferCoefficient;
        double evaporationRate;
        double tKOverE;
        double collisionForDiffusion;
        double collisionDiameter;
        double agentDiffusivity;
        double diameterOfSpill;
        double reynoldsNumberSpill;
        double evaporationRateSpill;
        double releaseRate;
        double time;
        int stabilityClass;
        double sig_x;
        double sig_y;
        double sig_z;
    };

} // namespace FastPlume
#endif // TASKEVAPDATAROW_H