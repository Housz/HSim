#include <app/app.h>

void createScene(HSim::App &app)
{
	auto scene = std::make_shared<HSim::SceneGraph>();
	app.setScene(scene);

	auto root = scene->root;

	scene->addGround(64);

	/**************************************************
	 * scene
	 */

	// HSim::Size3 gridResolution = {32, 32, 32};
	HSim::Size3 gridResolution = {10, 10, 10};
	// HSim::Vec3d gridOrigin = {-16, -16, -16};
	HSim::Vec3d gridOrigin = {0, 0, 0};

	auto velocityGrid = std::make_shared<HSim::FaceCenterGrid3<PRECISION>>(gridResolution);
	velocityGrid->setGridOrigin(gridOrigin);

	auto velGridMat = std::make_shared<HSim::LineFieldMaterial>();
	auto velocityGridGObject = std::make_shared<HSim::FaceCenterGrid3GObject>(velocityGrid, velGridMat);
	auto velocityGridRenderable = std::make_shared<HSim::Renderable>(velocityGrid, velocityGridGObject);

	auto velocityGO = std::make_shared<HSim::GameObject>();
	velocityGO->renderable = velocityGridRenderable;

	root->addChild(velocityGO);

	// for (size_t i = 15; i < 19; i++)
	// {
	// 	for (size_t j = 15; j < 19; j++)
	// 	{
	// 		for (size_t k = 15; k < 19; k++)
	// 		{
	// 			velocityGrid->u(i, j, k) = 1;
	// 			velocityGrid->v(i, j, k) = 1;
	// 			velocityGrid->v(i, j, k) = 1;
	// 		}
	// 	}
	// }

	// scalergrid for rendering velgrid scaler

	auto scalarGrid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(gridResolution);
	scalarGrid->setGridOrigin(gridOrigin);

	auto gridMat = std::make_shared<HSim::PointMaterial>();
	auto scalarGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(scalarGrid, gridMat);
	auto scalarGridenderable = std::make_shared<HSim::Renderable>(scalarGrid, scalarGridGObject);

	auto scalarGO = std::make_shared<HSim::GameObject>();
	scalarGO->renderable = scalarGridenderable;

	root->addChild(scalarGO);


	/**************************************************
	 * solver
	 */

	auto jacobiPressureSolver = std::make_shared<HSim::JacobiPressureSolver>();
	jacobiPressureSolver->setVelocityGO(velocityGO);
	jacobiPressureSolver->setScalarGO(scalarGO);

	app.simulator->solver = jacobiPressureSolver;

}

int main()
{
	HSim::App app;

	app.init(1920, 1080);

	createScene(app);

	app.mainLoop();

	return 0;
}