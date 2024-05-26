#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

namespace HSim
{
	template <typename T>
	inline T clamp(T value, T min, T max)
	{
		assert(min <= max);

		if (value < min)
		{
			return min;
		}
		else if (value > max)
		{
			return max;
		}
		else
		{
			return value;
		}
	}

	template <typename T>
	inline Vec3<T> clamp(const Vec3<T> &value, const Vec3<T> &min, const Vec3<T> &max)
	{
		assert(min.x <= max.x && min.y <= max.y && min.z <= max.z);

		return {
			clamp(value.x, min.x, max.x),
			clamp(value.y, min.y, max.y),
			clamp(value.z, min.z, max.z)};
	}

	template <typename T1, typename T2>
	inline Vec3<T1> min(const Vec3<T1> &a, const Vec3<T2> &b)
	{
		return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
	}

	template <typename T1, typename T2>
	inline Vec3<T1> max(const Vec3<T1> &a, const Vec3<T2> &b)
	{
		return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
	}

	// barycentric
	// barycentric()

	/**
	 * linear interpolation
	 * nx
	 * nyx
	 * nzyx
	 */

	inline double lerp(const double n0, const double n1, const double tx)
	{
		return (1 - tx) * n0 + tx * n1;
	}

	inline double lerp(const double n00, const double n01,
					   const double n10, const double n11,
					   const double tx, const double ty)
	{
		return lerp(
			lerp(n00, n01, tx),
			lerp(n10, n11, tx),
			ty);
	}

	inline double lerp(const double n000, const double n001, const double n010, const double n011,
					   const double n100, const double n101, const double n110, const double n111,
					   const double tx, const double ty, const double tz)
	{
		return lerp(lerp(n000, n001, n010, n011, tx, ty),
					lerp(n100, n101, n110, n111, tx, ty),
					tz);
	}

	/**
	 * get the closest position of point p on line segment ab
	 */
	template <typename T>
	inline Vec3<T> closestPositionOnLineSegment(const Vec3<T> &a, const Vec3<T> &b, const Vec3<T> &p)
	{
		auto ap = p - a;
		auto ab = b - a;

		auto lengthSquared = ab.length() * ab.length();

		if (lengthSquared < EPSILON)
		{
			return a;
		}

		auto t = ap.dot(ab) / lengthSquared;

		if (t < 0.)
		{
			return a;
		}
		else if (t > 1.)
		{
			return b;
		}
		else
		{
			return a + t * ab;
		}
	}

	/**
	 * get the closest normal of point p on line segment ab,
	 */
	template <typename T>
	inline Vec3<T> closestNormalOnLineSegment(
		const Vec3<T> &a, const Vec3<T> &aNormal,
		const Vec3<T> &b, const Vec3<T> &bNormal,
		const Vec3<T> &p)
	{
		auto ap = p - a;
		auto ab = b - a;

		auto lengthSquared = ab.length() * ab.length();

		if (lengthSquared < EPSILON)
		{
			return aNormal;
		}

		auto t = ap.dot(ab) / lengthSquared;

		if (t < 0.)
		{
			return aNormal;
		}
		else if (t > 1.)
		{
			return bNormal;
		}
		else
		{
			return (aNormal + t * (bNormal - aNormal)).getNormalized();
		}
	}

} // namespace HSim
