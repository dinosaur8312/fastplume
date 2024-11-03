#ifndef FASTPLUME_IMPL_H
#define FASTPLUME_IMPL_H

#include <string>
#include "dispersionCoef.h"
#include "taskData.h"

namespace FastPlume
{
    class FastPlumeImpl
    {
    public:
        explicit FastPlumeImpl(const std::string &JsonInputFileName);
        void run();

        int getTaskNum() const
        {
            return m_taskData.getTaskNum();
        }

    private:
        dispersionCoef m_coefData;
        taskData m_taskData;

        float interpolate_x_from_sig_component(int istab, float wind, float sig_value, const std::string &flag);
        std::vector<float> interpolate_sig_component(int istab, float wind, float x, const std::string &flag);
        double calcDepletion(const taskDataRow &row, double x, double z);
    };
} // namespace FastPlume

#endif // FASTPLUME_IMPL_H
