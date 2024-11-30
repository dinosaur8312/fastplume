#ifndef FASTPLUME_H
#define FASTPLUME_H

#include <string>
#include <vector>
#include <memory>
#include "taskDataRow.h"
#include "taskEvapDataRow.h"

namespace FastPlume {

class FastPlumeImpl;  // Forward declaration of the implementation class

class FastPlume {
public:
    FastPlume();
    explicit FastPlume(const std::string& configFilePath);
    ~FastPlume();

    // Chaining methods to set attributes and configurations

    FastPlume& setAttr(const std::string& attrName, const std::vector<double>& values);
    FastPlume& setEvapAttr(const std::string& attrName, const std::vector<double>& values);
    FastPlume& setAttr(const std::string& attrName, const std::vector<int>& values);
    FastPlume& setEvapAttr(const std::string& attrName, const std::vector<int>& values);
    FastPlume& setAttr(const std::string& attrName, const std::vector<std::vector<double>>& values);
    FastPlume& setEvapAttr(const std::string& attrName, const std::vector<std::vector<double>>& values);
    FastPlume& setAttr(const std::string& attrName, const std::vector<std::vector<std::vector<double>>>& values);
    FastPlume& setDispersionCoefCSV(const std::string& filePath);
    FastPlume& setAgentPropertiesCSV(const std::string& filePath);
    FastPlume& setTaskDataCSV(const std::string& filePath);
    FastPlume& setEvapTaskDataCSV(const std::string& filePath);
    FastPlume& setOutputFilePath(const std::string& filePath);
    FastPlume& setEvapOutputFilePath(const std::string& filePath);
    FastPlume& setLocDataCSVImportDirectory(const std::string& directory);
    FastPlume& setOutputDirectory(const std::string& directory);
    FastPlume& setEvapOutputDirectory(const std::string& directory);
    FastPlume& setOutputMethod(const std::string& method);
    FastPlume& setEvapOutputMethod(const std::string& method);

    // Run simulation
    void run();

    void run_evap();


    // Retrieve attribute data
    template <typename T>
    std::vector<T> getAttr(const std::string& attrName) const;
    template <typename T>
    std::vector<T> getEvapAttr(const std::string& attrName) const;

    std::vector<taskDataRow> getResults() const;
    std::vector<taskEvapDataRow> getEvapResults() const;
    

    void printResult() const;
    void printEvapResult() const;

private:
    std::unique_ptr<FastPlumeImpl> pImpl;  // Pointer to the implementation
};

} // namespace FastPlume

#endif // FASTPLUME_H
