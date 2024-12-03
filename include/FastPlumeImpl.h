#ifndef FASTPLUME_IMPL_H
#define FASTPLUME_IMPL_H

#include <string>
#include <vector>
#include "dispersionCoef.h"
#include "taskData.h"
#include "taskEvapData.h"

namespace FastPlume
{
    class FastPlumeImpl
    {
    public:
        FastPlumeImpl();
        explicit FastPlumeImpl(const std::string &JsonInputFileName);

        // Main run method to execute the simulation
        void run();
        void run_evap();
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<double> &values);
        FastPlumeImpl &setEvapAttr(const std::string &attrName, const std::vector<double> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<int> &values);
        FastPlumeImpl &setEvapAttr(const std::string &attrName, const std::vector<int> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<std::vector<double>> &values);
        FastPlumeImpl &setEvapAttr(const std::string &attrName, const std::vector<std::vector<double>> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<std::vector<std::vector<double>>> &values);
        FastPlumeImpl &setAttr(const std::string &attrName, const std::vector<locData> &values);

        // Templated getAttr method to retrieve an attribute by name
        template <typename T>
        std::vector<T> getAttr(const std::string &attrName) const;
        template <typename T>
        std::vector<T> getEvapAttr(const std::string &attrName) const;

        FastPlumeImpl &setDispersionCoefCSV(const std::string &filePath);
        FastPlumeImpl &setAgentPropertiesCSV(const std::string &filePath);
        FastPlumeImpl &setTaskDataCSV(const std::string &filePath);
        FastPlumeImpl &setEvapTaskDataCSV(const std::string &filePath);
        FastPlumeImpl &setOutputMethod(const std::string &method);
        FastPlumeImpl &setEvapOutputMethod(const std::string &method);
        FastPlumeImpl &setOutputFilePath(const std::string &outputPath);
        FastPlumeImpl &setEvapOutputFilePath(const std::string &outputPath);
        FastPlumeImpl &setLocDataCSVImportDirectory(const std::string &directory);
        FastPlumeImpl &setOutputDirectory(const std::string &directory);
        FastPlumeImpl &setEvapOutputDirectory(const std::string &directory);
        // Accessor for task count
        int getTaskNum() const { return m_taskData.getTaskNum(); }
        int getEvapTaskNum() const { return m_taskEvapData.getTaskNum(); }

        std::vector<taskDataRow> getResults() const;
        std::vector<taskEvapDataRow> getEvapResults() const;
        void printResult() const;
        void printEvapResult() const;

    private:
        dispersionCoef m_coefData;
        agentProperties m_agentData;
        taskData m_taskData;
        taskEvapData m_taskEvapData;
        std::string DispersionCoefCSVPath;
        std::string AgentPropertiesCSVPath;
        std::string TaskDataCSVPath;
        std::string EvapTaskDataCSVPath;
        std::string LocDataCSVImportDirectory;
        std::string outputFilePath;
        std::string outputEvapFilePath;
        std::string outputMethod;
        std::string outputEvapMethod;
        std::string outputDirectory;
        std::string outputEvapDirectory;

        void validateConfiguration() const;

        // Internal methods for interpolation and calculation
        float interpolate_x_from_sig_component(int istab, float wind, float sig_value, const std::string &flag);
        std::vector<float> interpolate_sig_component(int istab, float wind, float x, const std::string &flag);
        double calcDepletion(const taskDataRow &row, double x, double z);
        double run_evap_row(taskEvapDataRow &row);
    };
} // namespace FastPlume

#endif // FASTPLUME_IMPL_H
