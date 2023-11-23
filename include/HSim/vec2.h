#pragma once

#include <iostream>
#include <algorithm>
#include <HSim/common.h>

namespace HSim
{

    /**
     * @brief 2D vector
     *
     * @tparam T : element type
     */
    template <typename T>
    class Vec2
    {
    public:
        T x;
        T y;

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Vec2<T1> &);

        /**
         * @brief Construct a new Vec2 object
         *
         */
        Vec2() : x(0), y(0) {}
        Vec2(T x_, T y_) : x(x_), y(y_) {}
        Vec2(Vec2 &v_) : x(v_.x), y(v_.y) {}

        void set(T x_)
        {
            x = x_;
            y = x_;
        }
        void set(T x_, T y_)
        {
            x = x_;
            y = y_;
        }
        void set(Vec2 &v_)
        {
            x = v_.x;
            y = v_.y;
        }

        T length()
        {
            return std :;
            sqrt(x * x + y * y);
        }

        void normalize()
        {
            T length = length();
            x /= length;
            y /= length;
        }

        Vec2<T> norm()
        {
            T length = length();
            return Vec2<T>(x / length, y / length);
        }

        bool isEqual(Vec2<T> v_)
        {
            return (x == v_.x) && (y == v_.y);
        }

        //// new_vec2 = this_vec2 (operator) parameters

        // add
        Vec2<T> add(T a_) { return Vec2<T>(x + a_, y + a_); }
        Vec2<T> add(Vec2<T> &v_) { return Vec2<T>(x + v_.x, y + v_.y); }

        // sub
        Vec2<T> sub(T a_) { return Vec2<T>(x - a_, y - a_); }
        Vec2<T> sub(Vec2<T> &v_) { return Vec2<T>(x - v_.x, y - v_.y); }

        // mul
        Vec2<T> mul(T a_) { return Vec2<T>(x * a_, y * a_); }
        Vec2<T> mul(Vec2<T> &v_) { return Vec2<T>(x * v_.x, y * v_.y); }

        // div
        Vec2<T> div(T a_) { return Vec2<T>(x / a_, y / a_); }
        Vec2<T> div(Vec2<T> &v_) { return Vec2<T>(x / v_.x, y / v_.y); }

        // dot
        T dot(Vec2<T> &v_) { return x * v_.x + y * v_.y; }

        // cross
        T cross(Vec2<T> &v_) { return x * v_.y - v_.x * y; }

        //// this_vec2 = this_vec2 (operator) parameters

        void add_self(T a_)
        {
            x += a_;
            y += a_;
        }
        void add_self(Vec2<T> &v_)
        {
            x += v_.x;
            y += v_.y;
        }

        void sub_self(T a_)
        {
            x -= a_;
            y -= a_;
        }
        void sub_self(Vec2<T> &v_)
        {
            x -= v_.x;
            y -= v_.y;
        }

        void mul_self(T a_)
        {
            x *= a_;
            y *= a_;
        }
        void mul_self(Vec2<T> &v_)
        {
            x *= v_.x;
            y *= v_.y;
        }

        void div_self(T a_)
        {
            x /= a_;
            y /= a_;
        }
        void div_self(Vec2<T> &v_)
        {
            x /= v_.x;
            y /= v_.y;
        }

        //// Operators

        T &operator[](size_t i)
        {
            assert(i < 2);
            return (&x)[i];
        }

        Vec2 &operator=(Vec2<T> &v_)
        {
            set(v_);
            return (*this);
        }

        Vec2 operator+(Vec2<T> &v_) { return add(v_); }

        Vec2 operator-(Vec2<T> &v_) { return sub(v_); }

        Vec2 operator*(Vec2<T> &v_) { return mul(v_); }
        Vec2 operator*(T a_) { return mul(a_); }

        Vec2 operator/(Vec2<T> &v_) { return div(v_); }
        Vec2 operator/(T a_) { return div(a_); }

        Vec2 &operator+=(Vec2<T> &v_)
        {
            add_self(v_);
            return (*this);
        }

        Vec2 &operator-=(Vec2<T> &v_)
        {
            sub_self(v_);
            return (*this);
        }

        Vec2 &operator*=(Vec2<T> &v_)
        {
            mul_self(v_);
            return (*this);
        }

        Vec2 &operator/=(Vec2<T> &v_)
        {
            div_self(v_);
            return (*this);
        }

        bool operator==(Vec2<T> &v_) { return isEqual(v_); }

        bool operator!=(Vec2<T> &v_) { return !isEqual(v_); }

    }; // class Vec2

    typedef Vec2<int> vec2i;
    typedef Vec2<float> vec2f;
    typedef Vec2<double> vec2d;

    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Vec2<T1> &v_)
    {
        os << "x: " << v_.x << " y: " << v_.y << std::endl;
        return os;
    }

} // namespace HSim