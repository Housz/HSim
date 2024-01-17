#pragma once

#include <HSim/vec3.h>

namespace HSim
{
	template <typename T>
	class Ray3
	{
	public:
		Ray3() {}
		~Ray3() {}

		Ray3(const Vec3<T>& origin_, const Vec3<T>& direction_)
		: origin(origin_), direction(direction_.getNormalized()) {}

	public:
		Vec3<T> pointAt(T t) const
		{
			return origin + t * direction;
		}

	public:
		Vec3<T> origin = {0, 0, 0};
		Vec3<T> direction = {1, 0, 0};
	};
	
    using Ray3f = Vec3<float>;
    using Ray3d = Vec3<double>;
	
	
} // namespace HSim