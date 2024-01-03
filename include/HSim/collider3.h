#pragma once 

#include <HSim/surface3.h>

namespace HSim
{
	template <typename T>
	class Collider3
	{
	public:
		Collider3() {};
		~Collider3() {};

		void resolveCollision()

	public:

		Surface3_Ptr<T> surface;
		T frictionCoeffient = 0.0;

	};
	
	
} // namespace HSim
