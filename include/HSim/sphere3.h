#pragma once

#include <HSim/common.h>
#include <HSim/surface3.h>
#include <HSim/vec3.h>

namespace HSim
{
	template <typename T>
	class Sphere3 : public Surface3<T>
	{
	public:
		Sphere3() {};
		~Sphere3() {};

		Sphere3(const Transform3<T>& transform_) : Surface3<T>(transform_) {};

		Sphere3(const Vec3<T>& center_, const T radius_, const Transform3<T>& transform_)
		: Surface3<T>(transform_), center(center_), radius(radius_) {}

		Sphere3(const Sphere3<T>& sphere_) 
		: Surface3<T>(sphere_.transform), center(sphere_.center), radius(sphere_.radius) {}

	public:
		Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const override
		{
			
		}

	public:
		Vec3<T> center = {0, 0, 0};
		T radius = 1;
	};
	
	
} // namespace HSim
