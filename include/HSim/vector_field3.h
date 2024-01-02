#pragma once

#include <HSim/common.h>
#include <HSim/field3.h>

namespace HSim
{
    template <typename T, size_t X, size_t Y, size_t Z>
    class VectorField3 : public Field3<T, X, Y, Z>
    {
    public:
        VectorField3() {};
        ~VectorField3() {};
    };
    

    

} // namespace HSim
