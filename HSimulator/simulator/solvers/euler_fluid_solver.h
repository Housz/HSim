#pragma once

#include <simulator/solvers/solver.h>

namespace HSim
{
	class EulerFluidSolver : public Solver
	{
	public:
		EulerFluidSolver();
		~EulerFluidSolver();
	
	public:

		virtual void advanceTimeStep(double timeInterval) = 0;

		virtual void advanceSubTimeStep(double subTimeInterval) = 0;

		virtual void init() = 0;
	
	public:
		Vec3<PRECISION> gravity = {0, -9.8, 0};

	};

} // namespace HSim
