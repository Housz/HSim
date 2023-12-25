#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>

namespace HSim
{
    template <typename T>
    class Mat33
    {
    public:
        Mat33() { container.fill(0); };
        ~Mat33() {};

        Mat33(T v) { container.fill(v); }
        Mat33(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
        {
        }

    // setter, getter
    public:
        void set(T v);
        void set(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22);

        template <typename T1>
        void set(const std::initializer_list<T1> &list);

        template <typename T1>
        void set(const std::initializer_list<std::initializer_list<T1>> &list);

        template <typename T1>
        void set(Mat33<T1> m);

    
    // data
    public:
        /**
         * row-major
         * 
         * 0  1  2
         * 3  4  5
         * 6  7  8
         * 
         * 00 01 02
         * 10 11 12
         * 20 21 22
        */
        std::array<T, 9> container;

    };
    
    
    using Mat33i = Mat33<int>;
    using Mat33f = Mat33<float>;
    using Mat33d = Mat33<double>;

    

} // namespace HSim
