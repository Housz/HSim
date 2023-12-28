#pragma once

#include <iostream>
#include <algorithm>
#include <HSim/common.h>

namespace HSim
{

    /**
     * @brief 2-D vector
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
        template <typename U>
        Vec2(const std::initializer_list<U> &list) { set(list); }

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
        void set(const Vec2 &v_)
        {
            x = v_.x;
            y = v_.y;
        }
        template <typename U>
        void set(const std::initializer_list<U> &list)
        {

            assert(list.size() >= 2);

            auto inputElem = list.begin();
            x = static_cast<T>(*inputElem);
            y = static_cast<T>(*(++inputElem));
        }

        T length() const
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

        Vec2<T> getNormalized() const
        {
            T length = length();
            return Vec2<T>(x / length, y / length);
        }

        bool isEqual(const Vec2<T>& v_)
        {
            return (x == v_.x) && (y == v_.y);
        }

        bool isZero() const
        {
            return (x==0 && y==0);
        }

        //// new_vec2 = this_vec2 (operator) parameters

        // add
        Vec2<T> add(T a_) const { return Vec2<T>(x + a_, y + a_); }
        Vec2<T> add(const Vec2<T> &v_) const  { return Vec2<T>(x + v_.x, y + v_.y); }

        // sub
        Vec2<T> sub(T a_) const { return Vec2<T>(x - a_, y - a_); }
        Vec2<T> sub(const Vec2<T> &v_) const  { return Vec2<T>(x - v_.x, y - v_.y); }

        // mul
        Vec2<T> mul(T a_) const { return Vec2<T>(x * a_, y * a_); }
        Vec2<T> mul(const Vec2<T> &v_) const { return Vec2<T>(x * v_.x, y * v_.y); }

        // div
        Vec2<T> div(T a_) const { return Vec2<T>(x / a_, y / a_); }
        Vec2<T> div(const Vec2<T> &v_) const { return Vec2<T>(x / v_.x, y / v_.y); }

        // dot
        T dot(const Vec2<T> &v_) const { return x * v_.x + y * v_.y; }

        // cross
        T cross(const Vec2<T> &v_) const { return x * v_.y - v_.x * y; }

        //// this_vec2 = this_vec2 (operator) parameters

        void add_self(T a_)
        {
            x += a_;
            y += a_;
        }
        void add_self(const Vec2<T> &v_)
        {
            x += v_.x;
            y += v_.y;
        }

        void sub_self(T a_)
        {
            x -= a_;
            y -= a_;
        }
        void sub_self(const Vec2<T> &v_)
        {
            x -= v_.x;
            y -= v_.y;
        }

        void mul_self(T a_)
        {
            x *= a_;
            y *= a_;
        }
        void mul_self(const Vec2<T> &v_)
        {
            x *= v_.x;
            y *= v_.y;
        }

        void div_self(T a_)
        {
            x /= a_;
            y /= a_;
        }
        void div_self(const Vec2<T> &v_)
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

        T operator[](size_t i) const
        {
            assert(i < 2);
            return (&x)[i];
        }

        T &operator()(size_t i)
        {
            assert(i < 2);
            return (&x)[i];
        }

        T operator()(size_t i) const
        {
            assert(i < 2);
            return (&x)[i];
        }

        Vec2 &operator=(Vec2<T> &v_)
        {
            set(v_);
            return (*this);
        }
        
        template <typename U>
        Vec2 &operator=(const std::initializer_list<U>& list)
        {
            set(list);
            return (*this);
        }


        Vec2 operator+(const Vec2<T> &v_) const { return add(v_); }

        Vec2 operator-(const Vec2<T> &v_) const { return sub(v_); }
        Vec2 operator-() const { return mul(-1); }

        Vec2 operator*(const Vec2<T> &v_) const { return mul(v_); }
        Vec2 operator*(T a_) { return mul(a_); }

        Vec2 operator/(const Vec2<T> &v_) const { return div(v_); }
        Vec2 operator/(T a_) const { return div(a_); }

        Vec2 &operator+=(const Vec2<T> &v_)
        {
            add_self(v_);
            return (*this);
        }

        Vec2 &operator-=(const Vec2<T> &v_)
        {
            sub_self(v_);
            return (*this);
        }

        Vec2 &operator*=(const Vec2<T> &v_)
        {
            mul_self(v_);
            return (*this);
        }

        Vec2 &operator/=(const Vec2<T> &v_)
        {
            div_self(v_);
            return (*this);
        }

        bool operator==(Vec2<T> &v_) { return isEqual(v_); }

        bool operator!=(Vec2<T> &v_) { return !isEqual(v_); }

    }; // class Vec2

    using Vec2i = Vec2<int>;
    using Vec2f = Vec2<float>;
    using Vec2d = Vec2<double>;

    using Size2 = Vec2<int>;
    
    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Vec2<T1> &v_)
    {
        os << "x: " << v_.x << " y: " << v_.y << std::endl;
        return os;
    }

    // n * v
    template <typename T1, typename T2>
    Vec2<T1> operator*(T2 n, Vec2<T1>& v)
    {
        return v.mul((T1)n);
    }
    

} // namespace HSim