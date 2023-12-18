#pragma once

#include <HSim/grid3.h>
#include <HSim/vec3.h>

namespace HSim
{

	template <typename T>
	class VectorGrid3 : public Grid3<T>
	{
		// constructor
	public:
		VectorGrid3(){};
		~VectorGrid3(){};



		// data
	public:
		// std::vector<Vec3<T>> _data; // [...][...][...]
		// std::vector<T> _data; // [... ... ...]
	};

} // namespace HSim
