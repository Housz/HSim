#include <simulator/solvers/naive_smoke_solver.h>

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

	// 3 pressure (Jacobi)
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
		(*temperatureGrid)(i, j, k) += (*emitterGrid)(i, j, k) * .001;
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
			buoyancySmokeDensityFactor * density +
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
	buildLinearSystem();

	jacobiSolve();

	// integrate pressure gradient to the velocity grid

	integratePressureGradient();
}

void HSim::naiveSmokeSolver::applyAdvection(double subTimeInterval)
{
	// semi-lagrangian

	auto densityGrid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(densityGO->renderable->spaceObject);
	auto oldDensityGrid = std::make_shared<CellCenterScalarGrid3<PRECISION>>(*densityGrid);

	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);

	// advection
	// mid-point integration

	densityGrid->parallelForEachCell([&](size_t i, size_t j, size_t k)
									 {
										 auto posNow = oldDensityGrid->positionAt(i, j, k);
										 auto velNow = velocityGrid->dataAtCellCenter(i, j, k);
										 // auto posPre

										 auto densityNow = densityGrid->dataAt(i, j, k);

										 auto midPosPre = posNow - 0.5 * subTimeInterval * velNow;
										 midPosPre = clamp(midPosPre,
														   densityGrid->dataOrigin(),
														   densityGrid->gridSpacing * densityGrid->gridResolution);
										 auto midVelPre = velocityGrid->sample(midPosPre);

										 //  std::cout << posNow;
										 //  std::cout << midPosPre;

										 auto posPre = posNow - subTimeInterval * midVelPre;

										 posPre = clamp(posPre,
														densityGrid->dataOrigin(),
														densityGrid->gridSpacing * densityGrid->gridResolution);

										 //  std::cout << posPre;

										 auto density = densityGrid->sample(posPre);

										 (*densityGrid)(i, j, k) = density; });

	// auto callback = [&](size_t i, size_t j, size_t k)
	// {
	// 	if (j > 0)
	// 	{
	// 		if ((*oldDensityGrid)(i, j - 1, k) > 0)
	// 		{
	// 			(*desityGrid)(i, j, k) = (*oldDensityGrid)(i, j - 1, k);
	// 		}
	// 	}
	// };

	// desityGrid->parallelForEachCell(callback);
}

void HSim::naiveSmokeSolver::buildLinearSystem()
{

	// buildMarkers todo

	// buildLinearSystem Ax = b
	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);

	linearSystem.clear();
	linearSystem.resize(velocityGrid->gridResolution);

	auto &A = linearSystem.A;
	auto &b = linearSystem.b;

	A.parallelForEachCell(
		[&](size_t i, size_t j, size_t k)
		{
			auto &row = A(i, j, k);

			// init
			row.center = row.right = row.up = row.front = 0.;
			b(i, j, k) = 0.;

			if (isBoundary(i, j, k))
			{
				row.center = 1.;
				return;
			}

			b(i, j, k) = velocityGrid->divergenceAtCellCenter(i, j, k);

			double invSpacingX2 = 1. / (velocityGrid->gridSpacing.x * velocityGrid->gridSpacing.x);
			double invSpacingY2 = 1. / (velocityGrid->gridSpacing.y * velocityGrid->gridSpacing.y);
			double invSpacingZ2 = 1. / (velocityGrid->gridSpacing.z * velocityGrid->gridSpacing.z);

			// x
			if (isBoundary(i - 1, j, k) && isBoundary(i + 1, j, k))
			{
			}
			else if (isBoundary(i - 1, j, k) || isBoundary(i + 1, j, k))
			{
				row.center += invSpacingX2;
				row.right = -invSpacingX2;
			}
			else // (i-1, j, k) (i+1, j, k) all fluid
			{
				row.center += 2 * invSpacingX2;
				row.right = -invSpacingX2;
			}

			// y
			if (isBoundary(i, j - 1, k) && isBoundary(i, j + 1, k))
			{
			}
			else if (isBoundary(i, j - 1, k) || isBoundary(i, j + 1, k))
			{
				row.center += invSpacingY2;
				row.up = -invSpacingY2;
			}
			else // (i-1, j, k) (i+1, j, k) all fluid
			{
				row.center += 2 * invSpacingY2;
				row.up = -invSpacingY2;
			}

			// z
			if (isBoundary(i, j - 1, k) && isBoundary(i, j + 1, k))
			{
			}
			else if (isBoundary(i, j - 1, k) || isBoundary(i, j + 1, k))
			{
				row.center += invSpacingZ2;
				row.front = -invSpacingZ2;
			}
			else // (i-1, j, k) (i+1, j, k) all fluid
			{
				row.center += 2 * invSpacingZ2;
				row.front = -invSpacingZ2;
			}
		});
}

