#pragma once

#include <HSim/vec3.h>
#include <HSim/mat33.h>
#include <HSim/mat44.h>

namespace HSim
{
    template <typename T>
    class Quaternion
    {
    public:
        Quaternion() {};
        ~Quaternion() {};
        Quaternion(const Quaternion<T>& q_) { set(q_); }
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

        void set(const Quaternion<T>& q_)
        {
            w = q_.w; x = q_.x; y = q_.y; z = q_.z;
        }

        void set(const std::initializer_list<T> &list_)
        {
            auto iter = list_.begin();
            w = *iter;
            x = *(++iter);
            y = *(++iter);
            z = *(++iter);
        }

        /**
         * @brief set quaternion from rotation axis and angle
         *
         * normalized axis: u, angle: a
         *
         * q = [cos(a/2), sin(a/2)*ux, sin(a/2)*uy, sin(a/2)*uz]
         *
         * @param axis : Axis of rotation
         * @param angle : Rotation Angle (radian)
         */
        void set(const Vec3<T> &axis_, T angle_)
        {
            const T eps = std::numeric_limits<T>::epsilon();

            T axisLengthSquared = axis_.length() * axis_.length();

            if (axisLengthSquared < eps)
            {
                setIdentity();
            }
            else
            {
                Vec3<T> normalizedAxis = axis_.getNormalized();
                T _sin = std::sin(angle_ / 2);
                T _cos = std::cos(angle_ / 2);

                w = _cos;
                x = normalizedAxis.x * _sin;
                y = normalizedAxis.y * _sin;
                z = normalizedAxis.z * _sin;
            }
        }

        // void set(const Vec3<T>& from, const Vec3<T>& to);

        // void set(
        //     const Vec3<T>& rotationBasis0,
        //     const Vec3<T>& rotationBasis1,
        //     const Vec3<T>& rotationBasis2);

        /**
         * @brief 3x3 rotation matrix to quaternion
         * https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
         * @param mat
         */
        void set(const Mat33<T> &m_)
        {
            const T eps = std::numeric_limits<T>::epsilon();
            const T quater = static_cast<T>(0.25);

            T onePlusTrace = m_.trace() + 1;

            if (onePlusTrace > eps)
            {
                T S = std::sqrt(onePlusTrace) * 2;
                w = quater * S;
                x = (m_(2, 1) - m_(1, 2)) / S;
                y = (m_(0, 2) - m_(2, 0)) / S;
                z = (m_(1, 0) - m_(0, 1)) / S;
            }
            else if (m_(0, 0) > m_(1, 1) && m_(0, 0) > m_(2, 2))
            {
                T S = std::sqrt(1 + m_(0, 0) - m_(1, 1) - m_(2, 2)) * 2;
                w = (m_(2, 1) - m_(1, 2)) / S;
                x = quater * S;
                y = (m_(0, 1) + m_(1, 0)) / S;
                z = (m_(0, 2) + m_(2, 0)) / S;
            }
            else if (m_(1, 1) > m_(2, 2))
            {
                T S = std::sqrt(1 + m_(1, 1) - m_(0, 0) - m_(2, 2)) * 2;
                w = (m_(0, 2) - m_(2, 0)) / S;
                x = (m_(0, 1) + m_(1, 0)) / S;
                y = quater * S;
                z = (m_(1, 2) + m_(2, 1)) / S;
            }
            else
            {
                T S = std::sqrt(1 + m_(2, 2) - m_(0, 0) - m_(1, 1)) * 2;
                w = (m_(1, 0) - m_(0, 1)) / S;
                x = (m_(0, 2) + m_(2, 0)) / S;
                y = (m_(1, 2) + m_(2, 1)) / S;
                z = quater * S;
            }
        }

        /**
         * @brief Conversion Quaternion to Matrix
         * https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
         * v' = qvq* = Mv
         * get M
         * @return Mat33<T> 
         */
        Mat33<T> toMatrix() const
        {
            T _2xx = 2 * x * x;
            T _2yy = 2 * y * y;
            T _2zz = 2 * z * z;
            T _2xy = 2 * x * y;
            T _2xz = 2 * x * z;
            T _2xw = 2 * x * w;
            T _2yz = 2 * y * z;
            T _2yw = 2 * y * w;
            T _2zw = 2 * z * w;

            Mat33<T> m(
                1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw,
                _2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw,
                _2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx);

            return m;
        }

