#pragma once

#include <HSim/vec3.h>

namespace HSim
{
    template <typename T>
    class Quaternion
    {
    public:
        Quaternion(){};
        ~Quaternion(){};
        Quaternion(T w_, T x_, T y_, T z_) : w(w_), x(x_), y(y_), z(z_) {}
        Quaternion(const std::initializer_list<T> &list) { set(list); }
        Quaternion(T w_, const Vec3<T> &u_) : w(w_), x(u_.x), y(u_.y), z(u_.z) {}
        Quaternion(const Vec3<T> &axis, T angle) { set(axis, angle); }

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Quaternion<T1> &);

        // setter, getter
    public:

        void set(T w_, T x_, T y_, T z_)
        {
            w = w_;
            x = x_;
            y = y_;
            z = z_;
        }

        void set(const std::initializer_list<T> &list)
        {
            auto iter = list.begin();
            w = *iter;
            x = *(++iter);
            y = *(++iter);
            z = *(++iter);
        }

        /**
         * @brief
         *
         * normalized axis: u, angle: a
         *
         * q = [cos(a/2), sin(a/2)*ux, sin(a/2)*uy, sin(a/2)*uz]
         *
         * @param axis : Axis of rotation
         * @param angle : Rotation Angle
         */
        void set(const Vec3<T> &axis, T angle)
        {
            const T eps = std::numeric_limits<T>::epsilon();

            T axisLengthSquared = axis.length() * axis.length();

            if (axisLengthSquared < eps)
            {
                setIdentity();
            }
            else
            {
                Vec3<T> normalizedAxis = axis.getNormalized();
                T _sin = std::sin(angle / 2);
                T _cos = std::cos(angle / 2);

                w = _cos;
                x = normalizedAxis.x * _sin;
                y = normalizedAxis.y * _sin;
                z = normalizedAxis.z * _sin;
            }
        }

    public:
        T w; // real
        T x; // i
        T y; // j
        T z; // k
    };

    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Quaternion<T1> &q)
    {
        os << q.w << " " << q.x << " " << q.y << " " << q.z << std::endl;
        return os;
    }

    using Quaternionf = Quaternion<float>;
    using Quaterniond = Quaternion<double>;

} // namespace HSim
