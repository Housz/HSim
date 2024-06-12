#pragma once

#include <HSim/common.h>
#include <HSim/linearSystem.h>
#include <HSim/math_utils.h>

#include <scene/scene_graph.h>
#include <simulator/solvers/solver.h>

#include <openvdb/openvdb.h>

#include <HSim/cell_center_scalar_grid3.h>

#define WRITEVDB

namespace HSim
{
	class JacobiPressureSolver : public Solver
	{
	public:
		JacobiPressureSolver();
		~JacobiPressureSolver();

	public:
		void update(const SimFrame &frame) override;

	public:
		void advanceTimeStep(double timeInterval);

		void advanceSubTimeStep(double subTimeInterval);

		void init();

		void writeVDB();

	public:
		void jacobiSolve();

	public:
		void setVelocityGO(const GameObject_ptr &other);
		void setScalarGO(const GameObject_ptr &other);
	
	public:
		bool isBoundary(size_t x, size_t y, size_t z);

	public:
		SimFrame currentFrame;
		double currentTime = 0.0;

		size_t numSubSteps = 1;

	// grids
	public:
		// FaceCenterGrid3 (MACGrid3, StaggeredGrid3)
		GameObject_ptr velocityGO = nullptr;
		
		// CellCenterScalarGrid3
		GameObject_ptr scalarGO = nullptr;
	
	};
	
} // namespace HSim
