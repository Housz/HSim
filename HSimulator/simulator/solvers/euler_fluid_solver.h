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
		void update(const Frame &frame) override;

		void advanceTimeStep(double timeInterval) = 0;

		void advanceSubTimeStep(double subTimeInterval) = 0;

		void init() = 0;
	
	public:
		Vec3<PRECISION> gravity = {0, -9.8, 0};

	};

} // namespace HSim
