#pragma once

#include <HSim/common.h>
#include <HSim/solver.h>

#include <scene/game_object.h>

namespace HSim
{
	template <typename T>
	class NaiveSolver : public Solver<T>
	{
	public:
		void update(Frame<T> frame) override;

	public:
		void advanceTimeStep(double timeInterval);

		void advanceSubTimeStep(double subTimeInterval);

		void init();

	public:
		GameObject_ptr go = nullptr;
		// std::vector<GameObject_ptr> goCollection;

		Frame<T> currentFrame;
		double currentTime;

		size_t numSubSteps = 1;


	};

} // namespace HSim
