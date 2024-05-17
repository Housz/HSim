/**
 * for testing fluid grid update, rendering, wirteVDB...
 * 
 */

#pragma once

#include <HSim/common.h>
#include <scene/scene_graph.h>
#include <simulator/solvers/solver.h>

#include <openvdb/openvdb.h>

#include <HSim/cell_center_scalar_grid3.h>

#define WRITEVDB

namespace HSim
{
    class NaiveFluidSolver : public Solver
    {
    public:
        NaiveFluidSolver();
        ~NaiveFluidSolver();

    public:
        void update(const SimFrame &frame) override;

    public:
        void advanceTimeStep(double timeInterval);

        void advanceSubTimeStep(double subTimeInterval);

        void init();

        void writeVDB();

    public:
        void setGameObject(GameObject_ptr go_);

        SimFrame currentFrame;
        double currentTime = 0.0;

        size_t numSubSteps = 1;

    public:
        // CellCenterScalarGrid3_Ptr<PRECISION> grid = nullptr;

        GameObject_ptr go = nullptr;
    };

    using NaiveFluidSolver_Ptr = std::shared_ptr<NaiveFluidSolver>;

} // namespace HSim
