#include <simulator/solvers/naive_solver.h>

namespace HSim
{
	template <typename T>
	void NaiveSolver<T>::update(Frame frame)
	{
		if (frame.index > currentFrame.index)
		{
			auto numFrames = frame.index - currentFrame.index;

			for (size_t i = 0; i < numFrames; i++)
			{
				advanceTimeStep(frame.timeInterval);
			}
			
			currentFrame = frame;
		}
	}

	template <typename T>
	void NaiveSolver<T>::advanceTimeStep(double timeInterval)
	{
		auto subTimeInterval = timeInterval / numSubSteps;

		for (size_t i = 0; i < numSubSteps; i++)
		{
			// timer start
			advanceSubTimeStep(subTimeInterval);
			// timer end

			currentTime += subTimeInterval;
		}

		go->renderable->renderingDataNeedUpdate = true;
	}

	template <typename T>
	void NaiveSolver<T>::advanceSubTimeStep(double subTimeInterval)
	{
		// timer start

		auto renderable = go->renderable;
		
		auto obj = go->renderable->spaceObject;

		obj->transform.translation.y -= 0.01;
		
		// renderable->renderingDataNeedUpdate = true;

		// timer end
	}

	template <typename T>
	void NaiveSolver<T>::init()
	{
		// pass
		std::cout << "[Simulator] Init Naive Solver.\n";
	}
}