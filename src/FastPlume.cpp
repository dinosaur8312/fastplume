#include "FastPlume.h"
#include "FastPlumeImpl.h"  // Include the full definition of FastPlumeImpl

namespace FastPlume {

FastPlume::FastPlume() : pImpl(std::make_unique<FastPlumeImpl>()) {}

FastPlume::FastPlume(const std::string& configFilePath)
    : pImpl(std::make_unique<FastPlumeImpl>(configFilePath)) {}

FastPlume::~FastPlume() = default;

FastPlume& FastPlume::setAttr(const std::string& attrName, const std::vector<double>& values) {
    pImpl->setAttr(attrName, values);
    return *this;
}
FastPlume& FastPlume::setEvapAttr(const std::string& attrName, const std::vector<double>& values) {
    pImpl->setAttr(attrName, values);
    return *this;
}

FastPlume& FastPlume::setAttr(const std::string& attrName, const std::vector<int>& values) {
    pImpl->setAttr(attrName, values);
    return *this;
}
FastPlume& FastPlume::setEvapAttr(const std::string& attrName, const std::vector<int>& values) {
    pImpl->setEvapAttr(attrName, values);
    return *this;
}

FastPlume& FastPlume::setAttr(const std::string& attrName, const std::vector<std::vector<double>>& values) {
    pImpl->setAttr(attrName, values);
    return *this;
}

FastPlume& FastPlume::setAttr(const std::string& attrName, const std::vector<std::vector<std::vector<double>>>& values) {
    pImpl->setAttr(attrName, values);
    return *this;
}

FastPlume& FastPlume::setDispersionCoefCSV(const std::string& filePath) {
    pImpl->setDispersionCoefCSV(filePath);
    return *this;
}

FastPlume& FastPlume::setAgentPropertiesCSV(const std::string& filePath) {
    pImpl->setAgentPropertiesCSV(filePath);
    return *this;
}

FastPlume& FastPlume::setTaskDataCSV(const std::string& filePath) {
    pImpl->setTaskDataCSV(filePath);
    return *this;
}

FastPlume& FastPlume::setEvapTaskDataCSV(const std::string& filePath) {
    pImpl->setEvapTaskDataCSV(filePath);
    return *this;
}

FastPlume& FastPlume::setOutputFilePath(const std::string& filePath) {
    pImpl->setOutputFilePath(filePath);
    return *this;
}
FastPlume& FastPlume::setEvapOutputFilePath(const std::string& filePath) {
    pImpl->setEvapOutputFilePath(filePath);
    return *this;
}

FastPlume& FastPlume::setOutputMethod(const std::string& method) {
    pImpl->setOutputMethod(method);
    return *this;
}
FastPlume& FastPlume::setEvapOutputMethod(const std::string& method) {
    pImpl->setEvapOutputMethod(method);
    return *this;
}

FastPlume& FastPlume::setLocDataCSVImportDirectory(const std::string& directory) {
    pImpl->setLocDataCSVImportDirectory(directory);
    return *this;
}

FastPlume& FastPlume::setOutputDirectory(const std::string& directory) {
    pImpl->setOutputDirectory(directory);
    return *this;
}
FastPlume& FastPlume::setEvapOutputDirectory(const std::string& directory) {
    pImpl->setEvapOutputDirectory(directory);
    return *this;
}

void FastPlume::run() {
    pImpl->run();
}

void FastPlume::run_evap() {
    pImpl->run_evap();
}

std::vector<taskDataRow> FastPlume::getResults() const {
    return pImpl->getResults();
}
std::vector<taskEvapDataRow> FastPlume::getEvapResults() const {
    return pImpl->getEvapResults();
}

void FastPlume::printResult() const {
    pImpl->printResult();
}
void FastPlume::printEvapResult() const {
    pImpl->printEvapResult();
}

template <typename T>
std::vector<T> FastPlume::getAttr(const std::string& attrName) const {
    return pImpl->getAttr<T>(attrName);
}

template <typename T>
std::vector<T> FastPlume::getEvapAttr(const std::string& attrName) const {
    return pImpl->getEvapAttr<T>(attrName);
}

} // namespace FastPlume
