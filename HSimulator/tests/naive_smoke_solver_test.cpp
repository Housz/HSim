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
	const size_t n = 32;

	auto velocityGrid = std::make_shared<HSim::FaceCenterGrid3<PRECISION>>(n, n, n);

	// auto gridMat = std::make_shared<HSim::PointMaterial>();
	// auto velocityGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(velocityGrid, gridMat);
	auto velocityGridRenderable = std::make_shared<HSim::Renderable>(velocityGrid, nullptr);

	auto velocityGO = std::make_shared<HSim::GameObject>();
	velocityGO->renderable = velocityGridRenderable;

	root->addChild(velocityGO);



	auto desityGrid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(n, n, n);

	auto gridMat = std::make_shared<HSim::PointMaterial>();
	auto desityGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(desityGrid, gridMat);
	auto desityGridenderable = std::make_shared<HSim::Renderable>(desityGrid, desityGridGObject);

	auto densityGO = std::make_shared<HSim::GameObject>();
	densityGO->renderable = desityGridenderable;

	root->addChild(densityGO);


	auto emitterGrid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(n, n, n);

	for (size_t i = 10; i < 20; i++)
	{
		for (size_t k = 10; k < 20; k++)
		{
			(*emitterGrid)(i, 0, k) = 1.0f;
		}
	}    

	auto emitterGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(emitterGrid, gridMat);
	auto emitterGridenderable = std::make_shared<HSim::Renderable>(emitterGrid, emitterGridGObject);
	
	auto emitterGO = std::make_shared<HSim::GameObject>();
	emitterGO->renderable = emitterGridenderable;

	root->addChild(emitterGO);

	/**************************************************/

	/**************************************************
	 * solver
	 */
	auto naiveSmokeSolver = std::make_shared<HSim::naiveSmokeSolver>();

	naiveSmokeSolver->setVelocityGO(velocityGO);
	naiveSmokeSolver->setDensityGO(densityGO);
	naiveSmokeSolver->setEmitterGO(emitterGO);

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