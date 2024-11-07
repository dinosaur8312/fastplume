#ifndef FASTPLUME_IMPL_H
#define FASTPLUME_IMPL_H

#include <string>
#include <vector>
#include "dispersionCoef.h"
#include "taskData.h"

namespace FastPlume
{
    class FastPlumeImpl
    {
    public:
        FastPlumeImpl();
        explicit FastPlumeImpl(const std::string &JsonInputFileName);

        // Main run method to execute the simulation
        void run();
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<double> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<int> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<std::vector<double>> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<std::vector<std::vector<double>>> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<locData> &values);

        // Templated getAttr method to retrieve an attribute by name
        template <typename T>
        std::vector<T> getAttr(const std::string &attrName) const;

        FastPlumeImpl &setDispersionCoefCSV(const std::string &filePath);
        FastPlumeImpl &setTaskDataCSV(const std::string &filePath);
        FastPlumeImpl &setOutputMethod(const std::string &method);
        FastPlumeImpl &setOutputFilePath(const std::string &outputPath);
        FastPlumeImpl &setLocDataCSVImportDirectory(const std::string &directory);
        FastPlumeImpl &setOutputDirectory(const std::string &directory);
        // Accessor for task count
        int getTaskNum() const { return m_taskData.getTaskNum(); }

        std::vector<taskDataRow> getResults() const;
        void printResult() const;

    private:
        dispersionCoef m_coefData;
        taskData m_taskData;
        std::string DispersionCoefCSVPath;
        std::string TaskDataCSVPath;
        std::string LocDataCSVImportDirectory;
        std::string outputFilePath;
        std::string outputMethod;
        std::string outputDirectory;

        void validateConfiguration() const;

        // Internal methods for interpolation and calculation
        float interpolate_x_from_sig_component(int istab, float wind, float sig_value, const std::string &flag);
        std::vector<float> interpolate_sig_component(int istab, float wind, float x, const std::string &flag);
        double calcDepletion(const taskDataRow &row, double x, double z);
    };
} // namespace FastPlume

#endif // FASTPLUME_IMPL_H
