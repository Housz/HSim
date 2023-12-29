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
        Mat44(const Mat44<T1> &m) { set(m); }

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

        void setRow(size_t i, const Vec4<T>& r)
        {
            container[i * 4 + 0] = r[0]; 
            container[i * 4 + 1] = r[1]; 
            container[i * 4 + 2] = r[2]; 
            container[i * 4 + 3] = r[3]; 
        }

        void setCol(size_t j, const Vec4<T>& c)
        {
            container[j + 4 * 0] = c[0];
            container[j + 4 * 1] = c[1];
            container[j + 4 * 2] = c[2];
            container[j + 4 * 3] = c[3];
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

        // operator
    public:

        template <typename T1>
        Mat44<T> &operator=(const Mat44<T1> &m_)
        {
            set(m_);
            return *this;
        }

        // add n
        template <typename T1>
        Mat44<T> add(T1 value) const
        {
            Mat44<T> m;
            for (size_t i = 0; i < 16; i++)
            {
                m(i) = container[i] + T(value);
            }
            return m;
        }
        // add mat
        template <typename T1>
        Mat44<T> add(const Mat44<T1> &m_) const
        {
            Mat44<T> m;
            for (size_t i = 0; i < 16; i++)
            {
                m(i) = container[i] + m_(i);
            }
            return m;
        }
        // addself n
        template <typename T1>
        void add_self(T1 value)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] += value;
            }
        }
        // addself mat
        template <typename T1>
        void add_self(const Mat44<T1> &m_)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] += m_(i);
            }
        }

        // += n
        template <typename T1>
        Mat44<T> &operator+=(T1 value)
        {
            add_self(value);
            return *this;
        }
        // += mat
        template <typename T1>
        Mat44<T> &operator+=(const Mat44<T1> &m_)
        {
            add_self(m_);
            return *this;
        }

        // sub n
        template <typename T1>
        Mat44<T> sub(T1 value) const
        {
            Mat44<T> m;
            for (size_t i = 0; i < 16; i++)
            {
                m(i) = container[i] - T(value);
            }
            return m;
        }        
        // sub mat
        template <typename T1>
        Mat44<T> sub(const Mat44<T1> &m_) const
        {
            Mat44<T> m;
            for (size_t i = 0; i < 16; i++)
            {
                m(i) = container[i] - m_(i);
            }
            return m;
        }        
        // subself n
        template <typename T1>
        void sub_self(T1 value)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] -= value;
            }
        }
        // subself mat
        template <typename T1>
        void sub_self(const Mat44<T1> &m_)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] -= m_(i);
            }
        }

        // -= n
        template <typename T1>
        Mat44<T> &operator-=(T1 value)
        {
            sub_self(value);
            return *this;
        }   
        // -= mat
        template <typename T1>
        Mat44<T> &operator-=(const Mat44<T1> &m_)
        {
            sub_self(m_);
            return *this;
        }      

        Mat44<T> operator-() const
        {
            // return (*this) * -1;
            return mul(-1);
        }        

        // mul n
        template <typename T1>
        Mat44<T> mul(T1 value) const
        {
            Mat44<T> m;
            for (size_t i = 0; i < 16; i++)
            {
                m(i) = container[i] * value;
            }
            return m;
        }
        // mul mat
        template <typename T1>
        Mat44<T> mul(const Mat44<T1> &m_) const
        {
            Mat44<T> m(
                container[0] * m_.container[0] + container[1] * m_.container[4] + container[2] * m_.container[8] + container[3] * m_.container[12],
                container[0] * m_.container[1] + container[1] * m_.container[5] + container[2] * m_.container[9] + container[3] * m_.container[13],
                container[0] * m_.container[2] + container[1] * m_.container[6] + container[2] * m_.container[10] + container[3] * m_.container[14],
                container[0] * m_.container[3] + container[1] * m_.container[7] + container[2] * m_.container[11] + container[3] * m_.container[15],

                container[4] * m_.container[0] + container[5] * m_.container[4] + container[6] * m_.container[8] + container[7] * m_.container[12],
                container[4] * m_.container[1] + container[5] * m_.container[5] + container[6] * m_.container[9] + container[7] * m_.container[13],
                container[4] * m_.container[2] + container[5] * m_.container[6] + container[6] * m_.container[10] + container[7] * m_.container[14],
                container[4] * m_.container[3] + container[5] * m_.container[7] + container[6] * m_.container[11] + container[7] * m_.container[15],

                container[8] * m_.container[0] + container[9] * m_.container[4] + container[10] * m_.container[8] + container[11] * m_.container[12],
                container[8] * m_.container[1] + container[9] * m_.container[5] + container[10] * m_.container[9] + container[11] * m_.container[13],
                container[8] * m_.container[2] + container[9] * m_.container[6] + container[10] * m_.container[10] + container[11] * m_.container[14],
                container[8] * m_.container[3] + container[9] * m_.container[7] + container[10] * m_.container[11] + container[11] * m_.container[15],

                container[12] * m_.container[0] + container[13] * m_.container[4] + container[14] * m_.container[8] + container[15] * m_.container[12],
                container[12] * m_.container[1] + container[13] * m_.container[5] + container[14] * m_.container[9] + container[15] * m_.container[13],
                container[12] * m_.container[2] + container[13] * m_.container[6] + container[14] * m_.container[10] + container[15] * m_.container[14],
                container[12] * m_.container[3] + container[13] * m_.container[7] + container[14] * m_.container[11] + container[15] * m_.container[15]
                );
            return m;
        }
        // mulself n
        template <typename T1>
        void mul_self(T1 value)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] *= value;
            }
        }

        // mulself mat
        template <typename T1>
        void mul_self(const Mat44<T1> &m_)
        {
            this->set(
                container[0] * m_.container[0] + container[1] * m_.container[4] + container[2] * m_.container[8] + container[3] * m_.container[12],
                container[0] * m_.container[1] + container[1] * m_.container[5] + container[2] * m_.container[9] + container[3] * m_.container[13],
                container[0] * m_.container[2] + container[1] * m_.container[6] + container[2] * m_.container[10] + container[3] * m_.container[14],
                container[0] * m_.container[3] + container[1] * m_.container[7] + container[2] * m_.container[11] + container[3] * m_.container[15],

                container[4] * m_.container[0] + container[5] * m_.container[4] + container[6] * m_.container[8] + container[7] * m_.container[12],
                container[4] * m_.container[1] + container[5] * m_.container[5] + container[6] * m_.container[9] + container[7] * m_.container[13],
                container[4] * m_.container[2] + container[5] * m_.container[6] + container[6] * m_.container[10] + container[7] * m_.container[14],
                container[4] * m_.container[3] + container[5] * m_.container[7] + container[6] * m_.container[11] + container[7] * m_.container[15],

                container[8] * m_.container[0] + container[9] * m_.container[4] + container[10] * m_.container[8] + container[11] * m_.container[12],
                container[8] * m_.container[1] + container[9] * m_.container[5] + container[10] * m_.container[9] + container[11] * m_.container[13],
                container[8] * m_.container[2] + container[9] * m_.container[6] + container[10] * m_.container[10] + container[11] * m_.container[14],
                container[8] * m_.container[3] + container[9] * m_.container[7] + container[10] * m_.container[11] + container[11] * m_.container[15],

                container[12] * m_.container[0] + container[13] * m_.container[4] + container[14] * m_.container[8] + container[15] * m_.container[12],
                container[12] * m_.container[1] + container[13] * m_.container[5] + container[14] * m_.container[9] + container[15] * m_.container[13],
                container[12] * m_.container[2] + container[13] * m_.container[6] + container[14] * m_.container[10] + container[15] * m_.container[14],
                container[12] * m_.container[3] + container[13] * m_.container[7] + container[14] * m_.container[11] + container[15] * m_.container[15]
            );
        }
        // *= n
        template <typename T1>
        Mat44<T> &operator*=(T1 value)
        {
            mul_self(value);
            return *this;
        }
        // *= mat
        template <typename T1>
        Mat44<T> &operator*=(const Mat44<T1> &m_)
        {
            mul_self(m_);
            return *this;
        }

        // div n
        template <typename T1>
        Mat44<T> div(T1 value) const
        {
            Mat44<T> m;
            for (size_t i = 0; i < 16; i++)
            {
                m(i) = container[i] / T(value);
            }
            return m;
        }
        // divself n
        template <typename T1>
        void div_self(T1 value)
        {
            for (size_t i = 0; i < 16; i++)
            {
                container[i] /= value;
            }
        }
        // /= n
        template <typename T1>
        Mat44<T> &operator/=(T1 value)
        {
            div_self(value);
            return *this;
        }

        // m = m^-1
        void invert()
        {
            T det = determinant();

            assert(det != 0);

            Mat44<T> m;
            m(0 ) = container[5] * container[10] * container[15] + container[6] * container[11] * container[13] + container[7] * container[9 ] * container[14] - container[5] * container[11] * container[14] - container[6] * container[9 ] * container[15] - container[7] * container[10] * container[13];
            m(1 ) = container[1] * container[11] * container[14] + container[2] * container[9 ] * container[15] + container[3] * container[10] * container[13] - container[1] * container[10] * container[15] - container[2] * container[11] * container[13] - container[3] * container[9 ] * container[14];
            m(2 ) = container[1] * container[6 ] * container[15] + container[2] * container[7 ] * container[13] + container[3] * container[5 ] * container[14] - container[1] * container[7 ] * container[14] - container[2] * container[5 ] * container[15] - container[3] * container[6 ] * container[13];
            m(3 ) = container[1] * container[7 ] * container[10] + container[2] * container[5 ] * container[11] + container[3] * container[6 ] * container[9 ] - container[1] * container[6 ] * container[11] - container[2] * container[7 ] * container[9 ] - container[3] * container[5 ] * container[10];
            m(4 ) = container[4] * container[11] * container[14] + container[6] * container[8 ] * container[15] + container[7] * container[10] * container[12] - container[4] * container[10] * container[15] - container[6] * container[11] * container[12] - container[7] * container[8 ] * container[14];
            m(5 ) = container[0] * container[10] * container[15] + container[2] * container[11] * container[12] + container[3] * container[8 ] * container[14] - container[0] * container[11] * container[14] - container[2] * container[8 ] * container[15] - container[3] * container[10] * container[12];
            m(6 ) = container[0] * container[7 ] * container[14] + container[2] * container[4 ] * container[15] + container[3] * container[6 ] * container[12] - container[0] * container[6 ] * container[15] - container[2] * container[7 ] * container[12] - container[3] * container[4 ] * container[14];
            m(7 ) = container[0] * container[6 ] * container[11] + container[2] * container[7 ] * container[8 ] + container[3] * container[4 ] * container[10] - container[0] * container[7 ] * container[10] - container[2] * container[4 ] * container[11] - container[3] * container[6 ] * container[8 ];
            m(8 ) = container[4] * container[9 ] * container[15] + container[5] * container[11] * container[12] + container[7] * container[8 ] * container[13] - container[4] * container[11] * container[13] - container[5] * container[8 ] * container[15] - container[7] * container[9 ] * container[12];
            m(9 ) = container[0] * container[11] * container[13] + container[1] * container[8 ] * container[15] + container[3] * container[9 ] * container[12] - container[0] * container[9 ] * container[15] - container[1] * container[11] * container[12] - container[3] * container[8 ] * container[13];
            m(10) = container[0] * container[5 ] * container[15] + container[1] * container[7 ] * container[12] + container[3] * container[4 ] * container[13] - container[0] * container[7 ] * container[13] - container[1] * container[4 ] * container[15] - container[3] * container[5 ] * container[12];
            m(11) = container[0] * container[7 ] * container[9 ] + container[1] * container[4 ] * container[11] + container[3] * container[5 ] * container[8 ] - container[0] * container[5 ] * container[11] - container[1] * container[7 ] * container[8 ] - container[3] * container[4 ] * container[9 ];
            m(12) = container[4] * container[10] * container[13] + container[5] * container[8 ] * container[14] + container[6] * container[9 ] * container[12] - container[4] * container[9 ] * container[14] - container[5] * container[10] * container[12] - container[6] * container[8 ] * container[13];
            m(13) = container[0] * container[9 ] * container[14] + container[1] * container[10] * container[12] + container[2] * container[8 ] * container[13] - container[0] * container[10] * container[13] - container[1] * container[8 ] * container[14] - container[2] * container[9 ] * container[12];
            m(14) = container[0] * container[6 ] * container[13] + container[1] * container[4 ] * container[14] + container[2] * container[5 ] * container[12] - container[0] * container[5 ] * container[14] - container[1] * container[6 ] * container[12] - container[2] * container[4 ] * container[13];
            m(15) = container[0] * container[5 ] * container[10] + container[1] * container[6 ] * container[8 ] + container[2] * container[4 ] * container[9 ] - container[0] * container[6 ] * container[9 ] - container[1] * container[4 ] * container[10] - container[2] * container[5 ] * container[8 ];
            m /= det;

            set(m);
        }

        // m = m^T
        void transpose()
        {
            std::swap(container[1], container[4]);
            std::swap(container[2], container[8]);
            std::swap(container[3], container[12]);
            std::swap(container[6], container[9]);
            std::swap(container[7], container[13]);
            std::swap(container[11], container[14]);
        }

        T determinant() const
        {
            return 
                container[0] * container[5] * container[10] * container[15] + container[0] * container[6] * container[11] * container[13] + container[0] * container[7] * container[9 ] * container[14] + 
                container[1] * container[4] * container[11] * container[14] + container[1] * container[6] * container[8 ] * container[15] + container[1] * container[7] * container[10] * container[12] + 
                container[2] * container[4] * container[9 ] * container[15] + container[2] * container[5] * container[11] * container[12] + container[2] * container[7] * container[8 ] * container[13] + 
                container[3] * container[4] * container[10] * container[13] + container[3] * container[5] * container[8 ] * container[14] + container[3] * container[6] * container[9 ] * container[12] - 
                container[0] * container[5] * container[11] * container[14] - container[0] * container[6] * container[9 ] * container[15] - container[0] * container[7] * container[10] * container[13] - 
                container[1] * container[4] * container[10] * container[15] - container[1] * container[6] * container[11] * container[12] - container[1] * container[7] * container[8 ] * container[14] - 
                container[2] * container[4] * container[11] * container[13] - container[2] * container[5] * container[8 ] * container[15] - container[2] * container[7] * container[9 ] * container[12] - 
                container[3] * container[4] * container[9 ] * container[14] - container[3] * container[5] * container[10] * container[12] - container[3] * container[6] * container[8 ] * container[13];
        }

        T trace() const
        {
            return container[0] + container[5] + container[10] + container[15];
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

    // mat + mat
    template <typename T1, typename T2>
    Mat44<T1> operator+(const Mat44<T1> &m1, const Mat44<T2> &m2)
    {
        return m1.add(m2);
    }

    // mat + n
    template <typename T1, typename T2>
    Mat44<T1> operator+(const Mat44<T1> &m_, T2 value)
    {
        return m_.add(value);
    }
    // n + mat
    template <typename T1, typename T2>
    Mat44<T1> operator+(T1 value, const Mat44<T2> &m_)
    {
        return m_.add(value);
    }

    // mat - mat
    template <typename T1, typename T2>
    Mat44<T1> operator-(const Mat44<T1> &m1, const Mat44<T2> &m2)
    {
        return m1.sub(m2);
    }
    // mat - n
    template <typename T1, typename T2>
    Mat44<T1> operator-(const Mat44<T1> &m_, T2 value)
    {
        return m_.sub(value);
    }    
    // n - mat
    template <typename T1, typename T2>
    Mat44<T1> operator-(T1 value, const Mat44<T2> &m_)
    {
        // return (-1*m_).add(value);
        return -m_.add(value);
    }

    // mat / n
    template <typename T1, typename T2>
    Mat44<T1> operator/(const Mat44<T1> &m_, T2 value)
    {
        return m_.div(value);
    }  

    // mat * n
    template <typename T1, typename T2>
    Mat44<T1> operator*(const Mat44<T1> &m_, T2 value)
    {
        return m_.mul(value);
    }
    // n * mat
    template <typename T1, typename T2>
    Mat44<T2> operator*(T1 value, const Mat44<T2> &m_)
    {
        return m_.mul(value);
    }

    // mat * mat
    template <typename T1, typename T2>
    Mat44<T1> operator*(const Mat44<T1> &m1, const Mat44<T2> &m2)
    {
        return m1.mul(m2);
    }    

    template <typename T1, typename T2>
    Vec4<T2> operator*(const Mat44<T1> &m_, const Vec4<T2> &v_)
    {
        Vec4<T2> v
        (
            m_(0 )*v_(0) + m_(1 )*v_(1) + m_(2 )*v_(2) + m_(3 )*v_(3),
            m_(4 )*v_(0) + m_(5 )*v_(1) + m_(6 )*v_(2) + m_(7 )*v_(3),
            m_(8 )*v_(0) + m_(9 )*v_(1) + m_(10)*v_(2) + m_(11)*v_(3),
            m_(12)*v_(0) + m_(13)*v_(1) + m_(14)*v_(2) + m_(15)*v_(3)
        );

        return v;
    }

    template <typename T1, typename T2>
    Vec4<T2> operator*(const Vec4<T1> &v_, const Mat44<T2> &m_)
    {
        Vec4<T2> v
        (
            m_(0 )*v_(0) + m_(1 )*v_(1) + m_(2 )*v_(2) + m_(3 )*v_(3),
            m_(4 )*v_(0) + m_(5 )*v_(1) + m_(6 )*v_(2) + m_(7 )*v_(3),
            m_(8 )*v_(0) + m_(9 )*v_(1) + m_(10)*v_(2) + m_(11)*v_(3),
            m_(12)*v_(0) + m_(13)*v_(1) + m_(14)*v_(2) + m_(15)*v_(3)
        );

        return v;
    }

    using Mat44i = Mat44<int>;
    using Mat44f = Mat44<float>;
    using Mat44d = Mat44<double>;

} // namespace HSim
