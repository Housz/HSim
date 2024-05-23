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

			std::this_thread::sleep_for(std::chrono::milliseconds((int)(frame.timeInterval * 10000) - du));
		}

		currentFrame = frame;

		std::cout << "[FRAME] " << currentFrame.index << "\n";
	}
}

void HSim::naiveSmokeSolver::advanceTimeStep(double timeInterval)
{
	auto subTimeInterval = timeInterval / numSubSteps;

	/******************************************************
	 * before a time step
	 */

	updateEmitter();

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

	// applyDiffusion()

	// update rendering states
	densityGO->renderable->renderingDataNeedUpdate = true;

	// auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);
	// auto velocityY = velocityGrid->dataV();
	// std::cout << velocityY.dataAt(0, 0, 0) << "\n";

	writeVDB();
}

void HSim::naiveSmokeSolver::advanceSubTimeStep(double subTimeInterval)
{
	// 1 external forces (gravity, buoyancy force, other ex forces)
	// applyGravity(subTimeInterval);
	applyBuoyancy(subTimeInterval);

	// 2 viscosity (todo)

	// 3 pressure (Gauss-Seidel)
	applyPressure(subTimeInterval);

	// 4 advection (Semi-Lagrangian)
	applyAdvection(subTimeInterval);
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
	auto densityGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(densityGO->renderable->spaceObject);

	openvdb::FloatGrid::Ptr vdbGrid = openvdb::FloatGrid::create();

	openvdb::FloatGrid::Accessor accessor = vdbGrid->getAccessor();

	// auto callback = [&](size_t i, size_t j, size_t k)
	// {
	// 	openvdb::Coord ijk(i, j, k);
	// 	accessor.setValue(ijk, densityGrid->dataAt(i, j, k));
	// };

	for (size_t k = 0; k < densityGrid->sizeZ(); k++)
	{
		for (size_t j = 0; j < densityGrid->sizeY(); j++)
		{
			for (size_t i = 0; i < densityGrid->sizeX(); i++)
			{
				openvdb::Coord ijk(i, j, k);
				accessor.setValue(ijk, densityGrid->dataAt(i, j, k));
			}
		}
	}

	openvdb::GridPtrVec vdbGrids;
	vdbGrids.push_back(vdbGrid);

	openvdb::io::File file("grid_" + std::to_string(currentFrame.index) + ".vdb");
	file.write(vdbGrids);
	file.close();
#endif // WRITEVDB
}

void HSim::naiveSmokeSolver::applyGravity(double subTimeInterval)
{
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);

	auto &velocityY = velocityGrid->dataV();

	velocityY.parallelForEach([&](PRECISION &vy)
							  { vy += subTimeInterval * gravity.y; });
}

void HSim::naiveSmokeSolver::updateEmitter()
{
	auto emitterGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(emitterGO->renderable->spaceObject);
	auto desityGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(densityGO->renderable->spaceObject);
	auto temperatureGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(temperatureGO->renderable->spaceObject);

	auto callback = [&](size_t i, size_t j, size_t k)
	{
		// density
		(*desityGrid)(i, j, k) += (*emitterGrid)(i, j, k);

		// temperature
		// (*temperatureGrid)(i, j, k) +=
	};

	emitterGrid->parallelForEachCell(callback);
}

void HSim::naiveSmokeSolver::applyBuoyancy(double subTimeInterval)
{
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);
	auto &velocityY = velocityGrid->dataV();

	auto desityGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(densityGO->renderable->spaceObject);
	auto temperatureGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(temperatureGO->renderable->spaceObject);

	// ambient temperature
	double ambTemperature =
		tbb::parallel_reduce(tbb::blocked_range<size_t>(0, temperatureGrid->_data.size()), 0, [&](tbb::blocked_range<size_t> r, double avg_local)
							 {
			for (size_t i = r.begin(); i != r.end(); i++)
			{
				avg_local += temperatureGrid->_data[i];
			}
			return avg_local; }, std::plus<double>());
	ambTemperature /= temperatureGrid->_data.size();


	auto callback = [&](size_t i, size_t j, size_t k)
	{
		auto density = (*desityGrid)(i, j, k);
		auto temperature = (*temperatureGrid)(i, j, k);

		// buoyancy force in i, j, k
		double buoyancyForce = 
			buoyancySmokeDensityFactor * density
			+
			buoyancyTemperatureFactor * (temperature - ambTemperature);

		velocityY(i, j, k) += subTimeInterval * buoyancyForce;
	};

	desityGrid->parallelForEachCell(callback);
}

void HSim::naiveSmokeSolver::applyBoundaryCondition()
{
	//
}

void HSim::naiveSmokeSolver::applyPressure(double subTimeInterval)
{
	// velOld = vel.clone
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);
	auto oldVelocityGrid = std::make_shared<FaceCenterGrid3<PRECISION>>(*velocityGrid);

	// solve pressure ( possion eqs: div v = - div grad p )
	// using parallel jacobi iteration

	// p_x-1 p_x p_x+1 : -1 2 -1


	// apply pressure to integrate velocity

}

void HSim::naiveSmokeSolver::applyAdvection(double subTimeInterval)
{
	// semi-lagrangian

	auto desityGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(densityGO->renderable->spaceObject);

	auto oldDensityGrid = std::make_shared<CellCenterScalarGrid3<PRECISION>>(*desityGrid);

	auto callback = [&](size_t i, size_t j, size_t k)
	{
		if (j > 0)
		{
			if ((*oldDensityGrid)(i, j - 1, k) > 0)
			{
				(*desityGrid)(i, j, k) = (*oldDensityGrid)(i, j - 1, k);
			}
		}
	};

	desityGrid->parallelForEachCell(callback);
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

bool HSim::naiveSmokeSolver::isBoundary(size_t x, size_t y, size_t z)
{
    // boundary: cells at edge of grid
	// x, y, z: cell index

	auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(densityGO->renderable->spaceObject);

	if (x == 0 || y == 9 || z == 0 || 
		x == grid->sizeX()-1 || y == grid->sizeY()-1 || z == grid->sizeZ()-1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
