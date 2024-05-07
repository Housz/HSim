#include <simulator/solvers/naive_solver.h>

namespace HSim
{
	NaiveSolver::NaiveSolver()
	{
	}

	NaiveSolver::~NaiveSolver()
	{
	}

	void NaiveSolver::update(const Frame &frame)
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

	void NaiveSolver::advanceTimeStep(double timeInterval)
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

	void NaiveSolver::advanceSubTimeStep(double subTimeInterval)
	{
		// timer start

		auto renderable = go->renderable;

		auto obj = std::static_pointer_cast<Surface3<PRECISION>>(go->renderable->spaceObject);

		// obj->transform.translation.y -= 0.0001;
		
		velocity += gravity * subTimeInterval * 0.90;

		std::cout << velocity;

		obj->transform.translation += velocity * subTimeInterval;

		std::cout << obj->transform.translation;

		if (obj->transform.translation.y < 0)
		{
			velocity.y = -velocity.y;
		}
		

		// renderable->renderingDataNeedUpdate = true;

		// timer end
	}

	void NaiveSolver::init()
	{
		// pass
		std::cout << "[SIMULATOR] Init Naive Solver.\n";
	}

} // namespace HSim