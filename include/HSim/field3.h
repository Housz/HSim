#pragma once

#include <HSim/vec3.h>

namespace HSim
{
	
    template <typename T, size_t X, size_t Y, size_t Z>
	class Field3
	{
	public:
		Field3() {}
        ~Field3() {}

        size_t size_x() { return X; }
        size_t size_y() { return Y; }
        size_t size_z() { return Z; }
	};


} // namespace HSim