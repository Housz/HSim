#pragma once

#include <HSim/common.h>
#include <HSim/space_object3.h>
#include <HSim/transform3.h>
#include <HSim/ray3.h>

#include <HSim/lock.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HSim
{
	enum class SurfaceType
	{
		BOX,
		SPHERE,
		Cylinder,
		PLANE,
		TRIANGLE,
		TRIANGLE_MESH,
	};

	struct IntersectionInfo
	{
		bool isIntersected = false;
		float distance = MAX_FLOAT;

		Vec3f position;
		Vec3f normal;

		inline friend std::ostream &operator<<(std::ostream &, IntersectionInfo &);
	};

	inline std::ostream &operator<<(std::ostream &os, IntersectionInfo &info)
	{
		if (info.isIntersected)
		{
			os << "isIntersected: "
			   << "true"
			   << "\n"
			   << "distance: " << info.distance << "\n"
			   << "position: " << info.position << "\n"
			   << "normal: " << info.normal << "\n";
		}
		else
		{
			os << "isIntersected: "
			   << "false"
			   << "\n";
		}

		return os;
	}

	inline std::ostream &operator<<(std::ostream &os, const IntersectionInfo &info)
	{
		if (info.isIntersected)
		{
			os << "isIntersected: "
			   << "true"
			   << "\n"
			   << "distance: " << info.distance << "\n"
			   << "position: " << info.position << "\n"
			   << "normal: " << info.normal << "\n";
		}
		else
		{
			os << "isIntersected: "
			   << "false"
			   << "\n";
		}

		return os;
	}

	template <typename T>
	class Surface3 : public SpaceObject3<T>
	{
	public:
		Surface3()
		{
			spaceObjectType = SpaceObjectType::SURFACE;
		};

		Surface3(const Transform3<T>& transform_) : transform(transform_)
		{
			spaceObjectType = SpaceObjectType::SURFACE;
		}

		Surface3(const Surface3<T>& surface3_)
			: transform(surface3_.transform), SurfaceType(surface3_.SurfaceType)
		{
			spaceObjectType = SpaceObjectType::SURFACE;
		}

		~Surface3(){};

		Vec3<T> closestPosition(const Vec3<T> &positionInWorld_) const
		{
			return transform.toWorld(closestPositionLocal(transform.toLocal(positionInWorld_)));
		}

		T closestDistance(const Vec3<T> &positionInWorld_) const
		{
			return closestDistanceLocal(transform.toLocal(positionInWorld_));
		}

		Vec3<T> closestNormal(const Vec3<T> &positionInWorld_) const
		{
			return transform.toWorld(closestNormalLocal(transform.toLocal(positionInWorld_)));
		}

		bool isInsideWorld(const Vec3<T> &positionInWorld_) const
		{
			return isInsideLocal(transform.toLocal(positionInWorld_));
		}
		bool isInside(const Vec3<T> &positionInWorld_) const
		{
			return isInsideLocal(transform.toLocal(positionInWorld_));
		}

		AABB3<T> AABB()
		{
			return transform.toWorld(AABBLocal());
		}

		bool intersected(const Ray3<T> &ray)
		{
			return intersectedLocal(transform.toLocal(ray));
		}

		IntersectionInfo interact(const Ray3<T> &ray)
		{
			auto interactionInfoLocal = interactLocal(transform.toLocal(ray));
			IntersectionInfo interactionInfo;
			interactionInfo.position = transform.toWorld(interactionInfoLocal.position);
			interactionInfo.normal = transform.toWorld(interactionInfoLocal.normal);
			return interactionInfo;
		}

		// in local frame
	public:
		virtual Vec3<T> closestPositionLocal(const Vec3<T> &positionInLocal_) const = 0;

		virtual Vec3<T> closestNormalLocal(const Vec3<T> &positionInLocal_) const = 0;

		T closestDistanceLocal(const Vec3<T> &positionInLocal_) const
		{
			return positionInLocal_.distanceTo(closestPositionLocal(positionInLocal_));
		}

		virtual bool isInsideLocal(const Vec3<T> &positionInLocal_) const
		{
			auto closestPosition = closestPositionLocal(positionInLocal_);
			auto closestNormal = closestNormalLocal(positionInLocal_);

			auto r = positionInLocal_ - closestPosition;
			return r.dot(closestNormal) < 0.0;
		}

		virtual AABB3<T> AABBLocal() = 0;

		virtual bool intersectedLocal(const Ray3<T> &ray) const = 0;

		virtual IntersectionInfo interactLocal(const Ray3<T> &ray) const = 0;

	public:
		Transform3<T> transform;

		SurfaceType SurfaceType;
		// for rendering
	public:
		// status flags
		bool renderingDataNeedUpdate = true;
		bool aabbNeedUpdate = true;

		// simulator change surface
		// surface.updateStatus() { renderingDataNeedUpdate = true;  aabbNeedUpdate = ture; }

		// buildAABB() {if(aabbNeedUpdate) build...; aabbNeedUpdate = false}
		// buildrenderdata() (if(renderingDataNeedUpdate) build...; renderingDataNeedUpdate = false;)

		void resetStatusFlags()
		{
			renderingDataNeedUpdate = true;
			aabbNeedUpdate = true;
		}

		virtual void serialize() {}

		virtual size_t toVBO() { return 0; }

		virtual size_t toEBO() { return 0; }

		virtual size_t toVAO() { return 0; }

		virtual void draw() {}

		virtual void drawBoundary() {}
	};

	template <typename T>
	using Surface3_Ptr = std::shared_ptr<Surface3<T>>;

	using Surface3f = Surface3<float>;
	using Surface3d = Surface3<double>;

} // namespace HSim
