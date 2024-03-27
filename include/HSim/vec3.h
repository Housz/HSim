#pragma once

#include <iostream>
#include <algorithm>
#include <HSim/common.h>
#include <HSim/vec4.h>

namespace HSim
{
    template <typename T>
    class Vec4;
    /**
     * @brief 3-D vector
     *
     * @tparam T : element type
     */
    template <typename T>
    class Vec3
    {
    public:
        T x;
        T y;
        T z;

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Vec3<T1> &);

        /**
         * @brief Construct a new Vec3 object
         *
         */
        Vec3() : x(0), y(0), z(0) {}
        Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
        template <typename U>
        Vec3(const Vec3<U> &v_) : x(v_.x), y(v_.y), z(v_.z) {}
        template <typename U>
        Vec3(const Vec4<U> &v_) : x(v_.x), y(v_.y), z(v_.z) {}
        template <typename U>
        Vec3(const std::initializer_list<U> &list) { set(list); }

        void set(T x_)
        {
            x = x_;
            y = x_;
            z = x_;
        }
        void set(T x_, T y_, T z_)
        {
            x = x_;
            y = y_;
            z = z_;
        }
        void set(const Vec3 &v_)
        {
            x = v_.x;
            y = v_.y;
            z = v_.z;
        }

        void set(const Vec4<T> &v_)
        {
            x = v_.x;
            y = v_.y;
            z = v_.z;
        }

        template <typename U>
        void set(const std::initializer_list<U> &list)
        {
            auto iter = list.begin();

            if (list.size() == 1)
            {
                x = static_cast<T>(*iter);
            }
            else if (list.size() == 2)
            {
                x = static_cast<T>(*iter);
                y = static_cast<T>(*(++iter));
            }
            else
            {
                x = static_cast<T>(*iter);
                y = static_cast<T>(*(++iter));
                z = static_cast<T>(*(++iter));
            }
        }

        T length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        void normalize()
        {
            T len = length();
            // assert(len > EPSILON);
            x /= len;
            y /= len;
            z /= len;
        }

        Vec3<T> getNormalized() const
        {
            T len = length();
            assert(len > EPSILON);
            return Vec3<T>(x / len, y / len, z / len);
        }

        bool isEqual(const Vec3<T> &v_) const
        {
            return (x == v_.x) && (y == v_.y) && (z == v_.z);
        }

        bool isSimilar(const Vec3<T> v_) const
        {
            return (std::fabs(x - v_.x) < EPSILON) &&
                   (std::fabs(y - v_.y) < EPSILON) &&
                   (std::fabs(z - v_.z) < EPSILON);
        }

        bool isZero() const
        {
            return (x == 0 && y == 0 && z == 0);
        }

        //// new_Vec3 = this_Vec3 (operator) parameters

