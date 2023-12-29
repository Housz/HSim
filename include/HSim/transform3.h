#pragma once

#include <HSim/quaternion.h>
#include <HSim/vec3.h>
#include <HSim/mat33.h>
#include <HSim/mat44.h>

namespace HSim
{
	template <typename T>
	class Transform3
	{
	public:
		Transform3() {};
		~Transform3() {};

		Transform3(const Quaternion<T>& orientation_, const Vec3<T>& translation_) : orientation(orientation_), translation(translation_){}
		
		void set(const Quaternion<T>& orientation_, const Vec3<T>& translation_)
		{
			orientation = orientation_;
			translation = translation_;
		}

		void setOrientation(const Quaternion<T>& orientation_)
		{
			orientation = orientation_;
		}

		void setTranslation(const Vec3<T>& translation_)
		{
			translation = translation_;
		}

		Mat33<T> getOrientationMatrix() const
		{
			return orientation.toMatrix();
		}

		Mat44<T> getTransformMatrix() const
		{
			Mat44<T> m;

			m.set(getOrientationMatrix());
			m.setCol(3, Vec4<T>(translation, 1));

			return m;
		}

	public:
		Quaternion<T> orientation;
		Vec3<T> translation;

		// Mat33<T> orientationMatrix;
		// Mat44<T> transformMatrix;
	};

	

} // namespace HSim
