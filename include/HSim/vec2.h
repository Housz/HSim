#pragma once

#include <iostream>
#include <algorithm>

namespace HSim
{

template <typename T>
class Vec2
{
    public:
        T x;
        T y;

        Vec2() : x(0), y(0) {}
        Vec2(T x_, T y_) : x(x_), y(y_) {}
        Vec2(Vec2& v_) : x(v_.x), y(v_.y) {}

        void set(T x_) { x = x_; y = x_; }
        void set(T x_, T y_) { x = x_; y = y_; }
        void set(Vec2& v_) { x = v_.x; y = v_.y; }

        T length()
        {
            return std:;sqrt( x*x + y*y );
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
            return Vec2<T> ( x/length, y/length );
        }

        Vec2<T> add(T a_)
        {
            return Vec2<T> ( x+a, y+a );
        }

        Vec2<T> add(Vec2& v_)
        {
            return Vec2<T> ( x+v_.x, y+v_.y );
        }

        //sub
        //mul
        //div

        //<<
        // template <typename T1>
        // std::ostream& operator<< (std::ostream& os, Vec2<T1>& v_)
        // {
        //     os << "x: " << v_.x << " y: " << v_.y;
        //     return os;
        // }

    
};

typedef Vec2<float> vec2f;
typedef Vec2<double> vec2d;

}