        // add
        template <typename T1>
        Vec3<T> add(T1 a_) const { return Vec3<T>(x + a_, y + a_, z + a_); }
        template <typename T1>
        Vec3<T> add(const Vec3<T1> &v_) const { return Vec3<T>(x + v_.x, y + v_.y, z + v_.z); }
        template <typename T1>
        Vec3<T> add(const std::initializer_list<T1> &list_) const 
        {
            assert(list_.size() > 0);

            auto iter = list_.begin();

            if (list_.size() == 1)
            {
                T x_ = static_cast<T>(*iter);
                return Vec3<T>(x + x_, y, z);
            }
            else if (list_.size() == 2)
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                return Vec3<T>(x + x_, y + y_, z);
            }
            else
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                T z_ = static_cast<T>(*(++iter));
                return Vec3<T>(x + x_, y + y_, z + z_);
            }
        }

        // sub
        template <typename T1>
        Vec3<T> sub(T1 a_) const { return Vec3<T>(x - a_, y - a_, z - a_); }
        template <typename T1>
        Vec3<T> sub(const Vec3<T1> &v_) const { return Vec3<T>(x - v_.x, y - v_.y, z - v_.z); }
        template <typename T1>
        Vec3<T> sub(const std::initializer_list<T1> &list_) const 
        {
            assert(list_.size() > 0);

            auto iter = list_.begin();

            if (list_.size() == 1)
            {
                T x_ = static_cast<T>(*iter);
                return Vec3<T>(x - x_, y, z);
            }
            else if (list_.size() == 2)
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                return Vec3<T>(x - x_, y + y_, z);
            }
            else
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                T z_ = static_cast<T>(*(++iter));
                return Vec3<T>(x - x_, y - y_, z - z_);
            }
        }

        // mul
        template <typename T1>
        Vec3<T> mul(T1 a_) const { return Vec3<T>(x * a_, y * a_, z * a_); }
        template <typename T1>
        Vec3<T> mul(const Vec3<T1> &v_) const { return Vec3<T>(x * v_.x, y * v_.y, z * v_.z); }

        // div
        template <typename T1>
        Vec3<T> div(T1 a_) const { return Vec3<T>(x / a_, y / a_, z / a_); }
        template <typename T1>
        Vec3<T> div(const Vec3<T1> &v_) const { return Vec3<T>(x / v_.x, y / v_.y, z / v_.z); }

        // dot
        template <typename T1>
        T dot(const Vec3<T1> &v_) const { return x * v_.x + y * v_.y + z * v_.z; }

        // cross
        template <typename T1>
        Vec3<T> cross(const Vec3<T1> &v_) const { return Vec3<T>(y * v_.z - v_.y * z, z * v_.x - v_.z * x, x * v_.y - v_.x * y); }

        //// this_Vec3 = this_Vec3 (operator) parameters

        template <typename T1>
        void add_self(T1 a_)
        {
            x += a_;
            y += a_;
            z += a_;
        }
        template <typename T1>
        void add_self(const Vec3<T1> &v_)
        {
            x += v_.x;
            y += v_.y;
            z += v_.z;
        }
        template <typename T1>
        void add_self(const std::initializer_list<T1> &list_) const 
        {
            assert(list_.size() > 0);

            auto iter = list_.begin();

            if (list_.size() == 1)
            {
                T x_ = static_cast<T>(*iter);
                x + x_;
            }
            else if (list_.size() == 2)
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                x + x_;
                y + y_;
            }
            else
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                T z_ = static_cast<T>(*(++iter));
                x + x_;
                y + y_; 
                z + z_;
            }
        }

        template <typename T1>
        void sub_self(T1 a_)
        {
            x -= a_;
            y -= a_;
            z -= a_;
        }
        template <typename T1>
        void sub_self(const Vec3<T1> &v_)
        {
            x -= v_.x;
            y -= v_.y;
            z -= v_.z;
        }
        template <typename T1>
        void sub_self(const std::initializer_list<T1> &list_) const 
        {
            assert(list_.size() > 0);

            auto iter = list_.begin();

            if (list_.size() == 1)
            {
                T x_ = static_cast<T>(*iter);
                x - x_;
            }
            else if (list_.size() == 2)
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                x - x_;
                y - y_;
            }
            else
            {
                T x_ = static_cast<T>(*iter);
                T y_ = static_cast<T>(*(++iter));
                T z_ = static_cast<T>(*(++iter));
                x - x_;
                y - y_; 
                z - z_;
            }
        }

        template <typename T1>
        void mul_self(T1 a_)
        {
            x *= a_;
            y *= a_;
            z *= a_;
        }
        template <typename T1>
        void mul_self(const Vec3<T1> &v_)
        {
            x *= v_.x;
            y *= v_.y;
            z *= v_.z;
        }

        template <typename T1>
        void div_self(T1 a_)
        {
            x /= a_;
            y /= a_;
            z /= a_;
        }
        template <typename T1>
        void div_self(const Vec3<T1> &v_)
        {
            x /= v_.x;
            y /= v_.y;
            z /= v_.z;
        }

        //// Operators

        T &operator[](size_t i)
        {
            assert(i < 3);
            return (&x)[i];
        }

        T operator[](size_t i) const
        {
            assert(i < 3);
            return (&x)[i];
        }

        T &operator()(size_t i)
        {
            assert(i < 3);
            return (&x)[i];
        }

        T operator()(size_t i) const
        {
            assert(i < 3);
            return (&x)[i];
        }

        Vec3 &operator=(const Vec3<T> &v_)
        {
            set(v_);
            return (*this);
        }

        template <typename U>
        Vec3 &operator=(const std::initializer_list<U> &list_)
        {
            set(list_);
            return (*this);
        }

        template <typename T1>
        Vec3 operator+(const Vec3<T1> &v_) const { return add(v_); }
        template <typename T1>
        Vec3 operator+(const std::initializer_list<T1> &list_) const { return add(list_); }

        template <typename T1>
        Vec3 operator-(const Vec3<T1> &v_) const { return sub(v_); }
        template <typename T1>
        Vec3 operator-(const std::initializer_list<T1> &list_) const { return sub(list_); }
        Vec3 operator-() const { return mul(-1); }

        template <typename T1>
        Vec3 operator*(const Vec3<T1> &v_) const { return mul(v_); }
        template <typename T1>
        Vec3 operator*(T1 a_) const { return mul(a_); }

        template <typename T1>
        Vec3 operator/(const Vec3<T1> &v_) const { return div(v_); }
        template <typename T1>
        Vec3 operator/(T1 a_) const { return div(a_); }

        template <typename T1>
        Vec3 &operator+=(const Vec3<T1> &v_)
        {
            add_self(v_);
            return (*this);
        }
        template <typename T1>
        Vec3 &operator+=(const std::initializer_list<T1> &list_)
        {
            add_self(list_);
            return (*this);
        }

        template <typename T1>
        Vec3 &operator-=(const Vec3<T1> &v_)
        {
            sub_self(v_);
            return (*this);
        }
        template <typename T1>
        Vec3 &operator-=(const std::initializer_list<T1> &list_)
        {
            sub_self(list_);
            return (*this);
        }

        template <typename T1>
        Vec3 &operator*=(const Vec3<T1> &v_)
        {
            mul_self(v_);
            return (*this);
        }

        template <typename T1>
        Vec3 &operator/=(const Vec3<T1> &v_)
        {
            div_self(v_);
            return (*this);
        }

        bool operator==(const Vec3<T> &v_) const { return isEqual(v_); }

        bool operator!=(const Vec3<T> &v_) const { return !isEqual(v_); }

        T distanceTo(const Vec3<T> &v_) const
        {
            return sub(v_).length();
        }

    }; // class Vec3
    
    using Vec3i = Vec3<int>;
    using Vec3f = Vec3<float>;
    using Vec3d = Vec3<double>;
    
    using Vec3ui = Vec3<size_t>;
    using Size3 = Vec3<int>;

    template <typename T1>
    inline std::ostream &operator<<(std::ostream &os, Vec3<T1> &v_)
    {
        os << "x: " << v_.x << " y: " << v_.y << " z: " << v_.z << std::endl;
        return os;
    }

    template <typename T1>
    inline std::ostream &operator<<(std::ostream &os, const Vec3<T1> &v_)
    {
        os << "x: " << v_.x << " y: " << v_.y << " z: " << v_.z << std::endl;
        return os;
    }

    // n * v
    template <typename T1, typename T2>
    inline Vec3<T1> operator*(T1 n, const Vec3<T2> &v)
    {
        return v.mul((T1)n);
    }

    // {initializer_list} + v
    template <typename T1, typename T2>
    inline Vec3<T1> operator+(const std::initializer_list<T1> &list_, const Vec3<T2> &v)
    {
        return v.add(list_);
    }

    // {initializer_list} - v
    template <typename T1, typename T2>
    inline Vec3<T1> operator-(const std::initializer_list<T1> &list_, const Vec3<T2> &v)
    {
        return (-v).add(list_);
    }

} // namespace HSim