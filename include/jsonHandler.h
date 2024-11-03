#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <json.hpp>
#include "taskConfig.h"
#include <string>
#include <fstream>
namespace FastPlume
{

class jsonHandler
{
public:
    explicit jsonHandler(const std::string &inputFileName = "");
    static taskConfig processJsonFile(const std::string &filePath);

private:
    static std::string findJsonFile(const std::string &fileName);
    static std::string basePath;
    static std::string currentPath;
};

} // namespace FastPlume

#endif // JSONHANDLER_H
