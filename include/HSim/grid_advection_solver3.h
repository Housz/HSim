#pragma once 

#include <HSim/common.h>

namespace HSim
{
	template <typename T>
	class GridAdvectionSolver3
	{
	private:
		/* data */
	public:
		GridAdvectionSolver3() {};
		~GridAdvectionSolver3() {};
	};
	
	template <typename T>
	using AdvectionSolver3 = GridAdvectionSolver3<T>;

	template <typename T>
	using GridAdvectionSolver3_ptr = std::shared_ptr<GridAdvectionSolver3<T>>;
	template <typename T>
	using AdvectionSolver3_ptr = std::shared_ptr<GridAdvectionSolver3<T>>;
	
} // namespace HSim
