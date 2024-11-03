// FastPlume.cpp
#include "FastPlume.h"
#include "FastPlumeImpl.h"


namespace FastPlume
{

    // Public methods of the FastPlume API

    FastPlume::FastPlume(const std::string &JsonInputFileName)
        : pImpl(new FastPlumeImpl(JsonInputFileName)) {}

    FastPlume::~FastPlume()
    {
        delete pImpl;
    }

    void FastPlume::run()
    {
        pImpl->run();
    }

} // namespace FastPlume
