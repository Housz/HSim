#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>

namespace HSim
{
    template <typename T>
    class Mat33
    {
    public:
        Mat33() {};
        ~Mat33() {};
    
    // data
    public:
        std::array<T, 9> container;

    };
    

    

} // namespace HSim
