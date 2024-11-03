#ifndef MATHUTIL_H
#define MATHUTIL_H
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

namespace FastPlume
{

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
        if ((hml < zrcp) && (zplume <= hml))
            return 0.;

        static constexpr double INV_ROOT2PI = 0.3989422804014327;
        float zrefl = 0.;
        if (zplume <= hml)
        {
            if (zrcp > hml)
                return 0.;
            if (sigz > hml)
                return 1. / hml;
            zplume = std::max(0.001, zplume);

            if (zplume <= hml)
                zplume = std::min(hml - 0.001, zplume);

            zrefl = zReflections<0>(zrcp, zplume, hml, sigz);
        }
        else
        {
            if (zrcp <= hml)
                return 0.;
            float arg = (zrcp - zplume) / sigz;

            zrefl = exp(-0.5 * arg * arg);

            arg = (zrcp + 2.0 * hml - zplume) / sigz;
            zrefl += exp(-0.5 * arg * arg);
        }

        return INV_ROOT2PI / sigz * zrefl;
    }

    inline double gaussFunction(double x)
    {

        return exp(-0.5 * x * x);
    }
    inline double pdfFunction(double y, double sigy = 1.)
    {
        static constexpr float INV_ROOT2PI = 0.3989422804014327;
        float arg = y / sigy;

        return INV_ROOT2PI * exp(-0.5 * arg * arg) / sigy;
    }

    inline double cdfFunction(double x, double scale = 1.)
    {
        static constexpr double INV_SQRT2 = 0.7071067811865475;
        return 0.5 * (1. + erf(INV_SQRT2 * (x / scale)));
    }

    inline double IcdfFunction(double x, double scale = 1.f)
    {
        double xs = x / scale;

        return xs * cdfFunction(x, scale) + pdfFunction(x / scale);
    }

    // Helper function to perform linear regression
    static void computeLinearRegression(const std::vector<float> &values, float &slope, float &intercept)
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

} // namespace FastPlume

#endif
