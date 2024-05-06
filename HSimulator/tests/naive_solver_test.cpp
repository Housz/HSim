#include <scene/game_object.h>
#include <config/numerical_config.h>
#include <app/app.h>
#include <IO/obj_reader.h>
#include <simulator/simulator.h>

void createScene(HSim::App &app)
{
	auto scene = std::make_shared<HSim::SceneGraph>();
	app.setScene(scene);

	auto root = scene->root;

	/**************************************************/

	auto groudHelper = std::make_shared<HSim::GroundHelper>();

	auto material0 = std::make_shared<HSim::BasicMaterial>();
	material0->color = {0.2, 0.2, 0.2};
	auto groundHelperGObject = std::make_shared<HSim::GroundHelperGObject>(groudHelper, material0);

	auto groundHelperRenderable = std::make_shared<HSim::Renderable>(groudHelper, groundHelperGObject);

	auto go0 = std::make_shared<HSim::GameObject>();
	go0->renderable = groundHelperRenderable;

	root->addChild(go0);

	/**************************************************/

	auto sphere = std::make_shared<HSim::Sphere3<PRECISION>>();

	sphere->transform.translation.y = 10;

	auto material1 = std::make_shared<HSim::BasicMaterial>();
	material1->color = {0.8, 0.4, 0.4};
	material1->wireframe = true;

	auto sphereGraphicsObject = std::make_shared<HSim::Sphere3GObject>(sphere, material1);

	auto sphereRenderable = std::make_shared<HSim::Renderable>(sphere, sphereGraphicsObject);

	auto go1 = std::make_shared<HSim::GameObject>();
	go1->renderable = sphereRenderable;
	// go1->addRenderable( SphereRenderableCreator() )

	root->addChild(go1);

	/**************************************************/


	/**
	 * solver 
	 */

	auto naiveSolver = std::make_shared<HSim::NaiveSolver<PRECISION>>();
	naiveSolver->go = go1;

	app.simulator->solver = naiveSolver;

}


int main()
{
	HSim::App app;

	app.init(1920, 1080);

	createScene(app);

	app.mainLoop();

	return 0;
}
