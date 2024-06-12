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
	const size_t n = 128;
	// HSim::Size3 gridResolution = {64, 64, 64};
	HSim::Size3 gridResolution = {32, 32, 32};

    // HSim::Vec3i gridResolution = {n, n, n};
    // HSim::Vec3<T> gridOrigin = Vec3<T>(0, 0, 0);
    // HSim::Vec3<T> gridSpacing = Vec3<T>(1, 1, 1);


	// velocity

	auto velocityGrid = std::make_shared<HSim::FaceCenterGrid3<PRECISION>>(gridResolution);

	auto velGridMat = std::make_shared<HSim::LineFieldMaterial>();
	auto velocityGridGObject = std::make_shared<HSim::FaceCenterGrid3GObject>(velocityGrid, velGridMat);
	auto velocityGridRenderable = std::make_shared<HSim::Renderable>(velocityGrid, velocityGridGObject);

	auto velocityGO = std::make_shared<HSim::GameObject>();
	velocityGO->renderable = velocityGridRenderable;

	root->addChild(velocityGO);

	// density

	auto desityGrid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(gridResolution);

	auto gridMat = std::make_shared<HSim::PointMaterial>();
	auto desityGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(desityGrid, gridMat);
	auto desityGridenderable = std::make_shared<HSim::Renderable>(desityGrid, desityGridGObject);

	auto densityGO = std::make_shared<HSim::GameObject>();
	densityGO->renderable = desityGridenderable;

	root->addChild(densityGO);

	// emitter

	auto emitterGrid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(gridResolution);

	// emitterGrid->parallelForEachCell([&](size_t i, size_t j, size_t k)
	// {
	// 	(*emitterGrid)(i, j, k) = -1.f;
	// });

	for (size_t i = 13; i < 17; i++)
	{
		for (size_t k = 13; k < 17; k++)
		{
			(*emitterGrid)(i, 1, k) = 1.0f;
			// (*emitterGrid)(i, 2, k) = 1.0f;
			// (*emitterGrid)(i, 3, k) = 1.0f;
		}
	}    

	auto emitterGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(emitterGrid, gridMat);
	auto emitterGridenderable = std::make_shared<HSim::Renderable>(emitterGrid, emitterGridGObject);
	
	auto emitterGO = std::make_shared<HSim::GameObject>();
	emitterGO->renderable = emitterGridenderable;

	root->addChild(emitterGO);

	// temperature

	auto temperatureGrid = std::make_shared<HSim::CellCenterScalarGrid3<PRECISION>>(gridResolution);

	auto temperatureGridGObject = std::make_shared<HSim::CellCenterScalarGrid3GObject>(temperatureGrid, gridMat);
	auto temperatureGridenderable = std::make_shared<HSim::Renderable>(temperatureGrid, temperatureGridGObject);

	auto temperatureGO = std::make_shared<HSim::GameObject>();
	temperatureGO->renderable = temperatureGridenderable;

	root->addChild(temperatureGO);

	/**************************************************/

	/**************************************************
	 * solver
	 */
	auto naiveSmokeSolver = std::make_shared<HSim::naiveSmokeSolver>();

	naiveSmokeSolver->setVelocityGO(velocityGO);
	naiveSmokeSolver->setDensityGO(densityGO);
	naiveSmokeSolver->setEmitterGO(emitterGO);
	naiveSmokeSolver->setTemperatureGO(temperatureGO);

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