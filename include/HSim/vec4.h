#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>

namespace HSim
{

    /**
     * @brief 3-D vector
     *
     * @tparam T : element type
     */
    template <typename T>
    class Vec4
    {
    public:
        T x;
        T y;
        T z;
        T w;

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Vec4<T1> &);

        /**
         * @brief Construct a new Vec4 object
         *
         */
        Vec4() : x(0), y(0), z(0), w(0) {}
        Vec4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
        Vec4(const Vec4 &v_) : x(v_.x), y(v_.y), z(v_.z), w(v_.w) {}
        Vec4(const Vec3<T> &v_, T w_) { set(v_, w_); }
        Vec4(const Vec3<T> &v_) : x(0), y(0), z(0), w(0) { set(v_); }
        template <typename U>
        Vec4(const std::initializer_list<U> &list) { set(list); }

        void set(T x_)
        {
            x = x_;
            y = x_;
            z = x_;
            w = x_;
        }
        void set(T x_, T y_, T z_, T w_)
        {
            x = x_;
            y = y_;
            z = z_;
            w = w_;
        }
        void set(const Vec4 &v_)
        {
            x = v_.x;
            y = v_.y;
            z = v_.z;
            w = v_.w;
        }

        void set(const Vec3<T> &v_, T w_)
        {
            x = v_.x;
            y = v_.y;
            z = v_.z;
            w = w_;
        }

        void set(const Vec3<T> &v_)
        {
            x = v_.x;
            y = v_.y;
            z = v_.z;
        }

        template <typename U>
        void set(const std::initializer_list<U> &list)
        {
            assert(list.size() <= 4);

            auto inputElem = list.begin();
            if (list.size() == 1)
            {
                x = static_cast<T>(*inputElem);
            }
            else if (list.size() == 2)
            {
                x = static_cast<T>(*inputElem);
                y = static_cast<T>(*(++inputElem));
            }
            else if (list.size() == 3)
            {
                x = static_cast<T>(*inputElem);
                y = static_cast<T>(*(++inputElem));
                z = static_cast<T>(*(++inputElem));
            }
            else if (list.size() == 4)
            {
                x = static_cast<T>(*inputElem);
                y = static_cast<T>(*(++inputElem));
                z = static_cast<T>(*(++inputElem));
                w = static_cast<T>(*(++inputElem));
            }
        }

        T length()
        {
            return std :;
            sqrt(x * x + y * y + z * z + w * w);
        }

        void normalize()
        {
            T length = length();
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }

        Vec4<T> norm()
        {
            T length = length();
            return Vec4<T>(x / length, y / length, z / length, w / length);
        }

        bool isEqual(Vec4<T> v_)
        {
            return (x == v_.x) && (y == v_.y) && (z == v_.z) && (w == v_.w);
        }

        bool isZero()
        {
            return (x == 0 && y == 0 && z == 0 && w == 0);
        }

        //// new_Vec3 = this_Vec3 (operator) parameters

        // add
        Vec4<T> add(T a_) const { return Vec4<T>(x + a_, y + a_, z + a_, w + a_); }
        Vec4<T> add(const Vec4<T> &v_) const { return Vec4<T>(x + v_.x, y + v_.y, z + v_.z, w + v_.w); }

        // sub
        Vec4<T> sub(T a_) const { return Vec4<T>(x - a_, y - a_, z - a_, w - a_); }
        Vec4<T> sub(const Vec4<T> &v_) const { return Vec4<T>(x - v_.x, y - v_.y, z - v_.z, w - v_.w); }

        // mul
        Vec4<T> mul(T a_) const { return Vec4<T>(x * a_, y * a_, z * a_, w * a_); }
        Vec4<T> mul(const Vec4<T> &v_) const { return Vec4<T>(x * v_.x, y * v_.y, z * v_.z, w * v_.w); }

        // div
        Vec4<T> div(T a_) const { return Vec4<T>(x / a_, y / a_, z / a_, w / a_); }
        Vec4<T> div(const Vec4<T> &v_) const { return Vec4<T>(x / v_.x, y / v_.y, z / v_.z, w / v_.w); }

