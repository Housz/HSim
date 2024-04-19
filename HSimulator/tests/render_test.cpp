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

	auto sphere = std::make_shared<HSim::Sphere3<PRECISION>>();

	auto material = std::make_shared<HSim::BasicMaterial>();
	material->color = {0.8, 0.4, 0.4};
	material->wireframe = true;
	
	auto sphereGraphicsObject = std::make_shared<HSim::Sphere3GObject>(sphere, material);

	auto sphereRenderable = std::make_shared<HSim::Renderable>(sphere, sphereGraphicsObject);

	auto go1 = std::make_shared<HSim::GameObject>();
	go1->renderable = sphereRenderable;


	root->addChild(go1);



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
