#include "SigmaInterp.hpp"
#define EPSILON 0.0001
// #define PRTCHECK

void findFourCoefs(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                   int &id0, int &id1, int &id2, int &id3, bool &flag)
{
    id0 = -1;
    id1 = -1;
    id2 = -1;
    id3 = -1;
    flag = false;

    std::vector<std::vector<double>> windgap_min(2, std::vector<double>(2, 9999999.f));
    std::vector<std::vector<double>> xgap_min(2, std::vector<double>(2, 9999999.f));

    for (int j = 0; j < coefs.size(); j++)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;
        double x_coef = row_coef.x;

        double windgap = abs(wind_coef - wind);
        double xgap = abs(x_coef - x);

        if (wind_coef <= wind)
        {
            if (windgap <= (windgap_min[0][0] + EPSILON))
            {
                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[0][0])
                    {
                        xgap_min[0][0] = xgap;
                        windgap_min[0][0] = windgap;
                        id0 = j;
                    }
                }
            }

            if (windgap <= (windgap_min[0][1] + EPSILON))
            {
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[0][1])
                    {
                        xgap_min[0][1] = xgap;
                        windgap_min[0][1] = windgap;
                        id1 = j;
                    }
                }
            }
        }
        else
        {
            if (windgap <= (windgap_min[1][0] + EPSILON))
            {

                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[1][0])
                    {
                        xgap_min[1][0] = xgap;
                        windgap_min[1][0] = windgap;
                        id2 = j;
                    }
                }
            }

            if (windgap < (windgap_min[1][1] + EPSILON))
            {
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[1][1])
                    {
                        xgap_min[1][1] = xgap;
                        windgap_min[1][1] = windgap;
                        id3 = j;
                    }
                }
            }
        }

        if ((id1 >= 0) && (id2 >= 0) && (id3 >= 0) && (id0 >= 0))
            break;

    } // end of loop over all rows in coefs to find id0, id1, id2, id3

    if ((id0 >= 0) && (id1 == -1))
        id1 = id0;
    if ((id2 >= 0) && (id3 == -1))
        id3 = id2;
    if ((id1 >= 0) && (id0 == -1))
        id0 = id1;
    if ((id3 >= 0) && (id2 == -1))
        id2 = id3;

    if ((id1 >= 0) && (id3 == -1))
    {
        id2 = id0;
        id3 = id1;
    }

    if ((id3 >= 0) && (id1 == -1))
    {
        id1 = id3;
        id0 = id2;
    }
}

void findFourCoefs_sig(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                       int &id0, int &id1, int &id2, int &id3, bool &flag, const int sigid)
{
    id0 = -1;
    id1 = -1;
    id2 = -1;
    id3 = -1;
    flag = false;

    std::vector<std::vector<double>> windgap_min(2, std::vector<double>(2, 9999999.f));
    std::vector<std::vector<double>> xgap_min(2, std::vector<double>(2, 9999999.f));

    printf("in findFourCoefs_sig\n");
    printf("istab=%d\n", istab);
    printf("wind=%f\n", wind);
    int jminwind = -1;
    int jmaxwind = -1;
    for (int j = 0; j < coefs.size(); j++)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;

        double x_coef;
        if (sigid == 0)
            x_coef = row_coef.sig_x;
        else if (sigid == 1)
            x_coef = row_coef.sig_y;
        else if (sigid == 2)
            x_coef = row_coef.sig_z;

       // printf("j=%d\n", j);
       // printf("wind_coef=%f\n", wind_coef);

        double windgap = abs(wind_coef - wind);
       // printf("windgap=%f\n", windgap);
        if (wind_coef <= wind)
        {
            if (windgap < windgap_min[0][0])
            {
                jminwind = j;
                windgap_min[0][0] = windgap;
            }
        }
        
    }
    for (int j = coefs.size()-1; j >=0; j--)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;

        double x_coef;
        if (sigid == 0)
            x_coef = row_coef.sig_x;
        else if (sigid == 1)
            x_coef = row_coef.sig_y;
        else if (sigid == 2)
            x_coef = row_coef.sig_z;

  //      printf("j=%d\n", j);
//        printf("wind_coef=%f\n", wind_coef);

        double windgap = abs(wind_coef - wind);
        //printf("windgap=%f\n", windgap);
        
        if (wind_coef >= wind)
        {
           // printf("else windgap_min01=%f\n", windgap_min[0][1]);
            if (windgap < windgap_min[0][1])
            {
                jmaxwind = j;
                windgap_min[0][1] = windgap;
            }
        }
    }
    printf("jminwind=%d\n", jminwind);
    printf("jmaxwind=%d\n", jmaxwind);

    for (int j = jminwind; j < jmaxwind; j++)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;

        double x_coef;
        if (sigid == 0)
            x_coef = row_coef.sig_x;
        else if (sigid == 1)
            x_coef = row_coef.sig_y;
        else if (sigid == 2)
            x_coef = row_coef.sig_z;

        double windgap = abs(wind_coef - wind);
        double xgap = abs(x_coef - x);

        /*
            if ((sigid == 0) && (j > 180) && (j < 185))
            {
                printf("\nj=%d\n", j);
                printf("sig=%f\n", x);

                printf("wind_coef=%f\n", wind_coef);
                printf("sig_coef=%f\n", x_coef);
                printf("windgap=%f\n", windgap);
                printf("siggap=%f\n", xgap);
            }
    */
        printf("j=%d, x_coef=%f, x=%f, xgap=%f\n", j, x_coef, x, xgap);
        if (wind_coef <= wind)
        {
            //          if ((sigid == 0) && (j > 180) && (j < 185))
            //        {
            //          printf("flag wind0\n");
            //        printf("windgap_min00=%f\n", windgap_min[0][0]);
            //  }
            if (windgap <= (windgap_min[0][0] + EPSILON))
            {
                //if ((sigid == 0) && (j > 180) && (j < 185))
                //{
                    //     printf("thresh wind0\n");
                    // printf("xgap=%f,xgap_min00=%f\n", xgap, xgap_min[0][0]);
                    // printf("x_coef=%f,x=%f\n", x_coef, x);
                //}
                if (x_coef <= x)
                {
                    // printf("x_coef<=x\n");
                    if (xgap <= (xgap_min[0][0] + EPSILON))
                    {
                        xgap_min[0][0] = xgap;
                        windgap_min[0][0] = windgap;
                        id0 = j;
                        printf("xgap_min00=%f,windgap_min00=%f,id0=%d\n", xgap, windgap, j);
                        // if ((sigid == 0) && (j > 180) && (j < 185))
                        //   printf("XXXXXXXXX FOUND id0\n");
                    }
                }
            }

            if (windgap <= (windgap_min[0][1] + EPSILON))
            {
                if ((sigid == 0) && (j > 180) && (j < 185))
                {
                    //  printf("thresh wind1\n");
                }
                if (x_coef > x)
                {
                    if (xgap <= (xgap_min[0][1] + EPSILON))
                    {
                        xgap_min[0][1] = xgap;
                        windgap_min[0][1] = windgap;
                        id1 = j;

                        // printf("XXXXXXXXX FOUND id1\n");
                         printf("xgap_min01=%f,windgap_min01=%f,id1=%d\n",xgap,windgap,j);
                    }
                }
            }
        }
        else
        {
            // if ((sigid == 0) && (j > 180) && (j < 185))
            //{
            //  printf("flag wind1\n");
            //}
            if (windgap <= (windgap_min[1][0] + EPSILON))
            {

                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[1][0])
                    {
                        xgap_min[1][0] = xgap;
                        windgap_min[1][0] = windgap;
                        id2 = j;
                    }
                }
            }

            if (windgap < (windgap_min[1][1] + EPSILON))
            {
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[1][1])
                    {
                        xgap_min[1][1] = xgap;
                        windgap_min[1][1] = windgap;
                        id3 = j;
                    }
                }
            }
        }

        // if ((id1 >= 0) && (id2 >= 0) && (id3 >= 0) && (id0 >= 0))
        //   break;

    } // end of loop over all rows in coefs to find id0, id1, id2, id3

    printf("raw:id0=%d,id1=%d,id2=%d,id3=%d\n", id0, id1, id2, id3);

    if ((id0 >= 0) && (id1 == -1))
        id1 = id0;
    if ((id2 >= 0) && (id3 == -1))
        id3 = id2;
    if ((id1 >= 0) && (id0 == -1))
        id0 = id1;
    if ((id3 >= 0) && (id2 == -1))
        id2 = id3;

    if ((id1 >= 0) && (id3 == -1))
    {
        id2 = id0;
        id3 = id1;
    }

    if ((id3 >= 0) && (id1 == -1))
    {
        id1 = id3;
        id0 = id2;
    }

    if (id1 < id0)
        id1 = id0 + 1;
    if (id3 < id2)
        id3 = id2 + 1;
    printf("final:id0=%d,id1=%d,id2=%d,id3=%d\n", id0, id1, id2, id3);
}

