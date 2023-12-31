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

		Transform3(const Transform3<T>& transform_) { set(transform_); }

		Transform3(const Quaternion<T>& orientation_, const Vec3<T>& translation_) : orientation(orientation_), translation(translation_){}

		template <typename T1>
		void set(const Transform3<T1>& transform_)
		{
			orientation = transform_.orientation;
			translation = transform_.translation_;
		}
		
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

	// transform operators
	public:
	
		/**
		 * @brief transform position (P_w) in world to position (P_l) in local
		 * 
		 * transform M_wl, orientation R_wl, translation T_wl
		 * M_wl = T_wl * R_wl
		 * M_lw = M_wl^-1 = (T_wl * R_wl)^-1 = R_wl^-1 * T_wl^-1 = R_lw * T_lw
		 * 
		 * P_l = M_lw * P_w = (R_lw * T_lw) * P_w
		 * 
		 * @param positionInWorld : P_l
		 * @return Vec3<T> : P_w
		 */
		Vec3<T> toLocal(const Vec3<T>& positionInWorld) const
		{
			auto R_lw = orientation.inverse();
			auto T_lw = -translation;
			return R_lw * (T_lw + positionInWorld);
		}

		Vec3<T> inverse_mul(const Vec3<T>& p) const
		{
			return toLocal(p);
		}

		/**
		 * @brief transform position (P_l) in local to position (P_w) in world
		 * 
		 * transform M_wl, orientation R_wl, translation T_wl
		 * M_wl = T_wl * R_wl
		 * M_lw = M_wl^-1 = (T_wl * R_wl)^-1 = R_wl^-1 * T_wl^-1 = R_lw * T_lw
		 * 
		 * P_w = M_wl * P_w = (T_wl * R_wl) * P_l
		 * 
		 * @param positionInLocal : P_w
		 * @return Vec3<T> : P_l
		 */
		Vec3<T> toWorld(const Vec3<T>& positionInLocal) const
		{
			auto R_wl = orientation;
			auto T_wl = translation;

			return T_wl + R_wl * positionInLocal;

			// auto T_wl = getTransformMatrix();
			// Vec4<T> v4(positionInLocal, 1);
			// auto v = T_wl * v4;
			// return Vec3<T>(v);
		}

		Vec3<T> mul(const Vec3<T>& p) const
		{
			return toWorld(p);
		}

	public:
		Quaternion<T> orientation = {1, 0, 0, 0}; // or rotation
		Vec3<T> translation = {0, 0, 0};
	};

	using SE3f = Transform3<float>;
	using SE3d = Transform3<double>;
	

} // namespace HSim
