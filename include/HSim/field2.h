#pragma once

#include <HSim/vec2.h>

namespace HSim
{
    template <typename T, size_t X, size_t Y>
    class Field2
    {
    public:
        Field2() {}
        ~Field2() {}

        size_t size_x() { return X; }
        size_t size_y() { return Y; }

    };


} // namespace HSim