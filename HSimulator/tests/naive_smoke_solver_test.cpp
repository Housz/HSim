#include <app/app.h>

void createScene(HSim::App &app)
{
    auto scene = std::make_shared<HSim::SceneGraph>();
    app.setScene(scene);

    auto root = scene->root;

    scene->addGround(100);

    /**************************************************
     * scene
     */
    const size_t n = 64;
    auto velocityGrid = std::make_shared<HSim::CellCenterScalarGrid3<float>>(n, n, n);

    auto gridMat = std::make_shared<HSim::PointMaterial>();
    auto velocityGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(velocityGrid, gridMat);
    auto velocityGridRenderable = std::make_shared<HSim::Renderable>(velocityGrid, velocityGridGObject);

    auto velocityGO = std::make_shared<HSim::GameObject>();
    velocityGO->renderable = velocityGridRenderable;

    root->addChild(velocityGO);

    /**************************************************/

    /**************************************************
     * solver
     */
    auto naiveSmokeSolver = std::make_shared<HSim::naiveSmokeSolver>();

    naiveSmokeSolver->setVelocityGO(velocityGO);


    app.simulator->solver = naiveSmokeSolver;

    /**************************************************/
}

int main()
{
    HSim::App app;

    app.init(1920, 1080);

    createScene(app);

    app.mainLoop();

    return 0;
}