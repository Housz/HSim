#pragma once

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

        // setter, getter
    public:
        void set(const std::initializer_list<T> &list) {}
        void set(T w_, T x_, T y_, T z_)
        {
            w = w_; x = x_; y = y_; z = z_;
        }
        


    public:
        T w;
        T x;
        T y;
        T z;
    };

} // namespace HSim
