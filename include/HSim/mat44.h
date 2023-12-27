#pragma once

#include <HSim/common.h>
#include <HSim/vec4.h>

namespace HSim
{
    /**
     * @brief 4x4 matrix
     *
     * row-major
     *
     * 0  1  2  3       00 01 02 03
     * 4  5  6  7       10 11 12 13
     * 8  9  10 11      20 21 22 23
     * 12 13 14 15      30 31 32 33
     *
     * @tparam T
     */
    template <typename T>
    class Mat44
    {
        // constructor
    public:
        Mat44() { container.fill(0); };
        ~Mat44(){};

        Mat44(T v) { set(v); }

        Mat44(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) 
        {
            set(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
        }
                
        template <typename T1>
        Mat44(const std::initializer_list<T1> &list) { set(list); }

        template <typename T1>
        Mat44(const Mat33<T1> &m) { set(m); }

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Mat44<T1> &);

        // setter, getter
    public:
        void set(T v) { container.fill(v); }

        void set(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33)
        {
            container[0 ] = m00;  container[1 ] = m01; container[2 ] = m02; container[3 ] = m03; 
            container[4 ] = m10;  container[5 ] = m11; container[6 ] = m12; container[7 ] = m13; 
            container[8 ] = m20;  container[9 ] = m21; container[10] = m22; container[11] = m23; 
            container[12] = m30;  container[13] = m31; container[14] = m32; container[15] = m33; 
        }

        template <typename T1>
        void set(const std::initializer_list<T1> &list)
        {
            for (auto iter = list.begin(); iter < list.end(); iter++)
            {
                container[iter - list.begin()] = *iter;
            }
        }

        template <typename T1>
        void set(const Mat44<T1> &m_)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] = T(m_(i));
            }
        }

        template <typename T1>
        void set_diag(T1 value)
        {
            container[0 ] = value;
            container[5 ] = value;
            container[10] = value;
            container[15] = value;
        }

        void setIdentity()
        {
            set(0);
            container[0 ] = 1;
            container[5 ] = 1;
            container[10] = 1;
            container[15] = 1;
        }

        void setZero()
        {
            container.fill(0);
        }

        void setRow(size_t i, const Vec3<T>& r)
        {
            container[i * 4 + 0] = r[0]; 
            container[i * 4 + 1] = r[1]; 
            container[i * 4 + 2] = r[2]; 
            container[i * 4 + 2] = r[3]; 
        }

        void setCol(size_t j, const Vec3<T>& c)
        {
            container[j + 4 * 0] = c[0];
            container[j + 4 * 1] = c[1];
            container[j + 4 * 2] = c[2];
            container[j + 4 * 2] = c[3];
        }

        T &operator()(size_t i, size_t j)
        {
            assert(i < 4 && j < 4);
            return container[i * 4 + j];
        }

        T operator()(size_t i, size_t j) const
        {
            assert(i < 4 && j < 4);
            return container[i * 4 + j];
        }

        T &operator()(size_t i)
        {
            assert(i < 16);
            return container[i];
        }

        T operator()(size_t i) const
        {
            assert(i < 16);
            return container[i];
        }

        // data
    public:
        std::array<T, 16> container;
    };

    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Mat44<T1> &m)
    {
        os << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << " " << m(0, 3) << std::endl;
        os << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << " " << m(1, 3) << std::endl;
        os << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << " " << m(2, 3) << std::endl;
        os << m(3, 0) << " " << m(3, 1) << " " << m(3, 2) << " " << m(3, 3) << std::endl;
        os << std::endl;

        return os;
    }

} // namespace HSim
