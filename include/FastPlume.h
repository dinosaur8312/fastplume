#ifndef FASTPLUME_H
#define FASTPLUME_H

#include <string>
#include <vector>
#include <memory>
#include "taskDataRow.h"

namespace FastPlume {

class FastPlumeImpl;  // Forward declaration of the implementation class

class FastPlume {
public:
    FastPlume();
    explicit FastPlume(const std::string& configFilePath);
    ~FastPlume();

    // Chaining methods to set attributes and configurations

    FastPlume& setAttr(const std::string& attrName, const std::vector<double>& values);
    FastPlume& setAttr(const std::string& attrName, const std::vector<int>& values);
    FastPlume& setAttr(const std::string& attrName, const std::vector<std::vector<double>>& values);
    FastPlume& setAttr(const std::string& attrName, const std::vector<std::vector<std::vector<double>>>& values);
    FastPlume& setDispersionCoefCSV(const std::string& filePath);
    FastPlume& setTaskDataCSV(const std::string& filePath);
    FastPlume& setOutputFilePath(const std::string& filePath);
    FastPlume& setLocDataCSVImportDirectory(const std::string& directory);
    FastPlume& setOutputDirectory(const std::string& directory);
    FastPlume& setOutputMethod(const std::string& method);

    // Run simulation
    void run();


    // Retrieve attribute data
    template <typename T>
    std::vector<T> getAttr(const std::string& attrName) const;

    std::vector<taskDataRow> getResults() const;

    void printResult() const;

private:
    std::unique_ptr<FastPlumeImpl> pImpl;  // Pointer to the implementation
};

} // namespace FastPlume

#endif // FASTPLUME_H
