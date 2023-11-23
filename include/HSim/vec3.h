#pragma once

#include <iostream>
#include <algorithm>
#include <HSim/common.h>
// #include <HSim/Vec3.h>

namespace HSim
{

	/**
	 * @brief 3D vector
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
		Vec3(Vec3 &v_) : x(v_.x), y(v_.y), z(v_.z) {}

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
		void set(Vec3 &v_)
		{
			x = v_.x;
			y = v_.y;
			z = v_.z;
		}

		T length()
		{
			return std :;
			sqrt(x * x + y * y + z * z);
		}

		void normalize()
		{
			T length = length();
			x /= length;
			y /= length;
		}

		Vec3<T> norm()
		{
			T length = length();
			return Vec3<T>(x / length, y / length, z / length);
		}

		bool isEqual(Vec3<T> v_)
		{
			return (x == v_.x) && (y == v_.y) && (z == v_.z);
		}

		//// new_Vec3 = this_Vec3 (operator) parameters

		// add
		Vec3<T> add(T a_) { return Vec3<T>(x + a_, y + a_, z + a_); }
		Vec3<T> add(Vec3<T> &v_) { return Vec3<T>(x + v_.x, y + v_.y, z + v_.z); }

		// sub
		Vec3<T> sub(T a_) { return Vec3<T>(x - a_, y - a_, z - a_); }
		Vec3<T> sub(Vec3<T> &v_) { return Vec3<T>(x - v_.x, y - v_.y, z - v_.z); }

		// mul
		Vec3<T> mul(T a_) { return Vec3<T>(x * a_, y * a_, z * a_); }
		Vec3<T> mul(Vec3<T> &v_) { return Vec3<T>(x * v_.x, y * v_.y, z * v_.z); }

		// div
		Vec3<T> div(T a_) { return Vec3<T>(x / a_, y / a_, z / a_); }
		Vec3<T> div(Vec3<T> &v_) { return Vec3<T>(x / v_.x, y / v_.y, z / v_.z); }

		// dot
		T dot(Vec3<T> &v_) { return x * v_.x + y * v_.y + z * v_.z; }

		// cross
		T cross(Vec3<T> &v_) { return y * v_.z - v_.y * z, z * v_.x - v_.z * x, x * v_.y - v_.x * y; }

		//// this_Vec3 = this_Vec3 (operator) parameters

		void add_self(T a_)
		{
			x += a_;
			y += a_;
			z += a_;
		}
		void add_self(Vec3<T> &v_)
		{
			x += v_.x;
			y += v_.y;
			z += v_.z;
		}

		void sub_self(T a_)
		{
			x -= a_;
			y -= a_;
			z -= a_;
		}
		void sub_self(Vec3<T> &v_)
		{
			x -= v_.x;
			y -= v_.y;
			z -= v_.z;
		}

		void mul_self(T a_)
		{
			x *= a_;
			y *= a_;
			z *= a_;
		}

		void mul_self(Vec3<T> &v_)
		{
			x *= v_.x;
			y *= v_.y;
			z *= v_.z;
		}

		void div_self(T a_)
		{
			x /= a_;
			y /= a_;
			z /= a_;
		}
		void div_self(Vec3<T> &v_)
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

		Vec3 &operator=(Vec3<T> &v_)
		{
			set(v_);
			return (*this);
		}

		Vec3 operator+(Vec3<T> &v_) { return add(v_); }

		Vec3 operator-(Vec3<T> &v_) { return sub(v_); }

		Vec3 operator*(Vec3<T> &v_) { return mul(v_); }
		Vec3 operator*(T a_) { return mul(a_); }

		Vec3 operator/(Vec3<T> &v_) { return div(v_); }
		Vec3 operator/(T a_) { return div(a_); }

		Vec3 &operator+=(Vec3<T> &v_)
		{
			add_self(v_);
			return (*this);
		}

		Vec3 &operator-=(Vec3<T> &v_)
		{
			sub_self(v_);
			return (*this);
		}

		Vec3 &operator*=(Vec3<T> &v_)
		{
			mul_self(v_);
			return (*this);
		}

		Vec3 &operator/=(Vec3<T> &v_)
		{
			div_self(v_);
			return (*this);
		}

		bool operator==(Vec3<T> &v_) { return isEqual(v_); }

		bool operator!=(Vec3<T> &v_) { return !isEqual(v_); }

	}; // class Vec3

	typedef Vec3<int> vec3i;
	typedef Vec3<float> vec3f;
	typedef Vec3<double> vec3d;

} // namespace HSim