void HSim::naiveSmokeSolver::jacobiSolve()
{
	auto &A = linearSystem.A;
	auto &x = linearSystem.x;
	auto &b = linearSystem.b;

	const size_t MAX_NUM_ITERATIONS = 100;
	const size_t CHECK_INTERVAL = 10;
	const double TOLERANCE = 1e-8;

	auto size = A.size;

	PossionVector3 tempX;
	tempX.resize(x.size);

	PossionVector3 residual;
	residual.resize(x.size);

	for (size_t iter = 0; iter < MAX_NUM_ITERATIONS; iter++)
	{
		// jacobi relax
		A.parallelForEachCell(
			[&](size_t i, size_t j, size_t k)
			{
				// double r =
				// 	((i == 0) ? A(i - 1, j, k).right * x(i - 1, j, k) : 0.0) +
				// 	((i == size.x) ? A(i, j, k).right * x(i + 1, j, k) : 0.0) +
				// 	((j == 0) ? A(i, j - 1, k).up * x(i, j - 1, k) : 0.0) +
				// 	((j == size.y) ? A(i, j, k).up * x(i, j + 1, k) : 0.0) +
				// 	((k == 0) ? A(i, j, k - 1).front * x(i, j, k - 1) : 0.0) +
				// 	((k == size.z) ? A(i, j, k).front * x(i, j, k + 1) : 0.0);

				double r =
					((i > 0) ? A(i - 1, j, k).right * x(i - 1, j, k) : 0.0) +
					((i < size.x - 1) ? A(i, j, k).right * x(i + 1, j, k) : 0.0) +
					((j > 0) ? A(i, j - 1, k).up * x(i, j - 1, k) : 0.0) +
					((j < size.y - 1) ? A(i, j, k).up * x(i, j + 1, k) : 0.0) +
					((k > 0) ? A(i, j, k - 1).front * x(i, j, k - 1) : 0.0) +
					((k < size.z - 1) ? A(i, j, k).front * x(i, j, k + 1) : 0.0);

				// tempX(i, j, k) = (b(i, j, k) - r) / A(i, j, k).center;
				tempX(i, j, k) = (b(i, j, k) - r) / A(i, j, k).center;
			});

		std::swap(x._data, tempX._data);

		// check residual
		if (iter != 0 && iter % CHECK_INTERVAL == 0)
		{
			// compute current residual
			linearSystem3Residual(linearSystem, residual);

			double l2Norm = possionVector3L2Norm(residual);

			// break jacobi iterations
			if (l2Norm < TOLERANCE)
			{
				std::cout << "[OK] " << "iter: " << iter << " residual: " << l2Norm << "\n";
				// break;
				return;
			}
		}
	}

	linearSystem3Residual(linearSystem, residual);
	double l2Norm = possionVector3L2Norm(residual);

	if (l2Norm < TOLERANCE)
	{
		std::cout << "[OK] " << "iter: " << MAX_NUM_ITERATIONS << " residual: " << l2Norm << "\n";
	}
	else
	{
		std::cout << "[FAIL] " << "iter: " << MAX_NUM_ITERATIONS << " residual: " << l2Norm << "\n";
	}
}

void HSim::naiveSmokeSolver::integratePressureGradient()
{
	auto &x = linearSystem.x;

	auto size = x.size;

	auto velocityGrid = std::static_pointer_cast<FaceCenterGrid3<PRECISION>>(velocityGO->renderable->spaceObject);

	auto &u = velocityGrid->dataV();
	auto &v = velocityGrid->dataV();
	auto &w = velocityGrid->dataV();

	double invSpacingX = 1. / velocityGrid->gridSpacing.x;
	double invSpacingY = 1. / velocityGrid->gridSpacing.y;
	double invSpacingZ = 1. / velocityGrid->gridSpacing.z;

	x.parallelForEachCell(

		[&](size_t i, size_t j, size_t k)
		{
			if (isBoundary(i, j, k))
			{
				return;
			}

			if (!isBoundary(i + 1, j, k))
			{
				if (i == size.x-1)
				{
					std::cout << "err";
				}
				
				u(i + 1) += invSpacingX * (x(i + 1, j, k) - x(i, j, k));
			}

			if (!isBoundary(i, j + 1, k))
			{
				if (j == size.y-1)
				{
					std::cout << "err";
				}
				
				v(i + 1) += invSpacingY * (x(i, j + 1, k) - x(i, j, k));
			}

			if (!isBoundary(i, j, k + 1))
			{
				if (k == size.z-1)
				{
					std::cout << "err";
				}
				w(i + 1) += invSpacingZ * (x(i, j, k + 1) - x(i, j, k));
			}
		}

	);
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

bool HSim::naiveSmokeSolver::isCollider(size_t x, size_t y, size_t z)
{
	// todo
	return false;
}
