#include <app/app.h>

void createScene(HSim::App &app)
{
    auto scene = std::make_shared<HSim::SceneGraph>();
    app.setScene(scene);

    auto root = scene->root;

    scene->addGround(20);

    /**************************************************/
    const size_t n = 64;

    auto grid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(n, n, n);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-5, 5);

    auto fillGrid = [&](size_t i, size_t j, size_t k)
    {
        int dice_roll = distribution(generator);

        (*grid)(i, j, k) = dice_roll;
    };

    grid->parallelForEachCell(fillGrid);

    auto gridMat = std::make_shared<HSim::PointMaterial>();
    // gridMat->color = {0.2, 0.2, 0.2};

    auto gridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(grid, gridMat);
    auto gridRenderable = std::make_shared<HSim::Renderable>(grid, gridGObject);

    auto go0 = std::make_shared<HSim::GameObject>();
    go0->renderable = gridRenderable;

    root->addChild(go0);

    /**************************************************/

    /**
     * solver
     */

    auto naiveFluidSolver = std::make_shared<HSim::NaiveFluidSolver>();

    naiveFluidSolver->setGameObject(go0);

    app.simulator->solver = naiveFluidSolver;
}

int main()
{
    HSim::App app;

    app.init(1920, 1080);

    createScene(app);

    app.mainLoop();

    return 0;
}