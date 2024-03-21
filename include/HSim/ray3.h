#pragma once

#include <HSim/vec3.h>

namespace HSim
{
	template <typename T>
	class Ray3
	{
	public:
		Ray3() {}
		~Ray3() {}
		
		Ray3(const Vec3<T>& origin_, const Vec3<T>& direction_) : origin(origin_), direction(direction_.getNormalized()) {}

		// template<typename T1, typename T2>
		// Ray3(const std::initializer_list<T1> &origin_list, const std::initializer_list<T2> &direction_list)
		// : origin(origin_list) 
		// { 
		// 	direction = direction_list; 
		// 	direction.normalize(); 
		// }

		template<typename T1>
		Ray3(const Ray3<T1>& ray_) : origin(ray_.origin), direction(ray_.direction) {}

	public:
		Vec3<T> pointAt(T t) const
		{
			return origin + t * direction;
		}

	public:
		Vec3<T> origin = {0, 0, 0};
		Vec3<T> direction = {1, 0, 0};
	};
	
    using Ray3f = Ray3<float>;
    using Ray3d = Ray3<double>;
	
	
} // namespace HSim