void compareSigmaData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float x, CSVDataRow row,
                      float &sig_x, float &exp_log_sig_x_coef, float &error_logx,
                      const int sigid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double sig_x0, sig_x1, sig_x2, sig_x3;

    if (sigid == 0)
    {
        sig_x0 = row0.sig_x;
        sig_x1 = row1.sig_x;
        sig_x2 = row2.sig_x;
        sig_x3 = row3.sig_x;
    }
    else if (sigid == 1)
    {
        sig_x0 = row0.sig_y;
        sig_x1 = row1.sig_y;
        sig_x2 = row2.sig_y;
        sig_x3 = row3.sig_y;
    }
    else if (sigid == 2)
    {
        sig_x0 = row0.sig_z;
        sig_x1 = row1.sig_z;
        sig_x2 = row2.sig_z;
        sig_x3 = row3.sig_z;
    }

    double log_sig_x0 = log(sig_x0);
    double log_sig_x1 = log(sig_x1);
    double log_sig_x2 = log(sig_x2);
    double log_sig_x3 = log(sig_x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    double log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    exp_log_sig_x_coef = exp(log_sig_x_coef);

#if (PRTCHECK)
    printf("xk_flag, compareSigmaData,  x0 = %f, x1 = %f, x2 = %f, x3 = %f\n", x0, x1, x2, x3);
    printf("xk_flag, compareSigmaData,   logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx0, logx1, logx2, logx3);
    printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    printf("xk_flag, compareSigmaData,  log_sig_x0 = %f, log_sig_x1 = %f, log_sig_x2 = %f, log_sig_x3 = %f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    printf("xk_flag, compareSigmaData,  logx = %f, wx01=%f, wx23=%f\n", logx, wx01, wx23);
    printf("xk_flag, compareSigmaData,  log_sig_x01 = %f, log_sig_x23 = %f\n", log_sig_x01, log_sig_x23);
    printf("xk_flag, compareSigmaData,  w_wind = %f\n", w_wind);
    printf("xk_flag, compareSigmaData,  log_sig_x_coef = %f\n", log_sig_x_coef);
    printf("xk_flag, compareSigmaData,  exp_log_sig_x_coef = %f\n", exp_log_sig_x_coef);
#endif

    if (sigid == 0)
        sig_x = row.sig_x;
    else if (sigid == 1)
        sig_x = row.sig_y;
    else if (sigid == 2)
        sig_x = row.sig_z;

    error_logx = abs(sig_x - exp_log_sig_x_coef) / abs(sig_x) * 100;
}

void compareZfunction(std::vector<CSVDataRow> data, std::ofstream &outputFile)
{

    outputFile << "ID,iplot,zrcp,zplume,hml,sig_z,zFunc_calc,zFunc_ref,error,pass\n";
    int pass_count = 0;
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        int iplot = row.istab;
        double zFunc = row.wind;
        double zrcp = row.x;
        double zplume = row.y;
        double hml = row.z;
        double sig_z = row.sig_z;

        float zfunc_calc = zFunction(zrcp, zplume, hml, sig_z);

        float error_zfunc = abs(zFunc) > 1E-6 ? abs(zFunc - zfunc_calc) / abs(zFunc) * 100 : abs(zFunc - zfunc_calc);

        outputFile << i << "," << iplot << "," << zrcp << "," << zplume << "," << hml << "," << sig_z << "," << zfunc_calc << "," << zFunc << "," << error_zfunc << ",";

        if (error_zfunc < 0.1f)
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
    }
    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}
void compareXData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                  const float sig_x, CSVDataRow row,
                  float &x, float &x_coef, float &error_x,
                  const int sigid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.sig_x;
    x1 = row1.sig_x;
    x2 = row2.sig_x;
    x3 = row3.sig_x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double sig_x0, sig_x1, sig_x2, sig_x3;

    if (sigid == 0)
    {
        sig_x0 = row0.x;
        sig_x1 = row1.x;
        sig_x2 = row2.x;
        sig_x3 = row3.x;
    }
    else if (sigid == 1)
    {
        sig_x0 = row0.y;
        sig_x1 = row1.y;
        sig_x2 = row2.y;
        sig_x3 = row3.y;
    }
    else if (sigid == 2)
    {
        sig_x0 = row0.z;
        sig_x1 = row1.z;
        sig_x2 = row2.z;
        sig_x3 = row3.z;
    }

    double log_sig_x0 = log(sig_x0);
    double log_sig_x1 = log(sig_x1);
    double log_sig_x2 = log(sig_x2);
    double log_sig_x3 = log(sig_x3);

    double logx = log(sig_x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    double log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    x_coef = exp(log_sig_x_coef);

#if (PRTCHECK)
    printf("xk_flag, compareSigmaData,  x0 = %f, x1 = %f, x2 = %f, x3 = %f\n", x0, x1, x2, x3);
    printf("xk_flag, compareSigmaData,   logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx0, logx1, logx2, logx3);
    printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    printf("xk_flag, compareSigmaData,  log_sig_x0 = %f, log_sig_x1 = %f, log_sig_x2 = %f, log_sig_x3 = %f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    printf("xk_flag, compareSigmaData,  logx = %f, wx01=%f, wx23=%f\n", logx, wx01, wx23);
    printf("xk_flag, compareSigmaData,  log_sig_x01 = %f, log_sig_x23 = %f\n", log_sig_x01, log_sig_x23);
    printf("xk_flag, compareSigmaData,  w_wind = %f\n", w_wind);
    printf("xk_flag, compareSigmaData,  log_sig_x_coef = %f\n", log_sig_x_coef);
    printf("xk_flag, compareSigmaData,  x_coef = %f\n", x_coef);
#endif

    if (sigid == 0)
        x = row.x;
    else if (sigid == 1)
        x = row.y;
    else if (sigid == 2)
        x = row.z;

    error_x = abs(x - x_coef) / abs(x) * 100;
}

void calcData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x,
              CSVDataRow &row)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x_coef;

    sig_x0 = row0.sig_x;
    sig_x1 = row1.sig_x;
    sig_x2 = row2.sig_x;
    sig_x3 = row3.sig_x;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    row.sig_x = exp(log_sig_x_coef);

    sig_x0 = row0.sig_y;
    sig_x1 = row1.sig_y;
    sig_x2 = row2.sig_y;
    sig_x3 = row3.sig_y;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    row.sig_y = exp(log_sig_x_coef);

    sig_x0 = row0.sig_z;
    sig_x1 = row1.sig_z;
    sig_x2 = row2.sig_z;
    sig_x3 = row3.sig_z;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    row.sig_z = exp(log_sig_x_coef);
}

double calcSigz(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x, double wind)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double w_wind = id0 == id2 ? 0.f : (wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x_coef;

    sig_x0 = row0.sig_z;
    sig_x1 = row1.sig_z;
    sig_x2 = row2.sig_z;
    sig_x3 = row3.sig_z;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    return exp(log_sig_x_coef);
}

void calcSigma(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x,
               CSVDataRow &row, const int comp)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    //printf("x0=%f,x1=%f,x2=%f,x3=%f\n", x0, x1, x2, x3);
    //printf("wx01=%f\n", wx01);
    //printf("wx23=%f\n", wx23);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x02, log_sig_x13;
    double log_sig_x_coef;

    if (comp == 0)
    {
        sig_x0 = row0.sig_x;
        sig_x1 = row1.sig_x;
        sig_x2 = row2.sig_x;
        sig_x3 = row3.sig_x;
    }
    else if (comp == 1)
    {
        sig_x0 = row0.sig_y;
        sig_x1 = row1.sig_y;
        sig_x2 = row2.sig_y;
        sig_x3 = row3.sig_y;
        //printf("sx0=%f,sx1=%f,sx2=%f,sx3=%f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    }
    else if (comp == 2)
    {
        sig_x0 = row0.sig_z;
        sig_x1 = row1.sig_z;
        sig_x2 = row2.sig_z;
        sig_x3 = row3.sig_z;
    }

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    // log_sig_02 = (1.f- w_wind) * log_sig_x0 + w_wind * log_sig_x2;
    // log_sig_13 = (1.f- w_wind) * log_sig_x1 + w_wind * log_sig_x3;

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;
    // printf("log_sig_x0=%f, log_sig_x1=%f,log_sig_x2=%f,log_sig_x3=%f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    // printf("log_sig_x01=%f, log_sig_x23=%f\n", log_sig_x01, log_sig_x23);

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    if (comp == 0)
        row.sig_x = exp(log_sig_x_coef);
    if (comp == 1)
    {
        row.sig_y = exp(log_sig_x_coef);
        //printf("sig_y=%f\n", row.sig_y);
    }
    if (comp == 2)
        row.sig_z = exp(log_sig_x_coef);
}
void calcData_virtual(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x,
                      CSVDataRow &row, const int xid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    if (xid == 0)
    {
        x0 = row0.sig_x;
        x1 = row1.sig_x;
        x2 = row2.sig_x;
        x3 = row3.sig_x;
    }
    if (xid == 1)
    {
        x0 = row0.sig_y;
        x1 = row1.sig_y;
        x2 = row2.sig_y;
        x3 = row3.sig_y;
    }
    if (xid == 2)
    {
        x0 = row0.sig_z;
        x1 = row1.sig_z;
        x2 = row2.sig_z;
        x3 = row3.sig_z;
    }
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x_coef;

    sig_x0 = row0.x;
    sig_x1 = row1.x;
    sig_x2 = row2.x;
    sig_x3 = row3.x;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    if (xid == 0)
        row.xv = exp(log_sig_x_coef);
    else if (xid == 1)
        row.yv = exp(log_sig_x_coef);
    else if (xid == 2)
        row.zv = exp(log_sig_x_coef);
}
void calcData_virtual_new(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double sig,
                          CSVDataRow &row, const int xid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    double sig_x0, sig_x1, sig_x2, sig_x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    if (xid == 0)
    {
        sig_x0 = row0.sig_x;
        sig_x1 = row1.sig_x;
        sig_x2 = row2.sig_x;
        sig_x3 = row3.sig_x;
    }
    if (xid == 1)
    {
        sig_x0 = row0.sig_y;
        sig_x1 = row1.sig_y;
        sig_x2 = row2.sig_y;
        sig_x3 = row3.sig_y;
    }
    if (xid == 2)
    {
        sig_x0 = row0.sig_z;
        sig_x1 = row1.sig_z;
        sig_x2 = row2.sig_z;
        sig_x3 = row3.sig_z;
    }
    printf("x0=%f,x1=%f,x2=%f,x3=%f\n", x0, x1, x2, x3);
    printf("sx0=%f,sx1=%f,sx2=%f,sx3=%f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);
    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);
    printf("log_sig_x0=%f, log_sig_x1=%f,log_sig_x2=%f,log_sig_x3=%f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    printf("w_wind=%f\n", w_wind);

    double log_sig_x02, log_sig_x13;
    log_sig_x02 = (1.f - w_wind) * log_sig_x0 + w_wind * log_sig_x2;
    log_sig_x13 = (1.f - w_wind) * log_sig_x1 + w_wind * log_sig_x3;
    printf("log_sig_x02=%f, log_sig_x13=%f\n", log_sig_x02, log_sig_x13);

    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    printf("logx0=%f, logx1=%f,logx2=%f,logx3=%f\n", logx0, logx1, logx2, logx3);

    double log_sig = log(sig);
    printf("log_sig=%f\n", log_sig);

    // double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    // double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double w_sig = id0 == id1 ? 0.f : (log_sig - log_sig_x02) / (log_sig_x13 - log_sig_x02);
    printf("w_sig=%f\n", w_sig);

    double log_x_coef = (1. - w_sig) * logx0 + w_sig * logx1;
    printf("log_x_coef=%f\n", log_x_coef);

    if (xid == 0)
        row.xv = exp(log_x_coef);
    else if (xid == 1)
        row.yv = exp(log_x_coef);
    else if (xid == 2)
        row.zv = exp(log_x_coef);
}

inline double normcdf(double x)
{
    static constexpr double INV_SQRT2 = 0.7071067811865475;
    return 0.5 * (1. + erf(INV_SQRT2 * x));
}

inline double normpdf(double x)
{
    static constexpr double INV_SQRT2PI = 0.3989422804014327;
    return INV_SQRT2PI * exp(-0.5f * x * x);
}

double centerlineDosage(
    double x, double t, double Q, double U, double zFunc, double sigX, double sigY)
{
    if (t < 0.f)
    {
        return 0.f;
    }
    static constexpr float INV_ROOT2PI = 0.3989422804014327;
    float coef = INV_ROOT2PI / (sigY * U) * zFunc;
    return Q * coef * normcdf((U * t - x) / sigX) - normcdf(-x / sigX);
}

double centerlineConcentration(
    double x, double t, double Q, double U, double zFunc, double sigX, double sigY)
{
    if (t < 0.f)
    {
        return 0.f;
    }
    static constexpr float INV_ROOT2PI = 0.3989422804014327;
    float coef = INV_ROOT2PI / (sigY * sigX) * zFunc;
    return Q * coef * normpdf((U * t - x) / sigX);
}

void calDosage(CSVDataRow &row)
{

    double zfunc = zFunction(row.zrcp, row.zplume, row.hml, row.sig_z);
    double yfunc = pdfFunction(row.y, row.sig_y);

    double qyz = row.mass * yfunc * zfunc;
    double xfunc, concentration, cpeak, xfuncp;
    double tip, tail, constant, tip1, tip0;
    double dosage;
    double dinf;

    /*
    if(row.xv>0)
    {
        row.x += row.xv;
    }
*/
    if (row.t >= 0)
    {
        if (row.dur < 1)
        {
            xfunc = pdfFunction(row.wind * row.t - row.x, row.sig_x);
            concentration = xfunc * qyz;
            xfuncp = pdfFunction(std::min(row.x, 0.), row.sig_x);
            cpeak = xfuncp * qyz;
            tail = 0;
            tip = 0;

            double arg0 = -row.x;
            double arg1 = row.wind * row.t - row.x;
            xfunc = (cdfFunction(arg1, row.sig_x) - cdfFunction(arg0, row.sig_x)) / row.wind;
            dosage = xfunc * qyz;
            double xfuncinf = (1.f - cdfFunction(arg0, row.sig_x)) / row.wind;
            dinf = xfuncinf * qyz;
        }
        else
        {
            // Calculate the tip
            double tip_time = row.t; // Adjust according to Python's tpeak function logic if necessary
            tip = cdfFunction(row.wind * tip_time - row.x, row.sig_x);

            // Calculate the tail
            double ut = std::max(0., row.t - row.dur) * row.wind; // corrected for the release duration
            tail = cdfFunction(ut - row.x, row.sig_x);

            row.ctip = tip;
            row.ctail = tail;

            // Calculate concentration
            concentration = (tip - tail) * qyz / row.wind / row.dur;

            double tp = std::max(row.dur, 0.5 * row.dur + row.x / row.wind);

            // Calculate peak concentration
            double tipp = cdfFunction(row.wind * tp - row.x, row.sig_x);
            double utp = std::max(0., tp - row.dur) * row.wind;
            double tailp = cdfFunction(utp - row.x, row.sig_x);
            cpeak = (tipp - tailp) * qyz / row.wind / row.dur;

            tip1 = IcdfFunction(row.wind * row.t - row.x, row.sig_x);
            tip0 = IcdfFunction(-row.x, row.sig_x);
            tip = tip1 - tip0;

            // For t<=T, tail is attached to the source
            double tail0 = cdfFunction(-row.x, row.sig_x) * std::min(row.t, row.dur) * row.wind / row.sig_x;

            // For t>=T, tail is moving
            double arg = row.wind * std::max(0., row.t - row.dur);
            double tail2 = IcdfFunction(arg - row.x, row.sig_x);
            double tail1 = IcdfFunction(-row.x, row.sig_x);
            tail = tail2 - tail1 + tail0;

            constant = qyz * row.sig_x / (row.dur * (row.wind * row.wind));
            dosage = (tip - tail) * constant;
            dinf = qyz / row.wind * cdfFunction(row.x, row.sig_x);
        }
    }

    row.concentration = concentration;
    row.cpeak = cpeak;
    row.dosage = dosage;
    row.dinf = dinf;
    row.xfunc = xfunc;
    row.xfuncp = xfuncp;
    row.zfunc = zfunc;
    row.yfunc = yfunc;
    row.qyz = qyz;

    /*
    if(row.xv>0)
    {
        row.x -= row.xv;
    }
    */
}

void generateSample(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.x;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    outputFile << "ID,istab,wind,x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, cpeak, concentration, dinf, dose\n";

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        double x = row.x;
        double y, z;
        int istab = row.istab;
        double wind = row.wind;

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
        printf("\ni=%d\n", i);
        printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
        if (x < xmin)
            x = xmin;
        if (x > xmax)
            x = xmax;
        // double logx = log(x);
        // double logy, logz;

        int id0, id1, id2, id3;
        bool flag = false;
        findFourCoefs(coefs, istab, wind, x, id0, id1, id2, id3, flag);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif

        calcData(coefs, id0, id1, id2, id3, x, row);

        calDosage(row);

        double defaultnan = -999.;
        outputFile << i << "," << istab << "," << wind << ",";
        outputFile << row.x << "," << row.y << "," << row.z << ",";
        outputFile << row.icurve << "," << row.t << ",";
        outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
        outputFile << row.zfunc << "," << defaultnan << "," << row.concentration << "," << defaultnan << "," << row.dosage << "\n";

    } // end of loop over all rows in data
    outputFile.close();
    std::cout << "CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}
void generateSourceSigma(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double ymin = 9999999.f;
    double ymax = -9999999.f;
    double zmin = 9999999.f;
    double zmax = -9999999.f;

    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (y < ymin)
            ymin = y;
        if (y > ymax)
            ymax = y;
        if (z < zmin)
            zmin = z;
        if (z > zmax)
            zmax = z;

        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    outputFile << "ID,istab,wind,x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, cpeak, concentration, dinf, dose\n";

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        int istab = row.istab;
        double wind = row.wind;

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
        printf("\ni=%d\n", i);
        printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
        if (x < xmin)
            x = xmin;
        if (x > xmax)
            x = xmax;
        if (y < ymin)
            y = ymin;
        if (y > ymax)
            y = ymax;
        if (z < zmin)
            z = zmin;
        if (z > zmax)
            z = zmax;
        printf("clip sig_x = %f\n", x);
        printf("clip sig_y = %f\n", y);
        printf("clip sig_z = %f\n", z);
        // double logx = log(x);
        // double logy, logz;

        int id0, id1, id2, id3;
        bool flag = false;
        findFourCoefs_sig(coefs, istab, wind, x, id0, id1, id2, id3, flag, 0);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData:sigmax search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif

        calcData_virtual(coefs, id0, id1, id2, id3, x, row, 0);
        printf("xk_flag, compareSigmaData, row.xv = %f\n", row.xv);

        findFourCoefs_sig(coefs, istab, wind, y, id0, id1, id2, id3, flag, 1);
        printf("xk_flag, compareSigmaData:sigmay search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
        calcData_virtual(coefs, id0, id1, id2, id3, y, row, 1);
        printf("xk_flag, compareSigmaData, row.yv = %f\n", row.yv);

        findFourCoefs_sig(coefs, istab, wind, z, id0, id1, id2, id3, flag, 2);
        printf("xk_flag, compareSigmaData:sigmaz search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
        calcData_virtual(coefs, id0, id1, id2, id3, z, row, 2);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData, row.zv = %f\n", row.zv);
#endif
        printf("xk_flag, compareSigmaData, row.x+row.xv = %f\n", row.x + row.xv);
        findFourCoefs(coefs, istab, wind, row.x + row.xv, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: x search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, row.x + row.xv, row, 0);

        findFourCoefs(coefs, istab, wind, row.x + row.yv, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: y search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, row.yv + row.x, row, 1);
        findFourCoefs(coefs, istab, wind, row.x + row.zv, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: z search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, row.zv + row.x, row, 2);
        printf("xk_flag, compareSigmaData, row.sig_x = %f\n", row.sig_x);
        printf("xk_flag, compareSigmaData, row.sig_y = %f\n", row.sig_y);
        printf("xk_flag, compareSigmaData, row.sig_z = %f\n", row.sig_z);

        // findFourCoefs_sig(coefs, istab, wind, y, id0, id1, id2, id3, flag);
        // calcData_virtual(coefs, id0, id1, id2, id3, y, row,1);
        // findFourCoefs_sig(coefs, istab, wind, z, id0, id1, id2, id3, flag);
        // calcData_virtual(coefs, id0, id1, id2, id3, z, row, 2);

        printf("zrcp=%f,zplume=%f,hml=%f\n", row.zrcp, row.zplume, row.hml);

        calDosage(row);

        /*
        double zfunc = zFunction(row.zrcp, row.zplume, row.hml, row.sig_z);
        printf("xk_flag, compareSigmaData, zfunc = %f\n", zfunc);

        double concentration = centerlineConcentration(
            (row.x + row.xv), row.t, row.mass, row.wind, zfunc, row.sig_x, row.sig_y);
        double dosage = centerlineDosage(
            (row.x + row.xv), row.t, row.mass, row.wind, zfunc, row.sig_x, row.sig_y);
        */
        double defaultnan = -999.;
        outputFile << i << "," << istab << "," << wind << ",";
        outputFile << row.x << "," << row.y << "," << row.z << ",";
        outputFile << row.icurve << "," << row.t << ",";
        outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
        outputFile << row.zfunc << "," << defaultnan << "," << row.concentration << "," << defaultnan << "," << row.dosage << "\n";

    } // end of loop over all rows in data
    outputFile.close();
    std::cout << "CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}

double calcDepletion(const CSVDataRow &row, const std::vector<CSVDataRow> &coefs)
{
    double x_high = row.x;
    double log_x_high = log(x_high);

    double x_low = 10.f;
    double log_x_low = log(x_low);
    double x_range = x_high - x_low;
    int integral_steps = 100;
    double step = (log_x_high - log_x_low) / (integral_steps - 1);

    double sumz = 0;
    for (int i = 0; i < integral_steps; i++)
    {
        // uniform distributed log scale from x_low to x_high

        double mylog_low = log_x_low + i * step;
        double mylog_high = log_x_low + (i + 1) * step;

        double mylog_x = log_x_low + i * step + 0.5 * step;
        double x = exp(mylog_x);
        double x_low = exp(mylog_low);
        double x_high = exp(mylog_high);

        int id0, id1, id2, id3;
        bool flag = false;

        findFourCoefs(coefs, row.istab, row.wind, x, id0, id1, id2, id3, flag);
        double sigz = calcSigz(coefs, id0, id1, id2, id3, x, row.wind);
        double zfunc = zFunction(row.zrcp, row.zplume, row.hml, sigz);

        sumz += zfunc * (x_high - x_low);
    }

    return exp(-sumz * row.vd / row.wind);
}

void generateDose(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.x;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    auto row_test = data[0];
    if (row_test.decay > 0)
    {
        outputFile << "ID,istab,wind,hml,x,y,z,icurve,t,dur,sig_x,sig_y,sig_z, zfunc, yfunc, xfunc,qyz,cpeak, concentration, c_remain, c_surf, dinf, c_surf_inf,dose, d_remain\n";
    }
    else if (row_test.vd >= 0)
    {
        outputFile << "ID,istab,wind,hml,vd, x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, yfunc, xfunc,qyz,cpeak, concentration, c_surf, c_surf_inf, C_dep, dose, D_dep, delta_dep\n";
    }
    else
    {
        outputFile << "ID,istab,wind,x,y,z,icurve,t,dur,sig_x,sig_y,sig_z, zfunc, yfunc, xfunc,qyz,cpeak, concentration, dinf, dose\n";
    }

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        double x = row.x;
        double y, z;
        int istab = row.istab;
        double wind = row.wind;

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
        // printf("\ni=%d, computeMode=5\n", i);
        // printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
        if (x < xmin)
            x = xmin;
        if (x > xmax)
            x = xmax;
        // double logx = log(x);
        // double logy, logz;

        int id0, id1, id2, id3;
        bool flag = false;
        findFourCoefs(coefs, istab, wind, x, id0, id1, id2, id3, flag);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif

        calcData(coefs, id0, id1, id2, id3, x, row);

        double zfunc = zFunction(row.zrcp, row.zplume, row.hml, row.sig_z);
        double yfunc = pdfFunction(row.y, row.sig_y);

        double qyz = row.mass * yfunc * zfunc;
        double xfunc, concentration, cpeak, xfuncp;
        double tip, tail, constant, tip1, tip0;
        double dosage;
        double dinf;

        double delta_dep, C_dep, D_dep;
        if (row.t >= 0)
        {
            if (row.dur < 1)
            {
                xfunc = pdfFunction(row.wind * row.t - row.x, row.sig_x);
                concentration = xfunc * qyz;
                // print using scientific notation
                // printf("xk_flag, pdfa = %f, xfunc = %f, qyz = %f, concentration = %8.4e\n", row.wind * row.t - row.x, xfunc, qyz, concentration);
                xfuncp = pdfFunction(std::min(row.x, 0.), row.sig_x);
                cpeak = xfuncp * qyz;
                tail = 0;
                tip = 0;

                double arg0 = -row.x;
                double arg1 = row.wind * row.t - row.x;
                xfunc = (cdfFunction(arg1, row.sig_x) - cdfFunction(arg0, row.sig_x)) / row.wind;
                dosage = xfunc * qyz;
                double xfuncinf = (1.f - cdfFunction(arg0, row.sig_x)) / row.wind;
                dinf = xfuncinf * qyz;

                if (row.vd >= 0)
                {
                    delta_dep = calcDepletion(row, coefs);
                    C_dep = concentration * delta_dep;
                    D_dep = dosage * delta_dep;
                }
            }
            else
            {
                // Calculate the tip
                double tip_time = row.t; // Adjust according to Python's tpeak function logic if necessary
                tip = cdfFunction(row.wind * tip_time - row.x, row.sig_x);

                // Calculate the tail
                double ut = std::max(0., row.t - row.dur) * row.wind; // corrected for the release duration
                tail = cdfFunction(ut - row.x, row.sig_x);

                // Calculate concentration
                concentration = (tip - tail) * qyz / row.wind / row.dur;

                double tp = std::max(row.dur, 0.5 * row.dur + row.x / row.wind);

                // Calculate peak concentration
                double tipp = cdfFunction(row.wind * tp - row.x, row.sig_x);
                double utp = std::max(0., tp - row.dur) * row.wind;
                double tailp = cdfFunction(utp - row.x, row.sig_x);
                cpeak = (tipp - tailp) * qyz / row.wind / row.dur;

                tip1 = IcdfFunction(row.wind * row.t - row.x, row.sig_x);
                tip0 = IcdfFunction(-row.x, row.sig_x);
                tip = tip1 - tip0;

                // For t<=T, tail is attached to the source
                double tail0 = cdfFunction(-row.x, row.sig_x) * std::min(row.t, row.dur) * row.wind / row.sig_x;

                // For t>=T, tail is moving
                double arg = row.wind * std::max(0., row.t - row.dur);
                double tail2 = IcdfFunction(arg - row.x, row.sig_x);
                double tail1 = IcdfFunction(-row.x, row.sig_x);
                tail = tail2 - tail1 + tail0;

                constant = qyz * row.sig_x / (row.dur * (row.wind * row.wind));
                dosage = (tip - tail) * constant;
                dinf = qyz / row.wind * cdfFunction(row.x, row.sig_x);
            }

            if (row.decay > 0)
            {
                double delta_remain = exp(-row.decay * row.x / row.wind);
                double c_remain = concentration * delta_remain;
                double c_surf = row.vd * D_dep;
                double c_surf_inf = row.vd * dinf * delta_dep;
                double d_remain = dosage * delta_remain;

                outputFile << i << "," << istab << "," << wind << "," << row.hml << ",";
                outputFile << row.x << "," << row.y << "," << row.z << ",";
                outputFile << row.icurve << "," << row.t << "," << row.dur << ",";
                outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
                outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
                outputFile << std::scientific << std::setprecision(8) << cpeak << "," << concentration << "," << c_remain << "," << c_surf << "," << dinf << "," << c_surf_inf << "," << dosage << "," << d_remain << "\n";
            }
            else if (row.vd >= 0)
            {
                double c_surf = row.vd * D_dep;
                double c_surf_inf = row.vd * dinf * delta_dep;
                outputFile << i << "," << istab << "," << wind << "," << row.hml << "," << row.vd << ",";
                outputFile << row.x << "," << row.y << "," << row.z << ",";
                outputFile << row.icurve << "," << row.t << "," << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
                outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
                outputFile << std::scientific << std::setprecision(8) << cpeak << "," << concentration << "," << c_surf << "," << c_surf_inf << "," << C_dep << "," << dosage << "," << D_dep << "," << delta_dep << "\n";
            }
            else
            {
                outputFile << i << "," << istab << "," << wind << ",";
                outputFile << row.x << "," << row.y << "," << row.z << ",";
                outputFile << row.icurve << "," << row.t << "," << row.dur << ",";
                outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
                outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
                outputFile << std::scientific << std::setprecision(8) << cpeak << "," << concentration << "," << dinf << "," << dosage << "\n";
            }
        }
        /*
        outputFile << i << "," << istab << "," << wind << ",";
        outputFile << row.x << "," << row.y << "," << row.z << ",";
        outputFile << row.icurve << "," << row.t << "," << row.dur << ",";
        outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
        outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
        outputFile << cpeak << "," << concentration << "," << dinf << "," << dosage << "\n";
        */

    } // end of loop over all rows in data
    outputFile.close();
    std::cout << "5 CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}

void generateDoseBatch(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.x;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    auto row_test = data[0];

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        std::ofstream outputFile(row.output_file);
        outputFile << "case,istab,wind,x,y,z,t,dur,";
        outputFile << "sig_x,sig_y,sig_z,zfunc,yfunc,xfunc,qyz,cpeak,concentration,dinf,dose,";
        if(row.vd >0)
            outputFile << "c_surf,c_surf_inf, C_dep, D_dep, delta_dep";
        outputFile << "\n";

        std::cout << "Writing to " << row.output_file << "\n";
        std::cout << "row.dur = " << row.dur << "\n";
        std::cout << "Reading from " << row.xyzt_file << "\n";
        auto xyzt_rows = CSVParser::parsexyztCSV(row.xyzt_file);

        int istab = row.istab;
        double wind = row.wind;

        for (int j = 0; j < xyzt_rows.size(); j++)
        {
            auto xyzt_row = xyzt_rows[j];
            double x = xyzt_row.x;
            double y = xyzt_row.y;
            double zrcp = xyzt_row.z;
            double t = xyzt_row.t;
            row.x = x;
            row.y = y;
            row.z = zrcp;
            row.t = t;

            if (x < xmin)
                x = xmin;
            if (x > xmax)
                x = xmax;
            // double logx = log(x);
            // double logy, logz;

            int id0, id1, id2, id3;
            bool flag = false;
            findFourCoefs(coefs, istab, wind, x, id0, id1, id2, id3, flag);
#if (PRTCHECK)
            printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif

            calcData(coefs, id0, id1, id2, id3, x, row);

            double zfunc = zFunction(zrcp, row.zplume, row.hml, row.sig_z);
            double yfunc = pdfFunction(y, row.sig_y);

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

                    xfunc = pdfFunction(row.wind * row.t - row.x, row.sig_x);
                    concentration = xfunc * qyz;
                    // print using scientific notation
                    // printf("xk_flag, pdfa = %f, xfunc = %f, qyz = %f, concentration = %8.4e\n", row.wind * row.t - row.x, xfunc, qyz, concentration);
                    xfuncp = pdfFunction(std::min(row.x, 0.), row.sig_x);
                    cpeak = xfuncp * qyz;
                    tail = 0;
                    tip = 0;

                    double arg0 = -row.x;
                    double arg1 = row.wind * row.t - row.x;
                    xfunc = (cdfFunction(arg1, row.sig_x) - cdfFunction(arg0, row.sig_x)) / row.wind;
                    dosage = xfunc * qyz;
                    double xfuncinf = (1.f - cdfFunction(arg0, row.sig_x)) / row.wind;
                    dinf = xfuncinf * qyz;
                }
                else
                {
                    // Calculate the tip
                    double tip_time = row.t; // Adjust according to Python's tpeak function logic if necessary
                    tip = cdfFunction(row.wind * tip_time - row.x, row.sig_x);

                    // Calculate the tail
                    double ut = std::max(0., row.t - row.dur) * row.wind; // corrected for the release duration
                    tail = cdfFunction(ut - row.x, row.sig_x);

                    // Calculate concentration
                    concentration = (tip - tail) * qyz / row.wind / row.dur;

                    double tp = std::max(row.dur, 0.5 * row.dur + row.x / row.wind);

                    // Calculate peak concentration
                    double tipp = cdfFunction(row.wind * tp - row.x, row.sig_x);
                    double utp = std::max(0., tp - row.dur) * row.wind;
                    double tailp = cdfFunction(utp - row.x, row.sig_x);
                    cpeak = (tipp - tailp) * qyz / row.wind / row.dur;

                    tip1 = IcdfFunction(row.wind * row.t - row.x, row.sig_x);
                    tip0 = IcdfFunction(-row.x, row.sig_x);
                    tip = tip1 - tip0;

                    // For t<=T, tail is attached to the source
                    double tail0 = cdfFunction(-row.x, row.sig_x) * std::min(row.t, row.dur) * row.wind / row.sig_x;

                    // For t>=T, tail is moving
                    double arg = row.wind * std::max(0., row.t - row.dur);
                    double tail2 = IcdfFunction(arg - row.x, row.sig_x);
                    double tail1 = IcdfFunction(-row.x, row.sig_x);
                    tail = tail2 - tail1 + tail0;

                    constant = qyz * row.sig_x / (row.dur * (row.wind * row.wind));
                    dosage = (tip - tail) * constant;
                    dinf = qyz / row.wind * cdfFunction(row.x, row.sig_x);
                }

                if (row.vd >= 0)
                {
                    delta_dep = calcDepletion(row, coefs);
                    C_dep = concentration * delta_dep;
                    D_dep = dosage * delta_dep;
                    c_surf = row.vd * D_dep;
                    c_surf_inf = row.vd * dinf * delta_dep;
                }
            }

            outputFile << i << "," << istab << "," << wind << ",";
            outputFile << row.x << "," << row.y << "," << row.z << ",";
            outputFile << row.t << "," << row.dur << ",";
            outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
            outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
            outputFile << std::scientific << std::setprecision(8) << cpeak << "," << concentration << "," << dinf << "," << dosage <<",";
            if(row.vd > 0)
            {
                outputFile << std::scientific << std::setprecision(8)  << c_surf << "," << c_surf_inf << "," << C_dep << "," << D_dep << "," << delta_dep ;;

            }
            outputFile << "\n";
        } // xyzt_rows

        outputFile.close();
    } // end of loop over all rows in data
    std::cout << "9 CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}

void generateComplete(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double ymin = 9999999.f;
    double ymax = -9999999.f;
    double zmin = 9999999.f;
    double zmax = -9999999.f;

    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (y < ymin)
            ymin = y;
        if (y > ymax)
            ymax = y;
        if (z < zmin)
            zmin = z;
        if (z > zmax)
            zmax = z;

        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    outputFile << "ID,istab,wind,x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, yfunc, xfunc,qyz,cpeak, concentration, dinf, dose\n";

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        int istab = row.istab;
        double wind = row.wind;

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
        printf("\ni=%d\n", i);
        printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
        /*
        if (x < xmin)
            x = xmin;
        if (x > xmax)
            x = xmax;
        if (y < ymin)
            y = ymin;
        if (y > ymax)
            y = ymax;
        if (z < zmin)
            z = zmin;
        if (z > zmax)
            z = zmax;
            */
        printf("clip sig_x = %f\n", x);
        printf("clip sig_y = %f\n", y);
        printf("clip sig_z = %f\n", z);
        // double logx = log(x);
        // double logy, logz;

        int id0, id1, id2, id3;
        bool flag = false;

        if (x > EPSILON)
        {
            findFourCoefs_sig(coefs, istab, wind, x, id0, id1, id2, id3, flag, 0);
#if (PRTCHECK)
            printf("xk_flag, compareSigmaData:sigmax search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
            calcData_virtual_new(coefs, id0, id1, id2, id3, x, row, 0);
        }
        else
            row.xv = 0;
        printf("xk_flag, compareSigmaData, row.xv = %f\n", row.xv);

        if (y > EPSILON)
        {
            findFourCoefs_sig(coefs, istab, wind, y, id0, id1, id2, id3, flag, 1);
            printf("xk_flag, compareSigmaData:sigmay search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
            calcData_virtual_new(coefs, id0, id1, id2, id3, y, row, 1);
        }
        else
            row.yv = 0;
        printf("xk_flag, compareSigmaData, row.yv = %f\n", row.yv);

        if (z > EPSILON)
        {
            findFourCoefs_sig(coefs, istab, wind, z, id0, id1, id2, id3, flag, 2);
            printf("xk_flag, compareSigmaData:sigmaz search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
            calcData_virtual_new(coefs, id0, id1, id2, id3, z, row, 2);
        }
        else
            row.zv = 0;
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData, row.zv = %f\n", row.zv);
#endif

        double x_merge = std::max(0., row.x) + row.xv;
        double y_merge = std::max(0., row.x) + row.yv;
        double z_merge = std::max(0., row.x) + row.zv;

        printf("xk_flag, compareSigmaData, row.x+row.xv = %f\n", row.x + row.xv);
        findFourCoefs(coefs, istab, wind, x_merge, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: x search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, x_merge, row, 0);

        printf("xk_flag, compareSigmaData, row.x+row.yv = %f\n", row.x + row.yv);
        findFourCoefs(coefs, istab, wind, y_merge, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: y search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, y_merge, row, 1);

        printf("xk_flag, compareSigmaData, row.x+row.zv = %f\n", row.x + row.zv);
        findFourCoefs(coefs, istab, wind, z_merge, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: z search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, z_merge, row, 2);
        printf("xk_flag, compareSigmaData, row.sig_x = %f\n", row.sig_x);
        printf("xk_flag, compareSigmaData, row.sig_y = %f\n", row.sig_y);
        printf("xk_flag, compareSigmaData, row.sig_z = %f\n", row.sig_z);

        // findFourCoefs_sig(coefs, istab, wind, y, id0, id1, id2, id3, flag);
        // calcData_virtual(coefs, id0, id1, id2, id3, y, row,1);
        // findFourCoefs_sig(coefs, istab, wind, z, id0, id1, id2, id3, flag);
        // calcData_virtual(coefs, id0, id1, id2, id3, z, row, 2);

        printf("zrcp=%f,zplume=%f,hml=%f\n", row.zrcp, row.zplume, row.hml);

        calDosage(row);
        outputFile << i << "," << istab << "," << wind << ",";
        outputFile << row.x << "," << row.y << "," << row.z << ",";
        outputFile << row.icurve << "," << row.t << ",";
        outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
        outputFile << row.zfunc << "," << row.yfunc << "," << row.xfunc << "," << row.qyz << "," << row.cpeak << "," << row.concentration << "," << row.dinf << "," << row.dosage << "\n";

    } // end of loop over all rows in data
    outputFile.close();
    std::cout << "CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}

void generateCompleteBatch(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double ymin = 9999999.f;
    double ymax = -9999999.f;
    double zmin = 9999999.f;
    double zmax = -9999999.f;

    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (y < ymin)
            ymin = y;
        if (y > ymax)
            ymax = y;
        if (z < zmin)
            zmin = z;
        if (z > zmax)
            zmax = z;

        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    //outputFile << "ID,istab,wind,x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, yfunc, xfunc,qyz,cpeak, concentration, dinf, dose\n";

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        std::ofstream outputFile(row.output_file);
        outputFile << "case,istab,wind,x,y,z,t,dur,";
        outputFile << "sig_x,sig_y,sig_z,zfunc,yfunc,xfunc,qyz,cpeak,concentration,dinf,dose,";
        if(row.vd >0)
            outputFile << "c_surf,c_surf_inf, C_dep, D_dep, delta_dep";
        outputFile << "\n";
        std::cout << "Writing to " << row.output_file << "\n";
        std::cout << "row.dur = " << row.dur << "\n";
        std::cout << "Reading from " << row.xyzt_file << "\n";
        double sig_x0 = row.sig_x0;
        double sig_y0 = row.sig_y0;
        double sig_z0 = row.sig_z0;
        auto xyzt_rows = CSVParser::parsexyztCSV(row.xyzt_file);

        int istab = row.istab;
        double wind = row.wind;

        int id0, id1, id2, id3;
        bool flag = false;

        printf("generateCompleteBatch, row %d sig_x0 = %f\n", i, sig_x0);

        if (sig_x0 > EPSILON)
        {
            findFourCoefs_sig(coefs, istab, wind, sig_x0, id0, id1, id2, id3, flag, 0);
            calcData_virtual_new(coefs, id0, id1, id2, id3, sig_x0, row, 0);
        }
        else
            row.xv = 0;
        printf("generateCompleteBatch, row %d xv = %f\n", i, row.xv);
        printf("generateCompleteBatch, row %d sig_y0 = %f\n", i, sig_y0);

        if (sig_y0 > EPSILON)
        {
            findFourCoefs_sig(coefs, istab, wind, sig_y0, id0, id1, id2, id3, flag, 1);
            calcData_virtual_new(coefs, id0, id1, id2, id3, sig_y0, row, 1);
        }
        else
            row.yv = 0;

        if (sig_z0 > EPSILON)
        {
            findFourCoefs_sig(coefs, istab, wind, sig_z0, id0, id1, id2, id3, flag, 2);
            calcData_virtual_new(coefs, id0, id1, id2, id3, sig_z0, row, 2);
        }
        else
            row.zv = 0;

        for (int j = 0; j < xyzt_rows.size(); j++)
        {
            auto xyzt_row = xyzt_rows[j];
            double x = xyzt_row.x;
            double y = xyzt_row.y;
            double zrcp = xyzt_row.z;
            double t = xyzt_row.t;
            row.x = x;
            row.y = y;
            row.z = zrcp;
            row.t = t;

            if (x < xmin)
                x = xmin;
            if (x > xmax)
                x = xmax;

            double x_merge = std::max(0., row.x) + row.xv;
            double y_merge = std::max(0., row.x) + row.yv;
            double z_merge = std::max(0., row.x) + row.zv;

            findFourCoefs(coefs, istab, wind, x_merge, id0, id1, id2, id3, flag);
            calcSigma(coefs, id0, id1, id2, id3, x_merge, row, 0);

            findFourCoefs(coefs, istab, wind, y_merge, id0, id1, id2, id3, flag);
            calcSigma(coefs, id0, id1, id2, id3, y_merge, row, 1);

            findFourCoefs(coefs, istab, wind, z_merge, id0, id1, id2, id3, flag);
            calcSigma(coefs, id0, id1, id2, id3, z_merge, row, 2);

            //printf("xk_flag, compareSigmaData, row.sig_x = %f\n", row.sig_x);
            //printf("xk_flag, compareSigmaData, row.sig_y = %f\n", row.sig_y);
            //printf("xk_flag, compareSigmaData, row.sig_z = %f\n", row.sig_z);
            double zfunc = zFunction(zrcp, row.zplume, row.hml, row.sig_z);
            double yfunc = pdfFunction(y, row.sig_y);

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

                    xfunc = pdfFunction(row.wind * row.t - row.x, row.sig_x);
                    concentration = xfunc * qyz;
                    // print using scientific notation
                    // printf("xk_flag, pdfa = %f, xfunc = %f, qyz = %f, concentration = %8.4e\n", row.wind * row.t - row.x, xfunc, qyz, concentration);
                    xfuncp = pdfFunction(std::min(row.x, 0.), row.sig_x);
                    cpeak = xfuncp * qyz;
                    tail = 0;
                    tip = 0;

                    double arg0 = -row.x;
                    double arg1 = row.wind * row.t - row.x;
                    xfunc = (cdfFunction(arg1, row.sig_x) - cdfFunction(arg0, row.sig_x)) / row.wind;
                    dosage = xfunc * qyz;
                    double xfuncinf = (1.f - cdfFunction(arg0, row.sig_x)) / row.wind;
                    dinf = xfuncinf * qyz;
                }
                else
                {
                    // Calculate the tip
                    double tip_time = row.t; // Adjust according to Python's tpeak function logic if necessary
                    tip = cdfFunction(row.wind * tip_time - row.x, row.sig_x);

                    // Calculate the tail
                    double ut = std::max(0., row.t - row.dur) * row.wind; // corrected for the release duration
                    tail = cdfFunction(ut - row.x, row.sig_x);

                    // Calculate concentration
                    concentration = (tip - tail) * qyz / row.wind / row.dur;

                    double tp = std::max(row.dur, 0.5 * row.dur + row.x / row.wind);

                    // Calculate peak concentration
                    double tipp = cdfFunction(row.wind * tp - row.x, row.sig_x);
                    double utp = std::max(0., tp - row.dur) * row.wind;
                    double tailp = cdfFunction(utp - row.x, row.sig_x);
                    cpeak = (tipp - tailp) * qyz / row.wind / row.dur;

                    tip1 = IcdfFunction(row.wind * row.t - row.x, row.sig_x);
                    tip0 = IcdfFunction(-row.x, row.sig_x);
                    tip = tip1 - tip0;

                    // For t<=T, tail is attached to the source
                    double tail0 = cdfFunction(-row.x, row.sig_x) * std::min(row.t, row.dur) * row.wind / row.sig_x;

                    // For t>=T, tail is moving
                    double arg = row.wind * std::max(0., row.t - row.dur);
                    double tail2 = IcdfFunction(arg - row.x, row.sig_x);
                    double tail1 = IcdfFunction(-row.x, row.sig_x);
                    tail = tail2 - tail1 + tail0;

                    constant = qyz * row.sig_x / (row.dur * (row.wind * row.wind));
                    dosage = (tip - tail) * constant;
                    dinf = qyz / row.wind * cdfFunction(row.x, row.sig_x);
                }

                if (row.vd >= 0)
                {
                    delta_dep = calcDepletion(row, coefs);
                    C_dep = concentration * delta_dep;
                    D_dep = dosage * delta_dep;
                    c_surf = row.vd * D_dep;
                    c_surf_inf = row.vd * dinf * delta_dep;
                }
            }//if (t >= 0)

            outputFile << i << "," << istab << "," << wind << ",";
            outputFile << row.x << "," << row.y << "," << row.z << ",";
            outputFile << row.t << "," << row.dur << ",";
            outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
            outputFile << zfunc << "," << yfunc << "," << xfunc << "," << qyz << ",";
            outputFile << std::scientific << std::setprecision(8) << cpeak << "," << concentration << "," << dinf << "," << dosage <<",";
            if(row.vd > 0)
            {
                outputFile << std::scientific << std::setprecision(8)  << c_surf << "," << c_surf_inf << "," << C_dep << "," << D_dep << "," << delta_dep ;;

            }
            outputFile << "\n";

        }//for (int j = 0; j < xyzt_rows.size(); j++)

        outputFile.close();
    } // end of loop over all rows in data


    //outputFile.close();
    std::cout << "12 CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}