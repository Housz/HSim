#include <HSim/common.h>
#include <HSim/sphere3.h>

#include <scene/game_object.h>
#include <config/numerical_config.h>
#include <app/app.h>

HSim::SceneGraph_ptr createScene()
{
	auto scene = std::make_shared<HSim::SceneGraph>();

	auto root = std::make_shared<HSim::GameObject>();
	scene->root = root;

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

	HSim::Vec3f start = {0, 0, 0};
	HSim::Vec3f end = {5, 5, 5};
	auto lineHelper = std::make_shared<HSim::LineHelper>(start, end);

	auto material2 = std::make_shared<HSim::BasicMaterial>();
	material2->color = {0.2, 0.8, 0.2};
	auto lineHelperGObject = std::make_shared<HSim::LineHelperGObject>(lineHelper, material2);

	auto lineHelperRenderable = std::make_shared<HSim::Renderable>(lineHelper, lineHelperGObject);

	auto go2 = std::make_shared<HSim::GameObject>();
	go2->renderable = lineHelperRenderable;

	root->addChild(go2);

	/**************************************************/

	return scene;
}

int main()
{
	HSim::App app;

	app.init(1920, 1080);

	app.setScene(createScene());

	app.mainLoop();

	return 0;
}
