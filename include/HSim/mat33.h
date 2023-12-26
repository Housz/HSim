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
    public:
        Mat33() { container.fill(0); };
        ~Mat33(){};

        Mat33(T v) { set(v); }

        Mat33(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) { set(m00, m01, m02, m10, m11, m12, m20, m21, m22); }

        template <typename T1>
        Mat33(const std::initializer_list<T1> &list) { set(list); }

        // template <typename T1>
        // Mat33(const std::initializer_list<std::initializer_list<T1>> &list) { set(list); }

        template <typename T1>
        Mat33(Mat33<T1> &m) { set(m); }

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
                container[iter-list.begin()] = *iter;
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
            container = m_.container;
        }

        template <typename T1>
        void set_diag(T1 value)
        {
            container[0] = value;
            container[4] = value;
            container[8] = value;
        }

        T& operator()(size_t i, size_t j)
        {
            assert(i<3 && j <3);
            return container[i + j*3];
        }

        T& operator()(size_t i)
        {
            assert(i<3 && j <3);
            return container[i];
        }

    // operator
    public:

        // add n
        template <typename T1>
        Mat33<T> add(T1 value)
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) += value;
            }
            return m;
        }
        // add mat
        template <typename T1>
        Mat33<T> add(Mat33<T1> m_)
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m.container[i] += m_(i);
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
        void add(Mat33<T1> m_)
        {
            for (size_t i = 0; i < 9; i++)
            {
                container[i] += m_(i);
            }
        }

        template <typename T1>
        Mat33<T>& operator=(const Mat33<T1>& m_)
        {
            set(m_);
        }

        // mat + n
        template <typename T1>
        Mat33<T> operator+(T1 value)
        {
            return add(value);
        }
        // mat + mat
        template <typename T1>
        Mat33<T> operator+(Mat33<T1>& m_)
        {
            return add(m_);
        }
        // += n
        template <typename T1>
        Mat33<T>& operator+=(T1 value)
        {
            add_self(value);
            return *this;
        }
        // += mat
        template <typename T1>
        Mat33<T>& operator+=(Mat33<T1>& m_)
        {
            add_self(m_);
            return *this;
        }


        // sub
        // sub
        // subself
        // subself

        // mul n
        template <typename T1>
        Mat33<T> add(T1 value)
        {
            Mat33<T> m;
            for (size_t i = 0; i < 9; i++)
            {
                m(i) *= value;
            }
            return m;
        }

        
        // mul m
        // mul v3
        // v3 mul m33 in v3.h
        // mulself n 
        // mulself m

        // div
        // div
        // divself
        // divself


        // operator + n
        // operator + m
        // operator m1 + m2

        // operator - n
        // operator - m
        // operator m1 - m2


        // operator * v
        // operator * m
        // operator m1 * m2



        // data
    public:
        std::array<T, 9> container;
    };
    
    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Mat33<T1> &m)
    {
        os << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << std::endl;
        os << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << std::endl;
        os << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << std::endl;
        os << std::endl;

        return os;
    }

    // n * mat
    // template <typename T1, typename T2>
    // Mat33<T2> operator*(T1 value, Mat33<T2> &m_)
    // {
    //     return m_.mul(value);
    // }


    // mat * n
    // n * mat
    // mat * mat
    // v * mat
    // mat * v

    using Mat33i = Mat33<int>;
    using Mat33f = Mat33<float>;
    using Mat33d = Mat33<double>;

} // namespace HSim
