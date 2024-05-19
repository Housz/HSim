#include <simulator/solvers/naive_smoke_solver.h>
#include "naive_smoke_solver.h"

HSim::naiveSmokeSolver::naiveSmokeSolver()
{
}

HSim::naiveSmokeSolver::~naiveSmokeSolver()
{
}

void HSim::naiveSmokeSolver::update(const SimFrame &frame)
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

			// std::cout << "advanceTimeStep " << du << "\n";
			// std::cout << "sleep  " << (int)(frame.timeInterval * 1000) - du << "\n";

			std::this_thread::sleep_for(std::chrono::milliseconds((int)(frame.timeInterval * 1000) - du));
		}

		currentFrame = frame;
	}
}

void HSim::naiveSmokeSolver::advanceTimeStep(double timeInterval)
{
	auto subTimeInterval = timeInterval / numSubSteps;

	/******************************************************
	 * before a time step
	 */

	// updateEmitter()


	/******************************************************
	 * advance a time step
	 */
	for (size_t i = 0; i < numSubSteps; i++)
	{
		// timer start
		advanceSubTimeStep(subTimeInterval);
		// timer end

		currentTime += subTimeInterval;
	}

	/******************************************************
	 * after a time step
	 */

	go->renderable->renderingDataNeedUpdate = true;

	writeVDB();
}

void HSim::naiveSmokeSolver::advanceSubTimeStep(double subTimeInterval)
{
	auto renderable = go->renderable;

	auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(go->renderable->spaceObject);

	// 1 external forces (gravity, buoyancy force, other ex forces)


	// 2 viscosity (pass)

	// 3 pressure (Gauss-Seidel)

	// 4 advection (Semi-Lagrangian)
}

void HSim::naiveSmokeSolver::init()
{
	openvdb::initialize();

	std::cout << "[SIMULATOR] Init Naive Fluid Solver.\n";
}


// void HSim::naiveSmokeSolver::setGameObject(GameObject_ptr go_)
// {
// 	go = go_;

// 	go->renderable->updateType = RenderableUpdateType::DYNAMIC;
// }

void HSim::naiveSmokeSolver::writeVDB()
{
#ifdef WRITEVDB
	auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(go->renderable->spaceObject);

	openvdb::FloatGrid::Ptr vdbGrid = openvdb::FloatGrid::create();

	openvdb::FloatGrid::Accessor accessor = vdbGrid->getAccessor();

	auto callback = [&](size_t i, size_t j, size_t k)
	{
		openvdb::Coord ijk(i, j, k);
		accessor.setValue(ijk, grid->dataAt(i, j, k));
	};

	openvdb::GridPtrVec vdbGrids;
	vdbGrids.push_back(vdbGrid);

	openvdb::io::File file("grid_" + std::to_string(currentFrame.index) + ".vdb");
	file.write(vdbGrids);
	file.close();
#endif // WRITEVDB
}

void HSim::naiveSmokeSolver::applyGravity()
{
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->graphicsObject);

	auto velocityY = velocityGrid->dataV();

	// velocittY. parallel for

}

void HSim::naiveSmokeSolver::updateEmitter()
{
	// parallel for
	// cell(ijk) if > 0
	// desity(ijk) +=
	// temperature(ijk) +=
}

void HSim::naiveSmokeSolver::applyBuoyancy()
{
	// all vY +=
}

void HSim::naiveSmokeSolver::applyBoundaryCondition()
{
	// 
}

void HSim::naiveSmokeSolver::applyPressure()
{
	// velOld = vel.clone


}

void HSim::naiveSmokeSolver::applyAdvection()
{
	// semi-lagrangian

	
}

void HSim::naiveSmokeSolver::setVelocityGO(const GameObject_ptr &other)
{
	velocityGO = other;

	velocityGO->renderable->updateType = RenderableUpdateType::DYNAMIC;

	velocityGO->renderable->visible = false;
}

void HSim::naiveSmokeSolver::setDensityGO(const GameObject_ptr &other)
{
	densityGO = other;

	densityGO->renderable->updateType = RenderableUpdateType::DYNAMIC;

	densityGO->renderable->visible = true;

}

void HSim::naiveSmokeSolver::setTemperatureGO(const GameObject_ptr &other)
{
	temperatureGO = other;

	temperatureGO->renderable->updateType = RenderableUpdateType::DYNAMIC;

	temperatureGO->renderable->visible = false;

}

void HSim::naiveSmokeSolver::setEmitterGO(const GameObject_ptr &other)
{
	emitterGO = other;

	emitterGO->renderable->updateType = RenderableUpdateType::DYNAMIC;

	emitterGO->renderable->visible = false;
}

void HSim::naiveSmokeSolver::setColliderGO(const GameObject_ptr &other)
{
	colliderGO = other;

	colliderGO->renderable->updateType = RenderableUpdateType::DYNAMIC;

	colliderGO->renderable->visible = false;
}
