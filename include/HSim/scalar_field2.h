#pragma once

#include <HSim/common.h>
#include <HSim/field2.h>

namespace HSim
{
    template <typename T, size_t X, size_t Y>
    class ScalarField2 : public Field2<T, X, Y>
    {

    public:
        ScalarField2() {}
        ~ScalarField2() {}
        
    public:
        virtual T sample(const Vec2i& pos) = 0;
        virtual Vec2<T> gradient(const Vec2i& pos) = 0;
        virtual T laplacian(const Vec2i& pos) = 0;
        
    };

} // namespace HSim