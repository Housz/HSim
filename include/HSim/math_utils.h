#pragma once 

#include <HSim/common.h>

namespace HSim
{
	template <typename T>
    T clamp(T value, T min, T max)
	{
		assert(min < max);
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
    Vec3<T> clamp(const Vec3<T>& value, const Vec3<T>& min, const Vec3<T>& max)
	{
		assert(min.x < max.x && min.y < max.y && min.z < max.z);

		return 
		{
			clamp(value.x, min.x, max.x),
			clamp(value.y, min.y, max.y),
			clamp(value.z, min.z, max.z)
		};
	}

	// barycentric
	// barycentric()

	/**
	 * get the closest position of point p on line segment ab
	 */
	template <typename T>
	Vec3<T> closestPositionOnLineSegment(const Vec3<T>& a, const Vec3<T>& b, const Vec3<T>& p)
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
	Vec3<T> closestNormalOnLineSegment(
		const Vec3<T>& a, const Vec3<T>& aNormal, 
		const Vec3<T>& b, const Vec3<T>& bNormal, 
		const Vec3<T>& p)
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
