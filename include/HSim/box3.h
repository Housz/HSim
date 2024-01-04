#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>
#include <HSim/surface3.h>
#include <HSim/plane3.h>
#include <HSim/math_utils.h>

namespace HSim
{
	template <typename T>
	class Box3 : public Surface3<T>
	{
	public:
		Box3(){};
		~Box3(){};

		Box3(const Transform3<T> transform_) : transform(transform_) {}

		Box3(const Vec3<T> &lowerCorner_, const Vec3<T> &upperCorner_)
			: lowerCorner(lowerCorner_), upperCorner(upperCorner_) {}

		Box3(const Vec3<T> &lowerCorner_, const Vec3<T> &upperCorner_, const Transform3<T> transform_)
			: lowerCorner(lowerCorner_), upperCorner(upperCorner_), transform(transform_) {}

		Box3(const Box3<T>& box_) : lowerCorner(box_.lowerCorner), upperCorner(box_.upperCorner) {  }

		/**
		 * @brief set Box3 with any two points
		 */
		void set(const Vec3<T> &p1, const Vec3<T> &p2)
		{
			lowerCorner = {std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)};
			upperCorner = {std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)};
		}

		void setLowerCorner(const Vec3<T> &lowerCorner_)
		{
			assert(lowerCorner_.x < upperCorner.x && lowerCorner_.y < upperCorner.y && lowerCorner_.z < upperCorner.z);
			lowerCorner = lowerCorner_;
		}

		void setUpperCorner(const Vec3<T> &upperCorner_)
		{
			assert(lowerCorner.x < upperCorner_.x && lowerCorner.y < upperCorner_.y && lowerCorner.z < upperCorner_.z);
			upperCorner = upperCorner_;
		}

		T width() const { return upperCorner.x - lowerCorner.x; }
		T height() const { return upperCorner.y - lowerCorner.y; }
		T depth() const { return upperCorner.z - lowerCorner.z; }

		bool isInsideLocal(const Vec3<T> &positionInLocal_) const
		{
			auto p_ = positionInLocal_;
			if (p_.x < lowerCorner.x || p_.y < lowerCorner.y || p_.z < lowerCorner.z ||
				p_.x > upperCorner.x || p_.y > upperCorner.y || p_.z > upperCorner.z)
			{
				return false;
			}
			else
				return true;
		}

		bool isInsideWorld(const Vec3<T> &positionInWorld_) const
		{
			return isInsideLocal(transform.toLocal(positionInWorld_));
		}

		// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			// if positionInLocal_ is inside this box
			if (isInsideLocal(positionInLocal_))
			{
				Plane3<T> planes[6] = {
					Plane3<T>({1, 0, 0}, upperCorner), // right
					Plane3<T>({0, 1, 0}, upperCorner), // up
					Plane3<T>({0, 0, 1}, upperCorner), // front
					Plane3<T>({1, 0, 0}, lowerCorner), // left
					Plane3<T>({0, 1, 0}, lowerCorner), // down
					Plane3<T>({0, 0, 1}, lowerCorner)  // back
				};

				Vec3<T> closestPosition = planes[0].closestPositionLocal(positionInLocal_);
				T closestDistance = planes[0].closestDistanceLocal(positionInLocal_);
				for (size_t i = 1; i < 6; i++)
				{
					auto currDistance = planes[i].closestDistanceLocal(positionInLocal_);
					if (currDistance < closestDistance)
					{
						closestDistance = currDistance;

						auto currPosition = planes[i].closestPositionLocal(positionInLocal_);
						closestPosition = currPosition;
					}
				}

				return closestPosition;
			}
			// if positionInLocal_ is outside this box
			else
			{
				return clamp(positionInLocal_, lowerCorner, upperCorner);
			}
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			Plane3<T> planes[6] = {
				Plane3<T>({1, 0, 0}, upperCorner), // right
				Plane3<T>({0, 1, 0}, upperCorner), // up
				Plane3<T>({0, 0, 1}, upperCorner), // front
				Plane3<T>({1, 0, 0}, lowerCorner), // left
				Plane3<T>({0, 1, 0}, lowerCorner), // down
				Plane3<T>({0, 0, 1}, lowerCorner)  // back
			};

			auto closestNormal = planes[0].normal;
			T closestDistance = planes[0].closestDistanceLocal(positionInLocal_);
			for (size_t i = 1; i < 6; i++)
			{
				auto currDistance = planes[i].closestDistanceLocal(positionInLocal_);
				if (currDistance < closestDistance)
				{
					closestDistance = currDistance;
					closestNormal = planes[i].normal;
				}
			}

			return closestNormal;
		}

	public:
		Vec3<T> lowerCorner = {0, 0, 0};
		Vec3<T> upperCorner = {1, 1, 1};
	};

	// template <typename T>
	// using Box3_Ptr = std::make_shared<Box3<T>>;

} // namespace HSim