        // dot
        T dot(const Vec4<T> &v_) const { return x * v_.x + y * v_.y + z * v_.z + w * v_.w; }

        // cross
        // T cross(const Vec3<T> &v_) const { return y * v_.z - v_.y * z, z * v_.x - v_.z * x, x * v_.y - v_.x * y; }

        //// this_Vec3 = this_Vec3 (operator) parameters

        void add_self(T a_)
        {
            x += a_;
            y += a_;
            z += a_;
            w += a_;
        }
        void add_self(const Vec4<T> &v_)
        {
            x += v_.x;
            y += v_.y;
            z += v_.z;
            w += v_.w;
        }

        void sub_self(T a_)
        {
            x -= a_;
            y -= a_;
            z -= a_;
            w -= a_;
        }
        void sub_self(const Vec4<T> &v_)
        {
            x -= v_.x;
            y -= v_.y;
            z -= v_.z;
            w -= v_.w;
        }

        void mul_self(T a_)
        {
            x *= a_;
            y *= a_;
            z *= a_;
            w *= a_;
        }

        void mul_self(const Vec4<T> &v_)
        {
            x *= v_.x;
            y *= v_.y;
            z *= v_.z;
            w *= v_.w;
        }

        void div_self(T a_)
        {
            x /= a_;
            y /= a_;
            z /= a_;
            w /= a_;
        }
        void div_self(const Vec4<T> &v_)
        {
            x /= v_.x;
            y /= v_.y;
            z /= v_.z;
            w /= v_.w;
        }

        //// Operators

        T &operator[](size_t i)
        {
            assert(i < 4);
            return (&x)[i];
        }

        T operator[](size_t i) const
        {
            assert(i < 4);
            return (&x)[i];
        }

        T &operator()(size_t i)
        {
            assert(i < 4);
            return (&x)[i];
        }

        T operator()(size_t i) const
        {
            assert(i < 4);
            return (&x)[i];
        }

        Vec4 &operator=(const Vec4<T> &v_)
        {
            set(v_);
            return (*this);
        }

        template <typename U>
        Vec4 &operator=(const std::initializer_list<U> &list)
        {
            set(list);
            return (*this);
        }

        Vec4 operator+(const Vec4<T> &v_) const { return add(v_); }

        Vec4 operator-(const Vec4<T> &v_) const { return sub(v_); }

        Vec4 operator-() const { return mul(-1); }

        Vec4 operator*(const Vec4<T> &v_) const { return mul(v_); }
        Vec4 operator*(T a_) const { return mul(a_); }

        Vec4 operator/(const Vec4<T> &v_) const { return div(v_); }
        Vec4 operator/(T a_) const { return div(a_); }

        Vec4 &operator+=(const Vec4<T> &v_)
        {
            add_self(v_);
            return (*this);
        }

        Vec4 &operator-=(const Vec4<T> &v_)
        {
            sub_self(v_);
            return (*this);
        }

        Vec4 &operator*=(const Vec4<T> &v_)
        {
            mul_self(v_);
            return (*this);
        }

        Vec4 &operator/=(const Vec4<T> &v_)
        {
            div_self(v_);
            return (*this);
        }

        bool operator==(const Vec4<T> &v_) const { return isEqual(v_); }

        bool operator!=(const Vec4<T> &v_) const { return !isEqual(v_); }

    }; // class Vec3

    using Vec4i = Vec4<int>;
    using Vec4f = Vec4<float>;
    using Vec4d = Vec4<double>;

    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Vec4<T1> &v_)
    {
        os << "x: " << v_.x << " y: " << v_.y << " z: " << v_.z << " w: " << v_.w << std::endl;
        return os;
    }

    // n * v
    template <typename T1, typename T2>
    Vec4<T1> operator*(T2 n, const Vec4<T1> &v)
    {
        return v.mul((T1)n);
    }

} // namespace HSim