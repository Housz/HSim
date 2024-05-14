#include <simulator/solvers/naive_fluid_solver.h>

HSim::NaiveFluidSolver::NaiveFluidSolver()
{
}

HSim::NaiveFluidSolver::~NaiveFluidSolver()
{
}

void HSim::NaiveFluidSolver::update(const SimFrame &frame)
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
            // std::this_thread::sleep_for(std::chrono::milliseconds((int)(frame.timeInterval * 1000) - du));
        }

        currentFrame = frame;
    }
}

void HSim::NaiveFluidSolver::advanceTimeStep(double timeInterval)
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

void HSim::NaiveFluidSolver::advanceSubTimeStep(double subTimeInterval)
{
	auto renderable = go->renderable;

	auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(go->renderable->spaceObject);

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-5, 5);

	auto fillGrid = [&](size_t i, size_t j, size_t k)
	{
		int dice_roll = distribution(generator);

		(*grid)(i, j, k) = dice_roll;
	};

    grid->parallelForEachCell(fillGrid);

}

void HSim::NaiveFluidSolver::init()
{
    // pass
    std::cout << "[SIMULATOR] Init Naive Fluid Solver.\n";
}

void HSim::NaiveFluidSolver::setGameObject(GameObject_ptr go_)
{
    go = go_;

    go->renderable->updateType = RenderableUpdateType::DYNAMIC;
}
