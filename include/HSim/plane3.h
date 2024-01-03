#pragma once

#include <HSim/vec3.h>
#include <HSim/surface3.h>

namespace HSim
{
	template <typename T>
	class Plane3 : public Surface3<T>
	{
	public:
		Plane3() {};
		~Plane3() {};

		Plane3(const Transform3<T>& transform_) : transform(transform_) {}

		Plane3(const Vec3<T> normal_, const Vec3<T> point_, const Transform3<T>& transform_)
		: normal(normal_), point(point_), transform(transform_) {}

		Plane3(const Vec3<T> p1, const Vec3<T> p2, const Vec3<T> p3)
		{
			auto v12 = p2 - p1;
			auto v13 = p3 - p1;

			normal = v12.cross(v13).getNormalized();
			point = p1;
		}

		Plane3(const Vec3<T> p1, const Vec3<T> p2, const Vec3<T> p3, const Transform3<T>& transform_)
		: transform(transform_)
		{
			auto v12 = p2 - p1;
			auto v13 = p3 - p1;

			normal = v12.cross(v13).getNormalized();
			point = p1;
		}

	
	// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const override
		{
			Vec3<T> r = positionInLocal_ - point;
			return point + r - normal.dot(r)*normal;
		}

		Vec3<T> closestNormalLocal(const Vec3<T>& positionInLocal_) const override
		{
			return normal;
		}

	// data
	public:
		// plane normal
		Vec3<T> normal = {0, 1, 0};
		// A point on the plane
		Vec3<T> point = {0, 0, 0};

	};
	
	// template <typename T>
	// using Plane3_Ptr = std::make_shared<Plane3<T>>;

} // namespace HSim
