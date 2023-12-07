#pragma once

#include <HSim/common.h>
#include <HSim/field2.h>

namespace HSim
{
    template <typename T, size_t X, size_t Y>
    class VectorField2 : public Field2<T, X, Y>
    {

    public:
        VectorField2() {}
        ~VectorField2() {}

    public:
        virtual Vec2<T> sample(const Vec2i& pos) = 0;
        

    };

} // namespace HSim