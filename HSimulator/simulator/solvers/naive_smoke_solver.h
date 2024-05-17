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

		GameObject_ptr go = nullptr;

		Vec3<PRECISION> gravity = {0, -9.8, 0};
	};
	
	
} // namespace HSim
