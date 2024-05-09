#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>
#include <HSim/surface3.h>
#include <HSim/math_utils.h>

namespace HSim
{
	template <typename T>
	class Cylinder3 : public Surface3<T>
	{
	public:
		Cylinder3() { SurfaceType = SurfaceType::Cylinder; }

		Cylinder3(const Cylinder3<T> &cylinder3_)
			: Surface3<T>(cylinder3_),
			  radiusTop(cylinder3_.radiusTop), radiusBottom(cylinder3_.radiusBottom), height(cylinder3_.height)
		{
			SurfaceType = SurfaceType::Cylinder;
		}

		Cylinder3(const Transform3<T> transform_)
			: transform(transform_) { SurfaceType = SurfaceType::Cylinder; }

		Cylinder3(const T radius_, const T height_)
			: radiusTop(radius_), radiusBottom(radius_), height(height_)
		{
			SurfaceType = SurfaceType::Cylinder;
		}

		Cylinder3(const T radiusTop_, const T radiusBottom_, const T height_)
			: radiusTop(radiusTop_), radiusBottom(radiusBottom_), height(height_)
		{
			SurfaceType = SurfaceType::Cylinder;
		}

		~Cylinder3() {}

		void setRadius(const T radius_)
		{
			radiusTop = radius_;
			radiusBottom = radius_;
		}

		void setRadius(const T radiusTop_, const T radiusBottom_)
		{
			radiusTop = radiusTop_;
			radiusBottom = radiusBottom_;
		}

		void setRadiusTop(const T radiusTop_)
		{
			radiusTop = radiusTop_;
		}

		void setRadiusBottom(const T radiusBottom_)
		{
			radiusBottom = radiusBottom_;
		}

		void setHeight(const T height_)
		{
			height = height;
		}

	public:
		Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo
			return Vec3<T>();
		}

		Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const override
		{
			// todo
			return Vec3<T>();
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
		T radiusTop;
		T radiusBottom;
		T height;
	};

	template <typename T>
	using Cylinder3_Ptr = std::shared_ptr<Cylinder3<T>>;

} // namespace HSim
