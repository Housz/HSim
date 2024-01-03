#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

namespace HSim
{
	template <typename T>
	class Surface3
	{
	public:
		Surface3() {};
		~Surface3(){};

		Surface3(const Transform3<T>& transform_) : transform(transform_) {}

		Vec3<T> closestPosition(const Vec3<T>& positionInWorld_) const
		{
			return transform.toWorld(closestPositionLocal(transform.toLocal(positionInWorld_)));
		}

		T closestDistance(const Vec3<T>& positionInWorld_) const
		{
			return closestDistanceLocal(transform.toLocal(positionInWorld_));
		}

		Vec3<T> closestNormal(const Vec3<T>& positionInWorld_) const
		{
			return transform.toWorld(closestNormalLocal(transform.toLocal(positionInWorld_)));
		}

	public:
		virtual Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const = 0;

		virtual Vec3<T> closestNormalLocal(const Vec3<T>& positionInLocal_) const = 0;

		T closestDistanceLocal(const Vec3<T>& positionInLocal_) const 
		{
			return positionInLocal_.distanceTo(closestPositionLocal(positionInLocal_));
		}
	
	public:
		Transform3<T> transform;
	};

	template <typename T>
	using Surface3_Ptr = std::make_shared<Surface3<T>>;
	
} // namespace HSim
