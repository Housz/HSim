#pragma once

#include <HSim/common.h>
// #include <HSim/solver.h>
#include <simulator/solvers/solver.h>
#include <scene/game_object.h>

namespace HSim
{
	template <typename T>
	class NaiveSolver : public Solver<T>
	{
	public:
		NaiveSolver(){};
		~NaiveSolver(){};

	public:
		void update(const Frame &frame) override
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

	public:
		void advanceTimeStep(double timeInterval)
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

		void advanceSubTimeStep(double subTimeInterval)
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

		void init()
		{
			// pass
			std::cout << "[SIMULATOR] Init Naive Solver.\n";
		}

	public:
		GameObject_ptr go = nullptr;
		// std::vector<GameObject_ptr> goCollection;

		Frame currentFrame;
		double currentTime = 0.0;

		size_t numSubSteps = 1;

	public:
		Vec3<T> gravity = {0, -9.8, 0}; 
		Vec3<T> velocity = {0, 0, 0};
	};

} // namespace HSim
