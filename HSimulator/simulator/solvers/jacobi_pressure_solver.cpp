#include <simulator/solvers/jacobi_pressure_solver.h>

HSim::JacobiPressureSolver::JacobiPressureSolver()
{
}

HSim::JacobiPressureSolver::~JacobiPressureSolver()
{
}

void HSim::JacobiPressureSolver::update(const SimFrame &frame)
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

			std::this_thread::sleep_for(std::chrono::milliseconds((int)(frame.timeInterval * 1000) - du));
		}

		currentFrame = frame;

		std::cout << "[FRAME] " << currentFrame.index << "\n";
	}
}

void HSim::JacobiPressureSolver::advanceTimeStep(double timeInterval)
{
	auto subTimeInterval = timeInterval / numSubSteps;

	/******************************************************
	 * before a time step
	 */


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

	// rendering
	scalarGO->renderable->renderingDataNeedUpdate = true;

	// writeVDB();
}

void HSim::JacobiPressureSolver::advanceSubTimeStep(double subTimeInterval)
{
	jacobiSolve();

	// for rendering
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);
	auto scalarGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(scalarGO->renderable->spaceObject);

	auto updatescalarCallback = [&](size_t i, size_t j, size_t k)
	{
		(*scalarGrid)(i, j, k) = velocityGrid->dataAtCellCenter(i, j, k).length();
	};

	scalarGrid->parallelForEachCell(updatescalarCallback);
}

void HSim::JacobiPressureSolver::init()
{
	openvdb::initialize();

	std::cout << "[SIMULATOR] Init Naive Fluid Solver.\n";
}

void HSim::JacobiPressureSolver::writeVDB()
{
#ifdef WRITEVDB
	auto scalarGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(scalarGO->renderable->spaceObject);

	openvdb::FloatGrid::Ptr vdbGrid = openvdb::FloatGrid::create();

	openvdb::FloatGrid::Accessor accessor = vdbGrid->getAccessor();

	for (size_t k = 0; k < scalarGrid->sizeZ(); k++)
	{
		for (size_t j = 0; j < scalarGrid->sizeY(); j++)
		{
			for (size_t i = 0; i < scalarGrid->sizeX(); i++)
			{
				openvdb::Coord ijk(i, j, k);
				accessor.setValue(ijk, scalarGrid->dataAt(i, j, k));
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

void HSim::JacobiPressureSolver::jacobiSolve()
{

}

void HSim::JacobiPressureSolver::setVelocityGO(const GameObject_ptr &other)
{
	velocityGO = other;
	velocityGO->renderable->updateType = RenderableUpdateType::DYNAMIC;
	velocityGO->renderable->visible = false;
}

void HSim::JacobiPressureSolver::setScalarGO(const GameObject_ptr &other)
{
	scalarGO = other;
	scalarGO->renderable->updateType = RenderableUpdateType::DYNAMIC;
	scalarGO->renderable->visible = true;
}

bool HSim::JacobiPressureSolver::isBoundary(size_t x, size_t y, size_t z)
{

	auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(scalarGO->renderable->spaceObject);

	if (x <= 0 || y <= 0 || z <= 0 ||
		x >= grid->sizeX() - 1 || y >= grid->sizeY() - 1 || z >= grid->sizeZ() - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}