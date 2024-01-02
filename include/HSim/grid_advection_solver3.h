#pragma once 

#include <HSim/common.h>
#include <HSim/scalar_grid3.h>
#include <HSim/vector_field3.h>

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

	public:
		// virtual void advect( 
		// 	const ScalarGrid3<T>& input,
		// 	const VectorField3<T>& flow,
		// 	T dt,
		// 	ScalarGrid3<T>* output,
		// 	const ScalarField3& boundarySdf
        //     = ConstantScalarField3(kMaxD)
		//  ) = 0;

	};
	
	template <typename T>
	using AdvectionSolver3 = GridAdvectionSolver3<T>;

	template <typename T>
	using GridAdvectionSolver3_ptr = std::shared_ptr<GridAdvectionSolver3<T>>;
	template <typename T>
	using AdvectionSolver3_ptr = std::shared_ptr<GridAdvectionSolver3<T>>;
	
} // namespace HSim
