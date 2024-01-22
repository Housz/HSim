#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		bool isInsideWorld(const Vec3<T> &positionInWorld_) const
		{
			return isInsideLocal(transform.toLocal(positionInWorld_));
		}
		bool isInside(const Vec3<T> &positionInWorld_) const
		{
			return isInsideLocal(transform.toLocal(positionInWorld_));
		}

	public:
		virtual void serialize() const {}

		virtual size_t toVBO() const { return 0; }
		
		virtual size_t toEBO() const { return 0; }
		
		virtual size_t toVAO() const { return 0; }

		virtual void draw() const {}

	// in local frame
	public:
		virtual Vec3<T> closestPositionLocal(const Vec3<T>& positionInLocal_) const = 0;

		virtual Vec3<T> closestNormalLocal(const Vec3<T>& positionInLocal_) const = 0;

		T closestDistanceLocal(const Vec3<T>& positionInLocal_) const 
		{
			return positionInLocal_.distanceTo(closestPositionLocal(positionInLocal_));
		}

		bool isInsideLocal(const Vec3<T>& positionInLocal_) const 
		{
			auto closestPosition = closestPositionLocal(positionInLocal_);
			auto closestNormal = closestNormalLocal(positionInLocal_);
			
			auto r = positionInLocal_ - closestPosition;
			return r.dot(closestNormal) < 0.0;
		}
	
	public:
		Transform3<T> transform;
	};

	template <typename T>
	using Surface3_Ptr = std::shared_ptr<Surface3<T>>;
	
} // namespace HSim
