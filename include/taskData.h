#ifndef TASKDATA_H
#define TASKDATA_H

#include <iostream>
#include <vector>
#include <string>

namespace FastPlume{

class locData
{
    public:
    // Constructor to initialize from a CSV file path   
        void parseCSV(const std::string &filePath);
        int getLocNum() const{
            return x.size();
        }
    std::vector<int> iloc;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> sig_x;
    std::vector<double> sig_y;
    std::vector<double> sig_z;
    std::vector<double> t;
    std::vector<double> concentration;
    std::vector<double> dosage;
    std::vector<double> cpeak;
    std::vector<double> dinf;
    std::vector<double> xfunc;
    std::vector<double> xfuncp;
    std::vector<double> yfunc;
    std::vector<double> zfunc;
    std::vector<double> qyz;
    std::vector<double> ctip;
    std::vector<double> ctail;

     // Granting taskData full access to private members
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
    locData m_locData;
};

class taskData
{
    public:

    //default constructor
    taskData() {}
    // Constructor to initialize from a CSV file path
    taskData(const std::string &csvFilePath);

    // Destructor
    ~taskData();


    // Template method to set attributes
    template<typename T>
    void setAttr(const std::string &attrName, const std::vector<T> &values);

    // Template method to get attributes
    template <typename T>
    std::vector<T> getAttr(const std::string &attrName) const;

    // Template method to print a specific attribute
    template <typename T>
    void printAttr(const std::string &attrName) const;

    // Method to print all data
    void printData() const;

    taskDataRow getRow(int index) const;
    void setRow(int index, const taskDataRow &row);

    int getTaskNum() const;


    private:

    void parseCSV(const std::string &csvFilePath);

    int taskNum;

    std::vector<int> id;
    std::vector<double> sig_x0;
    std::vector<double> sig_y0;
    std::vector<double> sig_z0;
    std::vector<int> istab;
    std::vector<double> wind;
    std::vector<double> mass;
    std::vector<double> decay;
    std::vector<double> vd;
    std::vector<double> hml;
    std::vector<double> zplume;
    std::vector<double> xv;
    std::vector<double> yv;
    std::vector<double> zv;
    std::vector<double> dur;

    std::vector<std::string> xyzt_file;
    std::vector<std::string> output_file;

    std::vector<locData> v_locData;
};

} // namespace FastPlume

#endif // TASKDATA_H