#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>

namespace HSim
{
    /**
     * @brief 3x3 matrix
     *
     * row-major
     *
     * 0  1  2     00 01 02
     * 3  4  5     10 11 12
     * 6  7  8     20 21 22
     *
     *
     * @tparam T
     */
    template <typename T>
    class Mat33
    {
    // constructor
    public:
        Mat33() { container.fill(0); };
        ~Mat33() {};

        Mat33(T v) { set(v); }

        Mat33(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) { set(m00, m01, m02, m10, m11, m12, m20, m21, m22); }

        template <typename T1>
        Mat33(const std::initializer_list<T1> &list) { set(list); }

        // template <typename T1>
        // Mat33(const std::initializer_list<std::initializer_list<T1>> &list) { set(list); }

        template <typename T1>
        Mat33(const Mat33<T1> &m) { set(m); }

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Mat33<T1> &);

    // setter, getter
    public:
        void set(T v) { container.fill(v); }
        void set(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
        {
            container[0] = m00; container[1] = m01; container[2] = m02;
            container[3] = m10; container[4] = m11; container[5] = m12;
            container[6] = m20; container[7] = m21; container[8] = m22;
        }

        template <typename T1>
        void set(const std::initializer_list<T1> &list)
        {
            for (auto iter = list.begin(); iter < list.end(); iter++)
            {
                container[iter - list.begin()] = *iter;
            }
        }

        // template <typename T1>
        // void set(const std::initializer_list<std::initializer_list<T1>> &list)
        // {
        //     for (auto iter = list.begin(); iter < list.end(); iter++)
        //     {
        //         container[iter-list.begin()] = *iter;
        //     }
        // }

        template <typename T1>
        void set(const Mat33<T1> &m_)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] = T(m_(i));
            }
        }

        template <typename T1>
        void setDiag(T1 value)
        {
            container[0] = value;
            container[4] = value;
            container[8] = value;
        }

        void setIdentity()
        {
            set(0);
            container[0] = 1;
            container[4] = 1;
            container[8] = 1;
        }

        void setZero()
        {
            container.fill(0);
        }

        void setRow(size_t i, const Vec3<T>& r)
        {
            container[i * 3 + 0] = r[0]; 
            container[i * 3 + 1] = r[1]; 
            container[i * 3 + 2] = r[2]; 
        }

        void setCol(size_t j, const Vec3<T>& c)
        {
            container[j + 3 * 0] = c[0];
            container[j + 3 * 1] = c[1];
            container[j + 3 * 2] = c[2];
        }

        T &operator()(size_t i, size_t j)
        {
            assert(i < 3 && j < 3);
            return container[i * 3 + j];
        }

        T operator()(size_t i, size_t j) const
        {
            assert(i < 3 && j < 3);
            return container[i * 3 + j];
        }

        T &operator()(size_t i)
        {
            assert(i < 9);
            return container[i];
        }

        T operator()(size_t i) const
        {
            assert(i < 9);
            return container[i];
        }

        // operator
    public:

        template <typename T1>
        Mat33<T> &operator=(const Mat33<T1> &m_)
        {
            set(m_);
            return *this;
        }

        // add n
        template <typename T1>
        Mat33<T> add(T1 value) const
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) = container[i] + T(value);
            }
            return m;
        }
        // add mat
        template <typename T1>
        Mat33<T> add(const Mat33<T1> &m_) const
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) = container[i] + m_(i);
            }
            return m;
        }
        // addself n
        template <typename T1>
        void add_self(T1 value)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] += value;
            }
        }
        // addself mat
        template <typename T1>
        void add_self(const Mat33<T1> &m_)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] += m_(i);
            }
        }

        // += n
        template <typename T1>
        Mat33<T> &operator+=(T1 value)
        {
            add_self(value);
            return *this;
        }
        // += mat
        template <typename T1>
        Mat33<T> &operator+=(const Mat33<T1> &m_)
        {
            add_self(m_);
            return *this;
        }

        // sub n
        template <typename T1>
        Mat33<T> sub(T1 value) const
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) = container[i] - T(value);
            }
            return m;
        }
        // sub mat
        template <typename T1>
        Mat33<T> sub(const Mat33<T1> &m_) const
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) = container[i] - m_(i);
            }
            return m;
        }
        // subself n
        template <typename T1>
        void sub_self(T1 value)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] -= value;
            }
        }
        // subself mat
        template <typename T1>
        void sub_self(const Mat33<T1> &m_)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] -= m_(i);
            }
        }

        // -= n
        template <typename T1>
        Mat33<T> &operator-=(T1 value)
        {
            sub_self(value);
            return *this;
        }
        // -= mat
        template <typename T1>
        Mat33<T> &operator-=(const Mat33<T1> &m_)
        {
            sub_self(m_);
            return *this;
        }
        
        Mat33<T> operator-() const
        {
            // return (*this) * -1;
            return mul(-1);
        }


        // mul n
        template <typename T1>
        Mat33<T> mul(T1 value) const
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) = container[i] * value;
            }
            return m;
        }
        // mul mat
        template <typename T1>
        Mat33<T> mul(const Mat33<T1> &m_) const
        {
            Mat33<T> m(
                container[0] * m_.container[0] + container[1] * m_.container[3] + container[2] * m_.container[6],
                container[0] * m_.container[1] + container[1] * m_.container[4] + container[2] * m_.container[7],
                container[0] * m_.container[2] + container[1] * m_.container[5] + container[2] * m_.container[8],

                container[3] * m_.container[0] + container[4] * m_.container[3] + container[5] * m_.container[6],
                container[3] * m_.container[1] + container[4] * m_.container[4] + container[5] * m_.container[7],
                container[3] * m_.container[2] + container[4] * m_.container[5] + container[5] * m_.container[8],

                container[6] * m_.container[0] + container[7] * m_.container[3] + container[8] * m_.container[6],
                container[6] * m_.container[1] + container[7] * m_.container[4] + container[8] * m_.container[7],
                container[6] * m_.container[2] + container[7] * m_.container[5] + container[8] * m_.container[8]
                );
            return m;
        }
        // mulself n
        template <typename T1>
        void mul_self(T1 value)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] *= value;
            }
        }

        // mulself mat
        template <typename T1>
        void mul_self(const Mat33<T1> &m_)
        {
            this->set(
            container[0] * m_.container[0] + container[1] * m_.container[3] + container[2] * m_.container[6],
            container[0] * m_.container[1] + container[1] * m_.container[4] + container[2] * m_.container[7],
            container[0] * m_.container[2] + container[1] * m_.container[5] + container[2] * m_.container[8],

            container[3] * m_.container[0] + container[4] * m_.container[3] + container[5] * m_.container[6],
            container[3] * m_.container[1] + container[4] * m_.container[4] + container[5] * m_.container[7],
            container[3] * m_.container[2] + container[4] * m_.container[5] + container[5] * m_.container[8],

            container[6] * m_.container[0] + container[7] * m_.container[3] + container[8] * m_.container[6],
            container[6] * m_.container[1] + container[7] * m_.container[4] + container[8] * m_.container[7],
            container[6] * m_.container[2] + container[7] * m_.container[5] + container[8] * m_.container[8]
            );
        }
        // *= n
        template <typename T1>
        Mat33<T> &operator*=(T1 value)
        {
            mul_self(value);
            return *this;
        }
        // *= mat
        template <typename T1>
        Mat33<T> &operator*=(const Mat33<T1> &m_)
        {
            mul_self(m_);
            return *this;
        }

        // div n
        template <typename T1>
        Mat33<T> div(T1 value) const
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) = container[i] / T(value);
            }
            return m;
        }
        // divself n
        template <typename T1>
        void div_self(T1 value)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] /= value;
            }
        }
        // /= n
        template <typename T1>
        Mat33<T> &operator/=(T1 value)
        {
            div_self(value);
            return *this;
        }


        // m = m^-1
        void invert()
        {
            T det = determinant();

            assert(det != 0);
            
            Mat33<T> m;
            m(0) = container[4] * container[8] - container[5] * container[7];
            m(1) = container[2] * container[7] - container[1] * container[8];
            m(2) = container[1] * container[5] - container[2] * container[4];
            m(3) = container[5] * container[6] - container[3] * container[8];
            m(4) = container[0] * container[8] - container[2] * container[6];
            m(5) = container[2] * container[3] - container[0] * container[5];
            m(6) = container[3] * container[7] - container[4] * container[6];
            m(7) = container[1] * container[6] - container[0] * container[7];
            m(8) = container[0] * container[4] - container[1] * container[3];
            m /= det;

            set(m);
        }

        // m = m^T
        void transpose()
        {
            std::swap(container[1], container[3]);
            std::swap(container[2], container[6]);
            std::swap(container[5], container[7]);
        }

        T determinant() const
        {
            return container[0] * container[4] * container[8] -
                   container[0] * container[5] * container[7] +
                   container[1] * container[5] * container[6] -
                   container[1] * container[3] * container[8] +
                   container[2] * container[3] * container[7] -
                   container[2] * container[4] * container[6];
        }

        T trace() const
        {
            return container[0] + container[4] + container[8];
        }

        // data
    public:
        std::array<T, 9> container;
    };

    template <typename T1>
    inline std::ostream &operator<<(std::ostream &os, Mat33<T1> &m)
    {
        os << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << std::endl;
        os << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << std::endl;
        os << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << std::endl;
        os << std::endl;

        return os;
    }

    // mat + mat
    template <typename T1, typename T2>
    inline Mat33<T1> operator+(const Mat33<T1> &m1, const Mat33<T2> &m2)
    {
        return m1.add(m2);
    }
    // mat + n
    template <typename T1, typename T2>
    inline Mat33<T1> operator+(const Mat33<T1> &m_, T2 value)
    {
        return m_.add(value);
    }
    // n + mat
    template <typename T1, typename T2>
    inline Mat33<T1> operator+(T1 value, const Mat33<T2> &m_)
    {
        return m_.add(value);
    }

    // mat - mat
    template <typename T1, typename T2>
    inline Mat33<T1> operator-(const Mat33<T1> &m1, const Mat33<T2> &m2)
    {
        return m1.sub(m2);
    }
    // mat - n
    template <typename T1, typename T2>
    inline Mat33<T1> operator-(const Mat33<T1> &m_, T2 value)
    {
        return m_.sub(value);
    }
    // n - mat
    template <typename T1, typename T2>
    inline Mat33<T1> operator-(T1 value, const Mat33<T2> &m_)
    {
        // return (-1*m_).add(value);
        return -m_.add(value);
    }

    // mat / n
    template <typename T1, typename T2>
    inline Mat33<T1> operator/(const Mat33<T1> &m_, T2 value)
    {
        return m_.div(value);
    }

    // mat * n
    template <typename T1, typename T2>
    inline Mat33<T1> operator*(const Mat33<T1> &m_, T2 value)
    {
        return m_.mul(value);
    }
    // n * mat
    template <typename T1, typename T2>
    inline Mat33<T2> operator*(T1 value, const Mat33<T2> &m_)
    {
        return m_.mul(value);
    }

    // mat * mat
    template <typename T1, typename T2>
    inline Mat33<T1> operator*(const Mat33<T1> &m1, const Mat33<T2> &m2)
    {
        return m1.mul(m2);
    }

    template <typename T1, typename T2>
    inline Vec3<T2> operator*(const Mat33<T1> &m_, const Vec3<T2> &v_)
    {
        Vec3<T2> v
        (
            m_(0)*v_(0) + m_(1)*v_(1) + m_(2)*v_(2),
            m_(3)*v_(0) + m_(4)*v_(1) + m_(5)*v_(2),
            m_(6)*v_(0) + m_(7)*v_(1) + m_(8)*v_(2)
        );

        return v;
    }

    template <typename T1, typename T2>
    inline Vec3<T2> operator*(const Vec3<T1> &v_, const Mat33<T2> &m_)
    {
        Vec3<T2> v
        (
            m_(0)*v_(0) + m_(1)*v_(1) + m_(2)*v_(2),
            m_(3)*v_(0) + m_(4)*v_(1) + m_(5)*v_(2),
            m_(6)*v_(0) + m_(7)*v_(1) + m_(8)*v_(2)
        );

        return v;
    }

    using Mat33i = Mat33<int>;
    using Mat33f = Mat33<float>;
    using Mat33d = Mat33<double>;

} // namespace HSim