        /**
         * @brief v' = qvq*
         * v' = [M]v
         *
         * @param v
         * @return Vec3<T>
         */
        Vec3<T> mul(const Vec3<T>& v_) const
        {
            T _2xx = 2 * x * x;
            T _2yy = 2 * y * y;
            T _2zz = 2 * z * z;
            T _2xy = 2 * x * y;
            T _2xz = 2 * x * z;
            T _2xw = 2 * x * w;
            T _2yz = 2 * y * z;
            T _2yw = 2 * y * w;
            T _2zw = 2 * z * w;

            return Vec3<T>(
                (1 - _2yy - _2zz) * v_.x + (_2xy - _2zw) * v_.y + (_2xz + _2yw) * v_.z,
                (_2xy + _2zw) * v_.x + (1 - _2zz - _2xx) * v_.y + (_2yz - _2xw) * v_.z,
                (_2xz - _2yw) * v_.x + (_2yz + _2xw) * v_.y + (1 - _2yy - _2xx) * v_.z);
        }

        /**
         * @brief q' = qq_
         *
         * @param q_
         * @return Quaternion<T>
         */
        Quaternion<T> mul(const Quaternion<T>& q_) const
        {
            return Quaternion(
                w * q_.w - x * q_.x - y * q_.y - z * q_.z,
                w * q_.x + x * q_.w + y * q_.z - z * q_.y,
                w * q_.y - x * q_.z + y * q_.w + z * q_.x,
                w * q_.z + x * q_.y - y * q_.x + z * q_.w);
        }

        void mul_self(const Quaternion<T>& q_)
        {
            set(w * q_.w - x * q_.x - y * q_.y - z * q_.z,
                w * q_.x + x * q_.w + y * q_.z - z * q_.y,
                w * q_.y - x * q_.z + y * q_.w + z * q_.x,
                w * q_.z + x * q_.y - y * q_.x + z * q_.w);
        }

        T dot(const Quaternion<T>& q_) const
        {
            return w * q_.w + x * q_.x + y * q_.y + z * q_.z;
        }

        void setIdentity()
        {
            set(1, 0, 0, 0);
        }

        /**
         * @brief rotate this quaternion
         * thisAngle += angle
         *
         * @param angle : radian
         */
        void rotate(T angle_)
        {
            auto thisAxis = axis();
            auto thisAngle = angle();

            thisAngle += angle_;

            set(thisAxis, thisAngle);
        }

        T angle() const
        {
            T ag = 2 * std::acos(w);

            if (ag < PI)
            {
                return ag;
            }
            else
            {
                return 2 * PI - ag;
            }
        }

        Vec3<T> axis() const
        {
            Vec3<T> ax(x, y, z);
            ax.normalize();

            if (2 * std::acos(w) < PI)
            {
                return ax;
            }
            else
            {
                return -ax;
            }
        }

        T norm() const
        {
            return std::sqrt(w * w + x * x + y * y + z * z);
        }

        void normalize()
        {
            T n = norm();

            if (n > 0)
            {
                w /= n;
                x /= n;
                y /= n;
                z /= n;
            }
        }

        /**
         * @brief get conjugate q* of this quaternion
         * q* = [w, -x, -y, -z]
         * @return Quaternion<T>
         */
        Quaternion<T> conjugate() const
        {
            return Quaternion(w, -x, -y, -z);
        }

        /**
         * @brief get inverse q^-1 of this quaternion
         * q^-1 = q* / norm^2
         * @return Quaternion<T>
         */
        Quaternion<T> inverse() const
        {
            T norm2 = w * w + x * x + y * y + z * z;
            return Quaternion(w / norm2, -x / norm2, -y / norm2, -z / norm2);
        }

        Quaternion<T>& operator=(const Quaternion<T>& q_)
        {
            set(q_);
            return *this;
        }

        Quaternion<T>& operator*=(const Quaternion<T>& q_)
        {
            mul_self(q_);
            return *this;
        }

    public:
        T w; // real part
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

    template <typename T1, typename T2>
    Vec3<T2> operator*(const Quaternion<T1>& q, const Vec3<T2>& v)
    {
        return q.mul(v);
    }    

    template <typename T1, typename T2>
    Quaternion<T1> operator*(const Quaternion<T1>& q1, const Quaternion<T2>& q2)
    {
        return q1.mul(q2);
    }   

    using Quaternionf = Quaternion<float>;
    using Quaterniond = Quaternion<double>;

} // namespace HSim
