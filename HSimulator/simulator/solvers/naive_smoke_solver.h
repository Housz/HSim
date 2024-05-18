/**
 * ref: 17 in https://matthias-research.github.io/pages/tenMinutePhysics/index.html
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
		void applyGravity();
		void applyBuoyancy();

		void updateEmitter();

		void applyBoundaryCondition();

    public:
        void setGameObject(GameObject_ptr go_);

        SimFrame currentFrame;
        double currentTime = 0.0;

        size_t numSubSteps = 1;

	// grids	
	public:

		GameObject_ptr go = nullptr;
		
		// FaceCenterGrid3 (MACGrid3, StaggeredGrid3)
		GameObject_ptr velocityGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr densityGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr temperatureGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr smokeEmitterGO = nullptr;

		// CellCenterScalarGrid3
		GameObject_ptr colliderGO = nullptr;

		

	// data
	public:

		Vec3<PRECISION> gravity = {0, -9.8, 0};

		double 

	};
	
	
} // namespace HSim
