#include "mathUtil.h"

namespace FastPlume
{

    float getSpeedFactor(int istab)
    {
        static const std::vector<float> speedFactors = {1.35, 1.47, 1.58, 1.7, 2.15, 2.6, 2.6};
        if (istab < 0 || istab >= speedFactors.size())
            return 1.0;
        return speedFactors[istab];
    }



    void computeLinearRegression(const std::vector<float> &values, float &slope, float &intercept)
    {
        size_t n = values.size();
        if (n < 2)
            return;

        float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
        for (size_t i = 0; i < n; ++i)
        {
            float x = static_cast<float>(i); // Use index as x value
            float y = values[i];
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
        }

        float denominator = n * sum_x2 - sum_x * sum_x;
        if (denominator == 0)
            return; // Prevent division by zero

        slope = (n * sum_xy - sum_x * sum_y) / denominator;
        intercept = (sum_y - slope * sum_x) / n;
    }

    void CalcPuddleDimensions(double quantity, const std::string surface_type,
                              double &m_dPuddleArea, double &m_dPuddleLength, double &m_dPuddleWidth)
    {

        if (quantity <= 0.001)
        {
            m_dPuddleArea = 1.0e-4;
        }
        else if (surface_type == "SUR_POROUS")
        {
            m_dPuddleArea = 0.153e-6 * quantity;
        }
        else if (surface_type == "SUR_NONPOROUS")
        {
            m_dPuddleArea = 1.21e-6 * quantity;
        }
        else
        {
            m_dPuddleWidth = m_dPuddleArea / m_dPuddleLength;
            return;
        }

        m_dPuddleLength = pow(m_dPuddleArea, 0.5);
        m_dPuddleWidth = m_dPuddleLength;

        return;
    } // end CalcPuddleDimensions
}