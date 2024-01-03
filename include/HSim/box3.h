#pragma once 

#include <HSim/common.h>
#include <HSim/vec3.h>
#include <HSim/surface3.h>

namespace HSim
{
	template <typename T>
	class Box3 : public Surface3<T>
	{
	public:
		Box3() {};
		~Box3() {};

		Box3(const Transform3<T> transform_) : transform(transform_) {}

		Box3(const Vec3<T>& lowerCorner_, const Vec3<T>& upperCorner_, const Transform3<T> transform_) 
		: lowerCorner(lowerCorner_), upperCorner(upperCorner_), transform(transform_) {}

		/**
		 * @brief set Box3 with any two points
		 */
		void set(const Vec3<T>& p1, const Vec3<T>& p2)
		{
			lowerCorner = {std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)};
			upperCorner = {std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)};
		}

		void setLowerCorner(const Vec3<T>& lowerCorner_) 
		{
			assert(lowerCorner_.x < upperCorner.x && lowerCorner_.y < upperCorner.y && lowerCorner_.z < upperCorner.z);
			lowerCorner = lowerCorner_;
		}

		void setUpperCorner(const Vec3<T>& upperCorner_) 
		{
			assert(lowerCorner.x < upperCorner_.x && lowerCorner.y < upperCorner_.y && lowerCorner.z < upperCorner_.z);
			upperCorner = upperCorner_;
		}

		T width() { return upperCorner.x - lowerCorner.x; }
		T height() { return upperCorner.y - lowerCorner.y; }
		T depth() { return upperCorner.z - lowerCorner.z; }
	
	// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const override
		{

		}

		Vec3<T> closestNormalLocal(const Vec3<T>& positionInLocal_) const override
		{

		}

	public:
		Vec3<T> lowerCorner = {0, 0, 0};
		Vec3<T> upperCorner = {1, 1, 1};
	};

	// template <typename T>
	// using Box3_Ptr = std::make_shared<Box3<T>>;

	// template <typename T>
	// class BoundingBox3 : public Box3<T>
	// {

	// };

	
} // namespace HSim
