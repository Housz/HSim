#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

namespace HSim
{
	template <typename T>
	class Surface3
	{
	public:
		Surface3(){};
		~Surface3(){};

		Surface3(const Transform3<T>& transform_) : transform(transform_) {}

		template <typename T1>
		Vec3<T1> closestPosition(const Vec3<T1>& positionInWorld_) const
		{
			return transform.toWorld(closestPositionLocal(transform.toLocal(positionInWorld_)));
		}

		T closestDistance(const Vec3<T1>& positionInWorld_) const
		{
			return closestDistanceLocal(transform.toLocal(positionInWorld_));
		}

	public:
		virtual Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const = 0;

		T closestDistanceLocal(const Vec3<T>& positionInLocal_) const 
		{
			return positionInLocal_.distanceTo(closestPositionLocal(positionInLocal_));
		}
	
	public:
		Transform3<T> transform;
	};
	
} // namespace HSim
