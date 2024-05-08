#pragma once

#include <HSim/common.h>
// #include <HSim/solver.h>
#include <simulator/solvers/solver.h>
#include <scene/game_object.h>

namespace HSim
{
	class NaiveSolver : public Solver
	{
	public:
		NaiveSolver();
		~NaiveSolver();

	public:
		void update(const SimFrame &frame) override;
	public:
		void advanceTimeStep(double timeInterval);

		void advanceSubTimeStep(double subTimeInterval);

		void init();

	public:
		GameObject_ptr go = nullptr;
		// std::vector<GameObject_ptr> goCollection;

		SimFrame currentFrame;
		double currentTime = 0.0;

		size_t numSubSteps = 1;

	public:
		Vec3<PRECISION> gravity = {0, -9.8, 0}; 
		Vec3<PRECISION> velocity = {0, 0, 0};
	};

	using NaiveSolver_Ptr = std::shared_ptr<NaiveSolver>;

} // namespace HSim
