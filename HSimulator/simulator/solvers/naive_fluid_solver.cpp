#include <simulator/solvers/naive_fluid_solver.h>
#include "naive_fluid_solver.h"

HSim::NaiveFluidSolver::NaiveFluidSolver()
{
}

HSim::NaiveFluidSolver::~NaiveFluidSolver()
{
}

void HSim::NaiveFluidSolver::update(const SimFrame &frame)
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

void HSim::NaiveFluidSolver::advanceTimeStep(double timeInterval)
{
    auto subTimeInterval = timeInterval / numSubSteps;

    for (size_t i = 0; i < numSubSteps; i++)
    {
        // timer start
        advanceSubTimeStep(subTimeInterval);
        // timer end

        currentTime += subTimeInterval;
    }

    go->renderable->renderingDataNeedUpdate = true;


    writeVDB();
}

void HSim::NaiveFluidSolver::advanceSubTimeStep(double subTimeInterval)
{
    auto renderable = go->renderable;

    auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(go->renderable->spaceObject);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-5, 5);

    auto fillGrid = [&](size_t i, size_t j, size_t k)
    {
        int dice_roll = distribution(generator);

        (*grid)(i, j, k) = dice_roll;
    };

    grid->parallelForEachCell(fillGrid);
}

void HSim::NaiveFluidSolver::init()
{
    openvdb::initialize();

    std::cout << "[SIMULATOR] Init Naive Fluid Solver.\n";
}



void HSim::NaiveFluidSolver::setGameObject(GameObject_ptr go_)
{
    go = go_;

    go->renderable->updateType = RenderableUpdateType::DYNAMIC;
}


void HSim::NaiveFluidSolver::writeVDB()
{
    std::chrono::high_resolution_clock clk;
    auto BEGIN_TIME = clk.now();

    auto grid = std::static_pointer_cast<CellCenterScalarGrid3<PRECISION>>(go->renderable->spaceObject);

    openvdb::FloatGrid::Ptr vdbGrid = openvdb::FloatGrid::create();

    openvdb::FloatGrid::Accessor accessor = vdbGrid->getAccessor();

    auto callback = [&](size_t i, size_t j, size_t k)
    {
        openvdb::Coord ijk(i, j, k);
        accessor.setValue(ijk, grid->dataAt(i, j, k));
    };

    grid->forEachCell(callback);

    openvdb::GridPtrVec vdbGrids;
    vdbGrids.push_back(vdbGrid);

    auto du = std::chrono::duration_cast<std::chrono::milliseconds>(clk.now() - BEGIN_TIME).count();

    // std::cout << "VDB grid build: " << du << "\n";

    BEGIN_TIME = clk.now();

    openvdb::io::File file("grid_" + std::to_string(currentFrame.index) + ".vdb");
    file.write(vdbGrids);
    file.close();

    du = std::chrono::duration_cast<std::chrono::milliseconds>(clk.now() - BEGIN_TIME).count();

    // std::cout << "VDB grid write: " << du << "\n";
}