#ifndef TASKDATAROW_H
#define TASKDATAROW_H
#include <iostream>
#include <vector>
#include <string>
namespace FastPlume
{

    class locDataRow
    {
    public:
        int iloc;
        double x, y, z, t;
        double sig_x, sig_y, sig_z;
        double concentration, dosage, cpeak, dinf, xfunc, xfuncp, yfunc, zfunc, qyz, ctip, ctail;
    };

    class taskDataRow
    {
    public:
        int id;
        double sig_x0;
        double sig_y0;
        double sig_z0;
        int istab;
        double wind;
        double mass;
        double decay;
        double vd;
        double hml;
        double zplume;
        double xv;
        double yv;
        double zv;
        double dur;
        std::string xyzt_file;
        std::string output_file;
       // locData m_locData;
       std::vector<locDataRow> locations; // Store location data rows
    };

}
#endif // TASKDATAROW_H