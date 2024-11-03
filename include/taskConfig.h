#ifndef TASKCONFIG_H
#define TASKCONFIG_H

#include <string>
namespace FastPlume
{

class taskConfig
{
    public:
    std::string coefCSVPath;
    std::string taskCSVPath;
    std::string outputCSVPath;
};

} // namespace FastPlume


#endif // TASKCONFIG_H