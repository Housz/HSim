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
		// (*scalarGrid)(i, j, k) = velocityGrid->divergenceAtCellCenter(i, j, k);
	};

	scalarGrid->parallelForEachCell(updatescalarCallback);


	// new vel
	for (size_t i = 6; i < 9; i++)
	{
		for (size_t j = 6; j < 9; j++)
		{
			for (size_t k = 6; k < 9; k++)
			{
				velocityGrid->u(i, j, k) = 1;
				velocityGrid->v(i, j, k) = 1;
				velocityGrid->v(i, j, k) = 1;
			}
		}
	}
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
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);
	auto velocityGrid_old = std::make_shared<FaceCenterGrid3<PRECISION>>(*velocityGrid);

	auto &u = velocityGrid_old->dataU();
	auto &v = velocityGrid_old->dataV();
	auto &w = velocityGrid_old->dataW();
	
	auto stencil = [&](size_t i, size_t j, size_t k)
	{
		if (isBoundary(i, j, k))
		{
			return;
		}

		size_t n = 			
			(isBoundary(i + 1, j, k) ? 0 : 1) +
			(isBoundary(i - 1, j, k) ? 0 : 1) +
			(isBoundary(i, j + 1, k) ? 0 : 1) +
			(isBoundary(i, j - 1, k) ? 0 : 1) +
			(isBoundary(i, j, k + 1) ? 0 : 1) +
			(isBoundary(i, j, k - 1) ? 0 : 1);
		
		double div = 
			(isBoundary(i + 1, j, k) ? 0 : u(i + 1, j, k)) +
			(isBoundary(i - 1, j, k) ? 0 : -u(i, j, k)) +
			(isBoundary(i, j + 1, k) ? 0 : v(i, j + 1, k)) +
			(isBoundary(i, j - 1, k) ? 0 : -v(i, j, k)) +
			(isBoundary(i, j, k + 1) ? 0 : w(i, j, k + 1)) +
			(isBoundary(i, j, k - 1) ? 0 : -w(i, j, k));
		
		div /= n;

		if (!isBoundary(i + 1, j, k))
			velocityGrid->u(i + 1, j, k) -= div;
		if (!isBoundary(i - 1, j, k))
			velocityGrid->u(i, j, k) += div;

		if (!isBoundary(i, j + 1, k))
			velocityGrid->v(i, j + 1, k) -= div;
		if (!isBoundary(i, j - 1, k))
			velocityGrid->v(i, j, k) += div;
			
		if (!isBoundary(i, j, k + 1))
			velocityGrid->w(i, j, k + 1) -= div;
		if (!isBoundary(i, j, k - 1))
			velocityGrid->w(i, j, k) += div;
		
	};

	double res = 0.;
	auto residualCallback = [&](size_t i, size_t j, size_t k)
	{
		auto div = velocityGrid->divergenceAtCellCenter(i, j, k);
		res += div * div;
	};


	const size_t MAX_ITERATIONS = 1000000;
	for (size_t i = 0; i < MAX_ITERATIONS; i++)
	{
		velocityGrid->parallelForEachCell(stencil);

		res = 0.;
		velocityGrid->forEachCell(residualCallback);

		if (res < 1e-8)
		{
			std::cout << "DONE iters = " << i << std::endl;
			break;
		}
		

		// velocityGrid->swap(*velocityGrid_old);
		velocityGrid_old->dataU()._data = velocityGrid->dataU()._data;
		velocityGrid_old->dataV()._data = velocityGrid->dataV()._data;
		velocityGrid_old->dataW()._data = velocityGrid->dataW()._data;
	}
	std::cout << "RES: " << res << std::endl;
	

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