#pragma once

#include <HSim/vec3.h>
#include <HSim/surface3.h>

namespace HSim
{
	template <typename T>
	class Plane3 : public Surface3<T>
	{
	public:
		Plane3() { SurfaceType = SurfaceType::PLANE; }

		Plane3(const Plane3<T> &plane3_)
			: Surface3<T>(plane3_),
			  normal(plane3_.normal), point(plane3_.point)
		{
		}

		Plane3(const Transform3<T> &transform_) : transform(transform_) { SurfaceType = SurfaceType::PLANE; }

		Plane3(const Vec3<T> normal_, const Vec3<T> point_)
			: normal(normal_), point(point_) { SurfaceType = SurfaceType::PLANE; }

		Plane3(const Vec3<T> normal_, const Vec3<T> point_, const Transform3<T> &transform_)
			: normal(normal_), point(point_), transform(transform_) { SurfaceType = SurfaceType::PLANE; }

		Plane3(const Vec3<T> p1, const Vec3<T> p2, const Vec3<T> p3)
		{
			SurfaceType = SurfaceType::PLANE;

			auto v12 = p2 - p1;
			auto v13 = p3 - p1;

			normal = v12.cross(v13).getNormalized();
			point = p1;
		}

		Plane3(const Vec3<T> p1, const Vec3<T> p2, const Vec3<T> p3, const Transform3<T> &transform_)
			: transform(transform_)
		{
			SurfaceType = SurfaceType::PLANE;

			auto v12 = p2 - p1;
			auto v13 = p3 - p1;

			normal = v12.cross(v13).getNormalized();
			point = p1;
		}

		void clone(std::shared_ptr<SpaceObject3<T>>& ptr) override
		{
			ptr = std::make_shared<Plane3<T>>(*this);
		}

		~Plane3(){};

		// Inherited from Surface3
	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			Vec3<T> r = positionInLocal_ - point;
			return point + r - normal.dot(r) * normal;
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			return normal;
		}

		AABB3<T> AABBLocal() override
		{
			// todo
			AABB3<T> aabb;
			return aabb;
		}

		bool intersectedLocal(const Ray3<T> &ray) const override
		{
			// todo
			return false;
		}

		IntersectionInfo interactLocal(const Ray3<T> &ray) const override
		{
			// todo
			IntersectionInfo intersectionInfo;

			return intersectionInfo;
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
