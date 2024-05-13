#include <simulator/solvers/naive_solver.h>

namespace HSim
{
	NaiveSolver::NaiveSolver()
	{
	}

	NaiveSolver::~NaiveSolver()
	{
	}

	void NaiveSolver::update(const SimFrame &frame)
	{
		if (frame.index > currentFrame.index)
		{
			auto numFrames = frame.index - currentFrame.index;

			std::chrono::high_resolution_clock clk;

			for (size_t i = 0; i < numFrames; i++)
			{
				auto BEGIN_TIME = clk.now();

				advanceTimeStep(frame.timeInterval);

				auto du = std::chrono::duration_cast<std::chrono::milliseconds>(clk.now() - BEGIN_TIME).count();
				// std::cout << du << "\n";

				// std::this_thread::sleep_for(std::chrono::milliseconds((int)timeInterval * 1000 - du));
				std::this_thread::sleep_for(std::chrono::milliseconds((int)(frame.timeInterval * 1000) - du));
			}

			currentFrame = frame;
		}

		// push sim results to renderer frame buffer
	}

	void NaiveSolver::writeRendererBuffer()
	{
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

		// go->renderable->renderingDataNeedUpdate = true;
	}

	void NaiveSolver::advanceSubTimeStep(double subTimeInterval)
	{
		// timer start

		auto renderable = go->renderable;

		auto obj = std::static_pointer_cast<Surface3<PRECISION>>(go->renderable->spaceObject);

		// obj->transform.translation.y -= 0.0001;

		velocity += gravity * subTimeInterval * 0.90;

		// std::cout << velocity;

		obj->transform.translation += velocity * subTimeInterval;

		// std::cout << obj->transform.translation;

		if (obj->transform.translation.y < 0)
		{
			velocity.y = -velocity.y;
		}

		auto mat = std::static_pointer_cast<BasicMaterial>(renderable->graphicsObject->material);
		mat->color = {obj->transform.translation.y / 10, 0.4, 0.4};

		// renderable->renderingDataNeedUpdate = true;

		// timer end
	}

	void NaiveSolver::init()
	{
		// pass
		std::cout << "[SIMULATOR] Init Naive Solver.\n";
	}

	void NaiveSolver::setGameObject(GameObject_ptr go_)
	{
		go = go_;

		go->renderable->updateType = RenderableUpdateType::RIGID;
	}

} // namespace HSim