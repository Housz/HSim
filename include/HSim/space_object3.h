#pragma once
#include <HSim/common.h>

namespace HSim
{
	// base class for 3d objects
	template <typename T>
	class SpaceObject3
	{
	public:
		SpaceObject3() {}
		~SpaceObject3() {}

		// todo: class info: class name, object id
	};

	template <typename T> 
	using SpaceObject3_Ptr = std::shared_ptr<SpaceObject3<T>>;

} // namespace HSim
