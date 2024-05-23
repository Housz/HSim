/**
 * ref:
 * [1] 17 in https://matthias-research.github.io/pages/tenMinutePhysics/index.html
 * 
 * [2] Fedkiw, Ronald, Jos Stam, and Henrik Wann Jensen. "Visual simulation of smoke." Proceedings of the 28th annual conference on Computer graphics and interactive techniques. 2001.
 * 
 * pressure solver: Gauss-Seidel method
 *
 * adevect solver: semi-lagrangian
 *
 */

#pragma once

#include <HSim/common.h>
#include <scene/scene_graph.h>
#include <simulator/solvers/solver.h>

#include <openvdb/openvdb.h>

#include <HSim/cell_center_scalar_grid3.h>

// #define WRITEVDB

namespace HSim
{
	class naiveSmokeSolver : public Solver
	{
	public:
		naiveSmokeSolver();
		~naiveSmokeSolver();

	public:
		void update(const SimFrame &frame) override;

	public:
		void advanceTimeStep(double timeInterval);

		void advanceSubTimeStep(double subTimeInterval);

		void init();

		void writeVDB();

	public:
		void updateEmitter();

		void applyGravity(double subTimeInterval);
		void applyBuoyancy(double subTimeInterval);

		void applyBoundaryCondition();

		void applyPressure(double subTimeInterval);

		void applyAdvection(double subTimeInterval);

	public:
		void setVelocityGO(const GameObject_ptr &other);
		void setDensityGO(const GameObject_ptr &other);
		void setTemperatureGO(const GameObject_ptr &other);
		void setEmitterGO(const GameObject_ptr &other);
		void setColliderGO(const GameObject_ptr &other);

	public:
		SimFrame currentFrame;
		double currentTime = 0.0;

		size_t numSubSteps = 1;

		// grids
	public:
		// FaceCenterGrid3 (MACGrid3, StaggeredGrid3)
		GameObject_ptr velocityGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr densityGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr temperatureGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr emitterGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr colliderGO = nullptr;

		// data
	public:
		Vec3<PRECISION> gravity = {0, -9.8, 0};

		// alpha in [2] eq. 8
		const double buoyancySmokeDensityFactor = -0.000625; 
		// beta in [2] eq. 8
		const double buoyancyTemperatureFactor = 5.0;
	};

} // namespace HSim
