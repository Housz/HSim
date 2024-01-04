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
    
} // namespace HSim
