// FastPlume.h
#ifndef FASTPLUME_H
#define FASTPLUME_H

#include <string>

namespace FastPlume {

class FastPlumeImpl;  // Forward declaration of the implementation class

class FastPlume {
public:
    FastPlume(const std::string& inputFileName);
    ~FastPlume();
    void run();

private:
    FastPlumeImpl* pImpl;  // Pointer to the implementation
};

} // namespace FastPlume

#endif // FASTPLUME_H
