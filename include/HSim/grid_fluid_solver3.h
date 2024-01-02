#pragma once
#include <HSim/animation.h>
#include <HSim/vec3.h>

namespace HSim
{
	template <typename T>
	class GridFluidSolver3 : public Animation<T>
	{

	public:
		GridFluidSolver3() {}
		~GridFluidSolver3() {}

		GridFluidSolver3(const Size3& resolution_, const Vec3<T> gridSpacing_ = {0, 0, 0}, const Vec3<T> gridOrigin_ = {0, 0, 0});
	
	// solvers
	public:
		
		// computeExforces()
		// computeViscosity()
		// computePressure()
		// computeAdvection()

		// advanceTimeStep(timeInterval)
		// {
			// beginTimeStep()

			// 1 computeExforces()
			// 2 computeViscosity()
			// 3 computePressure()
			// 4 computeAdvection()

			// endTimeStep()
		// }

	// setter, getter
	public:
		void setGravity(Vec3<T>);
		Vec3<T>& getGravity();

	public:
		Vec3<T> _gravity = {0.0, -9.8, 0.0};

	};

} // namespace HSim
