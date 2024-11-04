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

FastPlume& FastPlume::setAttr(const std::string& attrName, const std::vector<int>& values) {
    pImpl->setAttr(attrName, values);
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

FastPlume& FastPlume::setTaskDataCSV(const std::string& filePath) {
    pImpl->setTaskDataCSV(filePath);
    return *this;
}

FastPlume& FastPlume::setOutputFilePath(const std::string& filePath) {
    pImpl->setOutputFilePath(filePath);
    return *this;
}

FastPlume& FastPlume::setOutputMethod(const std::string& method) {
    pImpl->setOutputMethod(method);
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

void FastPlume::run() {
    pImpl->run();
}

std::vector<taskDataRow> FastPlume::getResults() const {
    return pImpl->getResults();
}

void FastPlume::printResult() const {
    pImpl->printResult();
}

} // namespace FastPlume
