#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>
#include <HSim/ray3.h>

#include <HSim/lock.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HSim
{
	struct IntersectionInfo
	{
		bool isIntersected = false;
		float distance = MAX_FLOAT;
		
		Vec3f position;
		Vec3f normal; 
	};

	template <typename T>
	class Surface3
	{
	public:
		Surface3(){};
		~Surface3(){};

		Surface3(const Transform3<T> &transform_) : transform(transform_) {}

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

		bool intersected(const Ray3<T>& ray) 
		{
			return intersectedLocal(transform.toLocal(ray));
		}

		IntersectionInfo interact(const Ray3<T>& ray)
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

		bool isInsideLocal(const Vec3<T> &positionInLocal_) const
		{
			auto closestPosition = closestPositionLocal(positionInLocal_);
			auto closestNormal = closestNormalLocal(positionInLocal_);

			auto r = positionInLocal_ - closestPosition;
			return r.dot(closestNormal) < 0.0;
		}

		virtual AABB3<T> AABBLocal() const = 0;

		virtual bool intersectedLocal(const Ray3<T>& ray) const = 0;

		virtual IntersectionInfo interactLocal(const Ray3<T>& ray) const = 0;

	public:
		Transform3<T> transform;

		// for rendering
	public:
		bool updated = false;

		virtual void serialize() {}

		virtual size_t toVBO() { return 0; }

		virtual size_t toEBO() { return 0; }

		virtual size_t toVAO() { return 0; }

		virtual void draw() {}
	};

	template <typename T>
	using Surface3_Ptr = std::shared_ptr<Surface3<T>>;

} // namespace HSim
