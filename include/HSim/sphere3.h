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

		void setCenter(const Vec3<T> center_) { center = center_; }
		void setRadius(const T radius_) { radius = radius_; }
		void setTransform(const Transform3<T> transform_) { transform = transform_; }

		// center in world frame;
		Vec3<T> getCenter() const
		{
			return transform.toWorld(center);
		}

	// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const override
		{
			return center + radius * closestNormalLocal(positionInLocal_);
		}

		Vec3<T> closestNormalLocal(const Vec3<T>& positionInLocal_) const override
		{
			if (center.isSimilar(positionInLocal_))
			{
				return Vec3<T>(1, 0, 0); // ?
			}
			else
			{
				return (positionInLocal_ - center).getNormalized();
			}
		}

	// data
	public:
		// center in local frame;
		Vec3<T> center = {0, 0, 0};
		T radius = 1;
	};
	
	template <typename T>
	using Sphere3_Ptr = std::shared_ptr<Sphere3<T>>;
	
} // namespace HSim
