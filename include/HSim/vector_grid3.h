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

		VectorGrid3(size_t x, size_t y, size_t z) 
		: Grid3<T>(x, y, z)
		{
		}

		VectorGrid3(Vec3i resolution, Vec3<T> origin = {0, 0, 0}, Vec3<T> gridSpacing = {1, 1, 1})
		: Grid3<T>(resolution, origin, gridSpacing)
		{

		}

		// data
	public:
		// std::vector<Vec3<T>> _data; // [...][...][...]
		// std::vector<T> _data; // [... ... ...]
	};


} // namespace HSim
